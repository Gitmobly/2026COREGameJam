#include <limits.h>

#include "weapon.h"
#include "entity.h"
#include "map.h"
#include <raylib.h>
#include <raymath.h>
#include "cheats.h"

char* str_projectile = "projectile";

weapon_t weapon_init_base(void)
{
	return (weapon_t){
		.speed	   = 8.0f,
		.fire_rate = 0.25f,
		.damage	   = 10.0f,
		.fire_last = -1.0,

        .ammo         = CHEATED_VALUE(CHEAT_AMMO, 6),
		.max_ammo	  = 6,
		.reload_time  = 1.25f,
		.reload_until = 0.0,
		.reloading	  = 0,

		.shots_fired	= 0,
		.last_shot_time = -1.0,
	};
}

void weapon_update(weapon_t* w)
{
	if (w->reloading && GetTime() >= w->reload_until)
	{
		w->ammo		 = w->max_ammo;
		w->reloading = 0;
	}
}

void weapon_fire(entity_t* ent, map_room_t* room, Vector2 dir)
{
	weapon_t* w	  = &ent->weapon;
	f64		  now = GetTime();

	if (w->reloading || w->ammo == 0)
	{
		return;
	}

	if (now - w->fire_last < w->fire_rate)
	{
		return;
	}

	entity_t projectile = {
		.hp	 = 1,
		.pos = ent->pos,
        .spawn_pos = ent->pos,
		.vel = Vector2Scale(dir, w->speed),
		.type = E_PROJECTILE,
		.name = "projectile",

        .projectileDamage = 10,

        .radius_visible     = 8,
        .radius_damage_deal = 8,
        .radius_damage_take = 8,
	};

	if (!map_add_entity(&projectile, room))
	{
		return;
	}

	w->ammo--;
	w->fire_last = now;

	w->last_shot_time = now;
	w->shots_fired++;

	if (w->ammo == 0)
	{
		w->reloading	= true;
		w->reload_until = now + w->reload_time;
	}
}
