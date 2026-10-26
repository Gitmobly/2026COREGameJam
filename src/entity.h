#pragma once

#include "utils.h"
#include "arena.h"

struct weapon_t;
typedef struct map_room_t map_room_t;
typedef struct gamestate gamestate;

typedef enum
{
	E_UNKNOWN,
	E_MONSTER,
	E_PROJECTILE,
	E_PLAYER,
} entity_type_t;

typedef struct entity_t
{
	f64				 pos[2];
	f64				 vel[2];
	f32				 speed;
	s32				 hp;
	char*			 name;
	entity_type_t	 type;
	struct weapon_t* weapon;
} entity_t;


entity_t* entity_player_init(arena_t* arena);
entity_t* entity_init_monster(arena_t* arena);
void	  entity_interactions(gamestate* state);

void entity_projectile_interact(entity_t* proj, entity_t* other, map_room_t* room);
void entity_player_interact(entity_t* player, entity_t* other, map_room_t* room);
void entity_monster_interact(entity_t* mon, entity_t* other, gamestate* gamestate);
