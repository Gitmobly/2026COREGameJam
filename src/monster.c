#include <math.h>
#include <raymath.h>

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
	if (other == NULL || other->type != E_PLAYER || other->hp <= 0)
	{
		return;
	}

    Vector2 vec_towards = Vector2Subtract(other->pos, mon->pos);
    f32 distance = Vector2Length(vec_towards);

    if (distance < 24)
	{
		mon->vel				  = (Vector2){0};
		app->ui_state->current_ui = init_comabat_ui(mon, app);
		return;
	}

    Vector2 direction = Vector2Normalize(vec_towards);

	mon->vel.x	= direction.x * mon->speed;
	mon->vel.y	= direction.y * mon->speed;
}
