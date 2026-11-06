#ifndef GAME_H
#define GAME_H

#include "utils.h"
#include <raylib.h>
#include "entity.h"

typedef struct map_room_t  map_room_t;
typedef struct entity_t	   entity_t;
typedef struct app_state_t app_state_t;

typedef struct level_state_t
{
	map_room_t* active_room;

    entity_t*   entity_controlled;
    entity_t*   entity_player;
    entity_t*   entity_soul;
} level_state_t;

typedef struct game_state_t
{
	level_state_t current_level;

	f32 wall_shake_remaining;
	f32 hit_shake_remaining;
	b32 player_hitting_wall;

    b32         soul_active;
    b32         soul_needsToBeSpawned;
    b32         soul_needsToBeDespawned;
    entity_t    soul_entityToSpawn;

	u32 room_number;
} game_state_t;

void game_state_init(game_state_t* state);
void game_next_room(app_state_t* app);
#endif
