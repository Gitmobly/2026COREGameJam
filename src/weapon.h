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

	u32 ammo;
	u32 max_ammo;

	f32 reload_time;
	f64 reload_until;
	b32 reloading;

	u32 shots_fired;
	f64 last_shot_time;
} weapon_t;

weapon_t weapon_init_base(void);
void	 weapon_update(weapon_t* weapon);
void	 weapon_fire(entity_t* ent, map_room_t* room, Vector2 dir);
