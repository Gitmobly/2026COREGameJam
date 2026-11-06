#include <math.h>
#include <raymath.h>

#include "monster.h"
#include "entity.h"
#include "ui.h"
#include "main.h"
#include "cheats.h"

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

    // TODO: we probably dont want an instakill, damage and knockback instead? idk
    if (distance < (mon->radius_damage_deal + other->radius_damage_take))
	{
		mon->vel				  = (Vector2){0};
		app->ui_state->current_ui = init_combat_ui(mon, app);
		return;
	}

    Vector2 direction = Vector2Normalize(vec_towards);

	mon->vel.x	= direction.x * mon->speed;
	mon->vel.y	= direction.y * mon->speed;
}
