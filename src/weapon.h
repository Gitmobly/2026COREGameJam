#pragma once

#include "utils.h"
#include "vendor/raylib/raylib.h"

typedef struct entity_t	  entity_t;
typedef struct map_room_t map_room_t;

typedef struct
{
	f32 fire_rate;
	f32 damage;
	f32 speed;
	f64 fire_last;
} weapon_t;

void weapon_fire(entity_t* ent, map_room_t* room, Vector2 dir);

weapon_t weapon_init_base();
