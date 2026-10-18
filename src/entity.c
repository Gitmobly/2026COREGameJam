#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "entity.h"
#include "map.h"

void entity_interactions(map_room_t* room)
{
	entity_t** ents = room->entities;
	for (int i = 0; i < MAP_ENTITY_MAX; i++)
	{
		if (ents[i] == NULL)
		{
			continue;
		}
		for (int j = 0; j < MAP_ENTITY_MAX; j++)
		{
			if (i == j || ents[j] == NULL)
			{
				continue;
			}
			if (ents[i]->type == E_PROJECTILE)
			{
				entity_projectile_interact(ents[i], ents[j], room);
			}
			if (ents[i]->type == E_PLAYER)
			{
				entity_player_interact(ents[i], ents[j], room);
			}
			if (ents[i]->type == E_MONSTER)
			{
				entity_monster_interact(ents[i], ents[j], room);
			}
		}
		if (ents[i]->type == E_PROJECTILE)
		{
			entity_projectile_interact(ents[i], NULL, room);
		}
		if (ents[i]->type == E_PLAYER)
		{
			entity_player_interact(ents[i], NULL, room);
		}
		if (ents[i]->type == E_MONSTER)
		{
			entity_monster_interact(ents[i], NULL, room);
		}
	}

	for (int i = 0; i < MAP_ENTITY_MAX; i++)
	{
		if (ents[i] == NULL)
		{
			continue;
		}

		if (ents[i]->hp <= 0)
		{
			// Todo - free list or just not use ptr array for ents since we have a max bound
			// free(ents[i]);
			ents[i] = NULL;
			continue;
		}

		ents[i]->pos[0] += ents[i]->vel[0];
		ents[i]->pos[1] += ents[i]->vel[1];
	}
}
