#ifndef FX_H
#define FX_H

#include "utils.h"
#include "vendor/raylib/raylib.h"

#define FX_PARTICLE_MAX 192

typedef struct fx_particle_t
{
	Vector2 pos;
	Vector2 vel;
	f32		life;
	f32		lifetime;
	f32		size;
	Color	color;
} fx_particle_t;

typedef struct fx_system_t
{
	fx_particle_t particles[FX_PARTICLE_MAX];
	u32			  next_particle;
} fx_system_t;

void fx_emit_monster_hit(fx_system_t* fx, Vector2 pos, Vector2 projectile_dir, b32 fatal);
void fx_update(fx_system_t* fx, f32 dt);
void fx_draw(const fx_system_t* fx, Vector2 room_origin);

#endif
