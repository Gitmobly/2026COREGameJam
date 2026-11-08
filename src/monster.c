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

entity_t entity_init_sniper() {
	return (entity_t){
		.hp	   = CHEATED_VALUE(70, CHEAT_MONSTER_HEALTH),
		.speed = 0.5f,
		.name  = "jeb_",
		.type  = E_SNIPER,
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

void entity_sniper_interact(entity_t* sniper, entity_t* other, app_state_t* app, f32 dt) {
	if (other == NULL || (other->type != E_PLAYER && other->type != E_SOUL) || other->hp <= 0)
	{
		return;
	}

    dt = dt;
    Vector2 vec_towards = Vector2Subtract(other->pos, sniper->pos);
    f32 distance = Vector2Length(vec_towards);
    Vector2 dir = Vector2Normalize(vec_towards);

	level_state_t* level = &app->game_state->current_level;
	map_room_t*	   room	 = level->active_room;

    if (sniper->hit_stun_remaining <= 0 && sniper->sniper_charging && sniper->sniper_shotsFired < 3 && sniper->sniper_shotDelay <= 0) {
        sniper->sniper_shotsFired += 1;

        entity_t projectile = {
            .hp	 = 1,
            .pos = sniper->pos,
            .spawn_pos = sniper->pos,
            .vel = Vector2Scale(dir, 9),
            .type = E_PROJECTILE,
            .name = "projectile",

            .isEnemyProjectile = true,

            .projectileDamage = 15,

            .radius_visible     = 8,
            .radius_damage_deal = 8,
            .radius_damage_take = 8,
        };

        map_add_entity(&projectile, room);

        sniper->sniper_shotDelay = 0.1;
    }

    if (sniper->sniper_shotsFired >= 3) {
        sniper->sniper_charging = false;
        sniper->sniper_shotsFired = 0;
        sniper->sniper_shotDelay = 0;
    }

    if (sniper->reload_remaining <= 0) {
        sniper->reload_remaining = 2.5;

        sniper->hit_stun_remaining = 0.7;
        sniper->sniper_charging = true;

        return;
    }




    if (distance < 400 && Vector2Length(sniper->knockback) < 10) {
        sniper->knockback = Vector2Scale(dir, -900);
        sniper->knockbackDragMultiplier = 10;
    }

    if (distance > 500) {
        sniper->vel = Vector2Scale(dir, sniper->speed);
    }





    // NOTE: copypaste from entity_monster_interact, might want to put in a function
	for (u32 i = 0; i < room->entity_count; i++) {
        if (sniper->dodge_cooldown > 0) { break; }

        entity_t *proj = &room->entities[i];
        if (proj->hp <= 0 || proj->type != E_PROJECTILE || proj->isEnemyProjectile) { continue; }

        if (Vector2Length(Vector2Subtract(proj->pos, sniper->pos)) > 250) { continue; }
        if (!endangered_by_projectile(proj, sniper)) { continue; }


        f32 noticeDistance = Vector2Distance(proj->spawn_pos, sniper->pos);
        f32 chanceToDodge = Remap(noticeDistance, 100, 500, 40, 90);
        chanceToDodge = Clamp(chanceToDodge, 70, 90);
        if (GetRandomValue(0, 100) > chanceToDodge) { break; }

        sniper->dodge_cooldown = 1.0;

        Vector2 dir = Vector2Normalize(Vector2Subtract(sniper->pos, proj->pos));
        dir = v2(-dir.y, dir.x);
        Vector2 dir_toPlayer = Vector2Normalize(Vector2Subtract(other->pos, sniper->pos));
        Vector2 dir2 = v2(-dir.x, -dir.y);

        if (Vector2DotProduct(dir, dir_toPlayer) < Vector2DotProduct(dir2, dir_toPlayer)) {
            dir = dir2;
        }

        dir = Vector2Normalize(Vector2Add(dir, dir_toPlayer));

        sniper->knockback = Vector2Scale(dir, 800);
        sniper->knockbackDragMultiplier = 10;


        break;
    }
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
        s32 damage = 10;
        // NOTE: berserk can just tear you to pieces in an instant
        if (mon->berserk_remaining > 0) {
            damage = 8;
        }

        other->invulnerability_remaining = 0.2;
        other->knockback = Vector2Scale(Vector2Normalize(Vector2Subtract(other->pos, mon->pos)), 500);
        other->knockbackDragMultiplier = 6;
        other->hit_stun_remaining = 0.25;
        other->hp -= damage;

        mon->knockback = Vector2Scale(Vector2Normalize(Vector2Subtract(other->pos, mon->pos)), -200);
        mon->knockbackDragMultiplier = 6;
        mon->playerLastDirection = Vector2Normalize(vec_towards);

		return;
	}

    f32 speed = mon->speed;
    if (distance < 200) {
        speed *= 1.3;
    }



    if (mon->berserk_remaining > 0) {
        // TODO: rename playerLastDirection to lastDirection or smth
        Vector2 direction = Vector2MoveTowards(mon->playerLastDirection, Vector2Normalize(vec_towards), 1.5 * dt);
        if (mon->justHitWall) {
            direction = Vector2Scale(Vector2Normalize(vec_towards), 0.5);
        }

        mon->playerLastDirection = direction;


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
