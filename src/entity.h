#pragma once

#include "utils.h"
#include "arena.h"
#include "weapon.h"
#include <raylib.h>

typedef struct map_room_t	 map_room_t;
typedef struct level_state_t level_state_t;
typedef struct app_state_t	 app_state_t;

typedef enum
{
	E_UNKNOWN,

	E_MONSTER,
    E_TURRET,

	E_PROJECTILE,

	E_PLAYER,
    E_SOUL,
} entity_type_t;

#define MONSTER_HIT_FLASH_TIME 0.12f

typedef struct entity_t
{
	Vector2		  pos;
	Vector2		  vel;
	f32			  speed;
	s32			  hp;
	char*		  name;
	entity_type_t type;
	weapon_t	  weapon;

    f32           radius_visible;       // colliding with walls and rendering
    f32           radius_damage_take;   // taking damage
    f32           radius_damage_deal;   // dealing damage

    b32 isEnemyProjectile;
    s32           projectileDamage;

	Vector2 knockback;
    float   knockbackDragMultiplier;
	f32		hit_stun_remaining;
	f32		hit_flash_remaining;
    f32     invulnerability_remaining;
    f32     reload_remaining;
} entity_t;

entity_t* entity_player_init(arena_t* arena);
void	  entity_interactions(app_state_t* app);
