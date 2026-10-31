#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "entity.h"
#include "input.h"
#include "weapon.h"
#include "graphics.h"

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

void entity_player_interact(entity_t* player, entity_t* other, map_room_t* room, input_state_t* inp)
{
	if (other != NULL)
	{
		return;
	}

	weapon_update(&player->weapon);

	player->vel.x = player->speed * inp->move.x;
	player->vel.y = player->speed * inp->move.y;

	if (inp->fire)
	{
		Vector2 mouse = GetMousePosition();

		Vector2 dir = {
			mouse.x - (player->pos.x + PAD),
			mouse.y - (player->pos.y + PAD),
		};

		f32 lenSq = dir.x * dir.x + dir.y * dir.y;

		if (lenSq > 0.0f)
		{
			f32 invLen = 1.0f / sqrtf(lenSq);

			dir.x *= invLen;
			dir.y *= invLen;

			weapon_fire(player, room, dir);
		}
	}
}
