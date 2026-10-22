#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "arena.h"
#include "entity.h"
#include "gamestate.h"
#include "ui.h"

char* monster_name = "jeb_";

entity_t* entity_init_monster(arena_t* arena)
{
	entity_t* mon = arena_push_struct(arena, entity_t);
	mon->hp		  = 50;
	mon->speed	  = 1.5;
	mon->name	  = monster_name;
	mon->type	  = E_MONSTER;
	mon->pos[0]	  = 320;
	mon->pos[1]	  = 320;
	return mon;
}

void entity_monster_interact(entity_t* mon, entity_t* other, gamestate* gamestate)
{
	if (other == NULL)
	{
		return;
	}

	if (other->type == E_PLAYER)
	{
		double mon_dx = other->pos[0] - mon->pos[0];
		double mon_dy = other->pos[1] - mon->pos[1];
		double mon_d  = sqrt(mon_dx * mon_dx + mon_dy * mon_dy);

		mon->vel[0] = mon->speed * mon_dx / mon_d;
		mon->vel[1] = mon->speed * mon_dy / mon_d;

		if (mon_d < 24)
		{
			//other->hp -= 10;

			gamestate->ui_state = COMBAT_UI;
			gamestate->active_ui = init_comabat_ui(mon);

			mon->pos[0] -= mon->vel[0] * 8;
			mon->pos[1] -= mon->vel[1] * 8;
		}
	}
}
