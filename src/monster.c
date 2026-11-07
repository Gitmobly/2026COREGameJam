#include <math.h>
#include <raymath.h>

#include "monster.h"
#include "entity.h"
#include "ui.h"
#include "main.h"
#include "cheats.h"
#include "map.h"
#include "game.h"

entity_t entity_init_monster(void)
{
	return (entity_t){
		.hp	   = CHEATED_VALUE(50, CHEAT_MONSTER_HEALTH),
		.speed = 0.5f,
		.name  = "jeb_",
		.type  = E_MONSTER,
		.pos   = {320.0f, 320.0f},

        .radius_visible     = 16,
        .radius_damage_take = 16,
        .radius_damage_deal = 12,
	};
}

bool endangered_by_projectile(entity_t *proj, entity_t *entity) {
    Vector2 dir_actual = Vector2Normalize(proj->vel);
    Vector2 dir_ideal = Vector2Normalize(Vector2Subtract(entity->pos, proj->pos));

    // NOTE: dot product is symmetrical so one point is enough
    Vector2 point_optimal = Vector2Add(entity->pos, Vector2Scale(v2(-dir_ideal.y, dir_ideal.x), 1.2 * entity->radius_damage_take + proj->radius_damage_deal));
    Vector2 dir_boundary = Vector2Normalize(Vector2Subtract(point_optimal, proj->pos));

    f32 dot_threshold = Vector2DotProduct(dir_ideal, dir_boundary);
    f32 dot_actual = Vector2DotProduct(dir_ideal, dir_actual);

    return (dot_actual >= dot_threshold);
}

void entity_monster_interact(entity_t* mon, entity_t* other, app_state_t* app, f32 dt)
{
	if (other == NULL || (other->type != E_PLAYER && other->type != E_SOUL) || other->hp <= 0)
	{
		return;
	}




    Vector2 vec_towards = Vector2Subtract(other->pos, mon->pos);
    f32 distance = Vector2Length(vec_towards);
    if (other->invulnerability_remaining <= 0 && distance < (mon->radius_damage_deal + other->radius_damage_take))
	{
        other->invulnerability_remaining = 0.2;
        other->knockback = Vector2Scale(Vector2Normalize(Vector2Subtract(other->pos, mon->pos)), 500);
        other->knockbackDragMultiplier = 6;
        other->hit_stun_remaining = 0.25;
        other->hp -= 10;

        mon->knockback = Vector2Scale(Vector2Normalize(Vector2Subtract(other->pos, mon->pos)), -200);
        mon->knockbackDragMultiplier = 6;

		return;
	}

    f32 speed = mon->speed;
    if (distance < 150) {
        speed *= 1.3;
    }



    if (mon->berserk_remaining > 0) {
        // TODO: rename playerLastDirection to lastDirection or smth
        Vector2 direction = Vector2MoveTowards(mon->playerLastDirection, Vector2Normalize(vec_towards), 1.5 * dt);
        mon->playerLastDirection = direction;

        // TODO: this should gradually increase?
        mon->vel.x = speed * direction.x * 9;
        mon->vel.y = speed * direction.y * 9;

        return;
    }


	level_state_t* level = &app->game_state->current_level;
	map_room_t*	   room	 = level->active_room;

    // TODO: this kinda works but idk
    Vector2 avg_vectorFromMonsters = {0};
    int avg_count = 0;

	for (u32 i = 0; i < room->entity_count; i++) {
        entity_t* ent = &room->entities[i];
        if (ent->hp <= 0 || ent->type != E_MONSTER || ent == mon) { continue; }

        Vector2 delta = Vector2Subtract(mon->pos, ent->pos);
        f32 distance = Vector2Length(delta);

        Vector2 scaledDelta = Vector2Scale(delta, 1 / powf(distance, 2));

        avg_vectorFromMonsters = Vector2Add(avg_vectorFromMonsters, scaledDelta);
        avg_count += 1;
    }

    Vector2 vectorFromMonsters = Vector2Scale(avg_vectorFromMonsters, 1.0f / (float)avg_count);
    vectorFromMonsters = Vector2Scale(vectorFromMonsters, 30000);

    f32 avoidanceDistance = 200;

    f32 scale_awayFromMonsters = 1;
    f32 scale_towardsTarget = 1;

    if (Vector2Length(vectorFromMonsters) > avoidanceDistance) {
        scale_awayFromMonsters = 4;

        f32 dot = Vector2DotProduct(Vector2Normalize(vectorFromMonsters), Vector2Normalize(vec_towards));
        if (dot < -0.9) {
            scale_awayFromMonsters = 0.2;
        }
    }
    else {
        scale_towardsTarget = 4;
    }

    if (Vector2Length(vec_towards) < avoidanceDistance) {
        scale_awayFromMonsters = 0;
        scale_towardsTarget = 1;
    } 

    Vector2 direction = Vector2Add(
        Vector2Scale(Vector2Normalize(vectorFromMonsters), scale_awayFromMonsters),
        Vector2Scale(Vector2Normalize(vec_towards), scale_towardsTarget)
    );
    direction = Vector2Normalize(direction);

	mon->vel.x	= direction.x * speed;
	mon->vel.y	= direction.y * speed;



	for (u32 i = 0; i < room->entity_count; i++) {
        if (mon->dodge_cooldown > 0) { break; }
        if (Vector2Length(Vector2Subtract(other->pos, mon->pos)) < 100) { break; }

        entity_t *proj = &room->entities[i];
        if (proj->hp <= 0 || proj->type != E_PROJECTILE || proj->isEnemyProjectile) { continue; }

        if (Vector2Length(Vector2Subtract(proj->pos, mon->pos)) > 250) { continue; }
        if (!endangered_by_projectile(proj, mon)) { continue; }
        TraceLog(LOG_INFO, "ABOBA");


        f32 noticeDistance = Vector2Distance(proj->spawn_pos, mon->pos);
        f32 chanceToDodge = Remap(noticeDistance, 100, 500, 40, 90);
        chanceToDodge = Clamp(chanceToDodge, 50, 90);
        if (GetRandomValue(0, 100) > chanceToDodge) { break; }

        mon->dodge_cooldown = 1.5;

        Vector2 dir = Vector2Normalize(Vector2Subtract(mon->pos, proj->pos));
        dir = v2(-dir.y, dir.x);
        Vector2 dir_toPlayer = Vector2Normalize(Vector2Subtract(other->pos, mon->pos));
        Vector2 dir2 = v2(-dir.x, -dir.y);

        if (Vector2DotProduct(dir, dir_toPlayer) < Vector2DotProduct(dir2, dir_toPlayer)) {
            dir = dir2;
        }

        dir = Vector2Normalize(Vector2Add(dir, dir_toPlayer));

        mon->knockback = Vector2Scale(dir, 800);
        mon->knockbackDragMultiplier = 10;


        break;
    }
}
