#pragma once

#include "utils.h"
#include "arena.h"
#include "weapon.h"
#include "vendor/raylib/raylib.h"

typedef struct map_room_t	 map_room_t;
typedef struct level_state_t level_state_t;
typedef struct app_state_t	 app_state_t;

typedef enum
{
	E_UNKNOWN,
	E_MONSTER,
	E_PROJECTILE,
	E_PLAYER,
} entity_type_t;

typedef struct entity_t
{
	Vector2		  pos;
	Vector2		  vel;
	f32			  speed;
	s32			  hp;
	char*		  name;
	entity_type_t type;
	weapon_t	  weapon;
} entity_t;

entity_t* entity_player_init(arena_t* arena);
entity_t* entity_init_monster(arena_t* arena);
void	  entity_interactions(app_state_t* app);
