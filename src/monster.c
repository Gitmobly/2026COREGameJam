#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "entity.h"

char* monster_name = "jeb_";

entity_t* entity_init_monster(void)
{
	entity_t* mon = malloc(sizeof(entity_t));
	mon->hp		  = 50;
	mon->speed	  = 1.5;
	mon->name	  = monster_name;
	mon->type	  = E_MONSTER;
	mon->pos[0]	  = 320;
	mon->pos[1]	  = 320;
	return mon;
}

void entity_monster_interact(entity_t* mon, entity_t* other, map_room_t* room)
{
	(void)room;
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
			other->hp -= 10;
			mon->pos[0] -= mon->vel[0] * 8;
			mon->pos[1] -= mon->vel[1] * 8;
		}
	}
}
