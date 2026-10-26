#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "entity.h"
#include "input.h"
#include "weapon.h"

char* player_name = "The Hero of Our Story";

entity_t* entity_player_init(arena_t* arena)
{
	entity_t* pla = arena_push_struct_zero(arena, entity_t);
	pla->hp		  = 100;
	pla->speed	  = 4;
	pla->name	  = player_name;
	pla->type	  = E_PLAYER;
	pla->pos.x	  = 40;
	pla->pos.y	  = 40;
	return pla;
}

void entity_player_interact(entity_t* player, entity_t* other, map_room_t* room)
{
	(void)room;
	(void)player;
	(void)other;

	if (other == NULL)
	{
		input_state_t inp = input_update_entity();
		player->vel.x	  = player->speed * inp.move[0];
		player->vel.y	  = player->speed * inp.move[1];

		if (inp.fire[0] != 0 || inp.fire[1] != 0)
		{
			Vector2 dir = {
				(f32)inp.fire[0],
				(f32)inp.fire[1],
			};
			weapon_fire(player, room, dir);
		}

		return;
	}
}
