#include <math.h>

#include "fx.h"

void fx_emit_monster_hit(fx_system_t* fx, Vector2 pos, Vector2 projectile_dir, b32 fatal)
{
	u32 count = fatal ? 28 : 10;

	for (u32 i = 0; i < count; i++)
	{
		fx_particle_t* p  = &fx->particles[fx->next_particle];
		fx->next_particle = (fx->next_particle + 1) % FX_PARTICLE_MAX;

		f32 angle = (f32)GetRandomValue(0, 6283) * 0.001f;
		f32 speed = (f32)GetRandomValue(85, fatal ? 290 : 210);

		p->pos = (Vector2){
			pos.x + (f32)GetRandomValue(-7, 7),
			pos.y + (f32)GetRandomValue(-7, 7),
		};
		p->vel = (Vector2){
			cosf(angle) * speed + projectile_dir.x * 75.0f,
			sinf(angle) * speed + projectile_dir.y * 75.0f,
		};
		p->lifetime = (f32)GetRandomValue(20, fatal ? 50 : 37) * 0.01f;
		p->life		= p->lifetime;
		p->size		= (f32)GetRandomValue(2, fatal ? 5 : 4);
		p->color	= (i % 5 == 0) ? RAYWHITE : (i % 2 == 0 ? ORANGE : RED);
	}
}

void fx_update(fx_system_t* fx, f32 dt)
{
	f32 drag = fmaxf(0.0f, 1.0f - 6.0f * dt);

	for (u32 i = 0; i < FX_PARTICLE_MAX; i++)
	{
		fx_particle_t* p = &fx->particles[i];
		if (p->life <= 0.0f)
		{
			continue;
		}

		p->life -= dt;
		if (p->life <= 0.0f)
		{
			continue;
		}

		p->pos.x += p->vel.x * dt;
		p->pos.y += p->vel.y * dt;
		p->vel.x *= drag;
		p->vel.y *= drag;
	}
}

void fx_draw(const fx_system_t* fx, Vector2 room_origin)
{
	for (u32 i = 0; i < FX_PARTICLE_MAX; i++)
	{
		const fx_particle_t* p = &fx->particles[i];
		if (p->life <= 0.0f)
		{
			continue;
		}

		f32		alpha	   = p->life / p->lifetime;
		Vector2 screen_pos = {
			room_origin.x + p->pos.x,
			room_origin.y + p->pos.y,
		};
		DrawCircleV(screen_pos, p->size * (0.4f + 0.6f * alpha), Fade(p->color, alpha));
	}
}
