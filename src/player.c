#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>

#include "map.h"
#include "entity.h"
#include "input.h"
#include "weapon.h"
#include "graphics.h"
#include "cheats.h"

char* player_name = "The Hero of Our Story";

entity_t* entity_player_init(arena_t* arena)
{
	entity_t* player = arena_push_struct_zero(arena, entity_t);
    *player = (entity_t){
        .pos    = v2(40, 40),
        .speed  = 4,
        .hp     = CHEATED_VALUE(CHEAT_PLAYER_HEALTH, 100),
        .name   = player_name,
        .type   = E_PLAYER,

        .radius_visible     = 16,
        .radius_damage_take = 12,
        .radius_damage_deal = 16,
    };

	return player;
}

void entity_player_interact(entity_t* player, map_room_t* room, input_state_t* inp, f32 dt)
{
    Vector2 direction = Vector2MoveTowards(player->playerLastDirection, inp->move, 20 * dt);
    player->playerLastDirection = direction;

    // Vector2 direction = inp->move;
    // dt = dt;

	player->vel.x = player->speed * direction.x;
	player->vel.y = player->speed * direction.y;


    if (player->type == E_PLAYER) {
        weapon_update(&player->weapon);

        if (inp->fire)
        {
            Vector2 mouse = GetMousePosition();

            Vector2 dir = {
                mouse.x - (player->pos.x + BOUNDARY_WIDTH),
                mouse.y - (player->pos.y + BOUNDARY_WIDTH),
            };

            if (Vector2Equals(dir, v2(0, 0))) { return; }

            weapon_fire(player, room, Vector2Normalize(dir));
        }
    }

    if (player->type == E_SOUL) {

    }
}
