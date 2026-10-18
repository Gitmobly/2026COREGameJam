#pragma once

#include "arena.h"
#include "entity.h"
#include "map.h"

typedef struct weapon_t
{
	f64 fire_rate;
	f64 damage;
	f64 speed;
	f64 fire_last;
} weapon_t;

void weapon_fire(entity_t* ent, map_room_t* room, int dir[2]);

weapon_t* weapon_init_base(arena_t* arena);
