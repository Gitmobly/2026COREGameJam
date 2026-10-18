#pragma once

#include <stdbool.h>

#include "entity.h"

typedef struct weapon_t weapon_t;

enum
{
	MAP_ENTITY_MAX = 128,
	MAP_WORLD_SIZE = 16,
	MAP_LOOT_MAX   = 8
};

typedef struct map_room_t
{
	entity_t* entities[MAP_ENTITY_MAX];
	weapon_t* loot[MAP_LOOT_MAX];
} map_room_t;

typedef struct
{
	bool open;
	bool locked_enemies;
	int	 locked_key;
} map_door_t;

typedef struct
{
	map_room_t rooms[MAP_WORLD_SIZE][MAP_WORLD_SIZE];
	map_door_t doors[MAP_WORLD_SIZE - 1][MAP_WORLD_SIZE - 1];
} map_world_t;

map_world_t* map_init_world(void);

bool map_add_entity(entity_t* entity, map_room_t* room);
