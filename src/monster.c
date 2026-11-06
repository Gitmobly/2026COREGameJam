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

void entity_monster_interact(entity_t* mon, entity_t* other, app_state_t* app)
{
	if (other == NULL || (other->type != E_PLAYER && other->type != E_SOUL) || other->hp <= 0)
	{
		return;
	}

    Vector2 vec_towards = Vector2Subtract(other->pos, mon->pos);
    f32 distance = Vector2Length(vec_towards);


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

	mon->vel.x	= direction.x * mon->speed;
	mon->vel.y	= direction.y * mon->speed;






    // TODO: we probably dont want an instakill, damage and knockback instead? idk
    if (other->invulnerability_remaining <= 0 && distance < (mon->radius_damage_deal + other->radius_damage_take))
	{
		mon->vel				  = (Vector2){0};
		app->ui_state->current_ui = init_combat_ui(mon, app);
		return;
	}
}
