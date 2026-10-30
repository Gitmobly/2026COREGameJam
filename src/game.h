#ifndef GAME_H
#define GAME_H

#include "utils.h"

typedef struct map_room_t map_room_t;
typedef struct entity_t	  entity_t;

typedef struct level_state_t
{
	map_room_t* active_room;
	entity_t*	player_entity;
} level_state_t;

typedef struct game_state_t
{
	level_state_t current_level;

	f32 wall_shake_remaining;
	b32 player_hitting_wall;
} game_state_t;

void game_state_init(game_state_t* state);
#endif
