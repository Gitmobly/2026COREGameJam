#pragma once

#include "entity.h"
#include "loot.h"
#include "fx.h"

#define MAP_ENTITY_MAX		 128
#define MAP_WORLD_SIZE		 16
#define MAP_LOOT_MAX		 8
#define MAP_EXIT_HALF_HEIGHT 50.0f

typedef struct map_room_t
{
	entity_t entities[MAP_ENTITY_MAX];
	u32		 entity_count;

	loot_t loot[MAP_LOOT_MAX];
	u32	   loot_count;

	b32			exit_open;
	f32			wall_break_time;
	fx_system_t fx;
} map_room_t;

typedef struct
{
	b32 open;
	b32 locked_enemies;
	s32 locked_key;
} map_door_t;

typedef struct
{
	map_room_t rooms[MAP_WORLD_SIZE][MAP_WORLD_SIZE];
	u32		   room_count;
	map_door_t doors[MAP_WORLD_SIZE - 1][MAP_WORLD_SIZE - 1];
	u32		   door_count;
} map_world_t;

b32	 map_add_entity(entity_t* entity, map_room_t* room);
void map_generate_room(map_room_t* room, const entity_t* player, u32 depth);
