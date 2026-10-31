#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"

typedef struct entity_t	  entity_t;
typedef struct arena_t	  arena_t;
typedef struct map_room_t map_room_t;

entity_t* entity_player_init(arena_t* arena);
void	  entity_player_interact(entity_t* player, entity_t* other, map_room_t* room, input_state_t* inp);

#endif
