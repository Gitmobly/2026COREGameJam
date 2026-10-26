#include <limits.h>

#include "weapon.h"
#include "entity.h"
#include "map.h"
#include "vendor/raylib/raylib.h"

char* str_projectile = "projectile";

weapon_t weapon_init_base()
{
	return (weapon_t){
		.speed	   = 8.0,
		.fire_rate = 1.0,
		.damage	   = 10.0,
		.fire_last = 0.0,
	};
}

void weapon_fire(entity_t* ent, map_room_t* room, Vector2 dir)
{
	f64 now = GetTime();
	if (now - ent->weapon.fire_last < ent->weapon.fire_rate)
	{
		return;
	}

	ent->weapon.fire_last = now;

	entity_t projectile = {
		.hp	  = INT_MAX,
		.pos  = ent->pos,
		.vel  = {dir.x * ent->weapon.speed, dir.y * ent->weapon.speed},
		.type = E_PROJECTILE,
		.name = "projectile",
	};

	map_add_entity(&projectile, room);
}
