#include <math.h>

#include "monster.h"
#include "entity.h"
#include "ui.h"
#include "main.h"

entity_t entity_init_monster(void)
{
	return (entity_t){
		.hp	   = 50,
		.speed = 0.5f,
		.name  = "jeb_",
		.type  = E_MONSTER,
		.pos   = {320.0f, 320.0f},
	};
}

void entity_monster_interact(entity_t* mon, entity_t* other, app_state_t* app)
{
	if (!other || other->type != E_PLAYER || other->hp <= 0)
	{
		return;
	}

	f32 dx = other->pos.x - mon->pos.x;
	f32 dy = other->pos.y - mon->pos.y;

	f32 distSq = dx * dx + dy * dy;

	if (distSq <= 24.0f * 24.0f)
	{
		mon->vel				  = (Vector2){0};
		app->ui_state->current_ui = init_comabat_ui(mon, app);
		return;
	}

	f32 invDist = 1.0f / sqrtf(distSq);
	mon->vel.x	= dx * invDist * mon->speed;
	mon->vel.y	= dy * invDist * mon->speed;
}
