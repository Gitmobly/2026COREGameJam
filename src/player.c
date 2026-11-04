#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <raymath.h>

#include "map.h"
#include "entity.h"
#include "input.h"
#include "weapon.h"
#include "graphics.h"

char* player_name = "The Hero of Our Story";

entity_t* entity_player_init(arena_t* arena)
{
	entity_t* player = arena_push_struct_zero(arena, entity_t);
    *player = (entity_t){
        .pos    = v2(40, 40),
        .speed  = 4,
        .hp     = 100,
        .name   = player_name,
        .type   = E_PLAYER,
    };

	return player;
}

void entity_player_interact(entity_t* player, map_room_t* room, input_state_t* inp)
{
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

        if (Vector2Equals(dir, v2(0, 0))) { return; }

        weapon_fire(player, room, Vector2Normalize(dir));
	}
}
