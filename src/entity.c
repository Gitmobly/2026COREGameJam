#include <math.h>

#include "entity.h"

#include "game.h"
#include "monster.h"
#include "projectile.h"
#include "map.h"
#include "player.h"
#include "main.h"
#include "graphics.h"
#include "ui.h"

void entity_interactions(app_state_t* app)
{
	level_state_t* level = &app->game_state->current_level;
	map_room_t*	   room	 = level->active_room;
	game_state_t*  game	 = app->game_state;
	f32			   dt	 = GetFrameTime();

	fx_update(&room->fx, dt);
	game->hit_shake_remaining = fmaxf(0.0f, game->hit_shake_remaining - dt);

	entity_player_interact(level->player_entity, NULL, room, app->input_state);

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		if (ent->type == E_MONSTER)
		{
			ent->hit_flash_remaining = fmaxf(0.0f, ent->hit_flash_remaining - dt);
			ent->hit_stun_remaining	 = fmaxf(0.0f, ent->hit_stun_remaining - dt);

			if (ent->hit_stun_remaining > 0.0f)
			{
				ent->vel = (Vector2){0};
				continue;
			}

			entity_monster_interact(ent, level->player_entity, app);

			if (app->ui_state->current_ui->screen != GAME)
			{
				return;
			}
		}
	}

	game->wall_shake_remaining -= dt;

	if (game->wall_shake_remaining < 0.0f)
	{
		game->wall_shake_remaining = 0.0f;
	}

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		Vector2 next = {
			ent->pos.x + ent->vel.x,
			ent->pos.y + ent->vel.y,
		};

		if (ent->type == E_MONSTER)
		{
			next.x += ent->knockback.x * dt;
			next.y += ent->knockback.y * dt;

			const f32 radius = 16.0f;
			next.x			 = fmaxf(radius, fminf(next.x, SCREEN_WD - PAD * 2 - radius));
			next.y			 = fmaxf(radius, fminf(next.y, SCREEN_HT - PAD * 2 - radius));

			f32 drag = fmaxf(0.0f, 1.0f - 10.0f * dt);
			ent->knockback.x *= drag;
			ent->knockback.y *= drag;
		}

		if (ent->type == E_PLAYER)
		{
			const f32 radius = 16.0f;

			const f32 min_x = radius;
			const f32 min_y = radius;

			const f32 max_x = SCREEN_WD - PAD * 2 - radius;
			const f32 max_y = SCREEN_HT - PAD * 2 - radius;

			f32 exit_y		 = (SCREEN_HT - PAD * 2) * 0.5f;
			f32 half_opening = MAP_EXIT_HALF_HEIGHT - radius;
			b32 inside_exit	 = next.y >= exit_y - half_opening && next.y <= exit_y + half_opening;

			if (room->exit_open && inside_exit && next.x > max_x)
			{
				game_next_room(app);
				return;
			}

			b32 hit_wall = next.x < min_x || next.x > max_x || next.y < min_y || next.y > max_y;

			if (hit_wall && !game->player_hitting_wall)
			{
				game->wall_shake_remaining = 0.15f;
			}

			game->player_hitting_wall = hit_wall;
			next.x					  = fmaxf(min_x, fminf(next.x, max_x));
			next.y					  = fmaxf(min_y, fminf(next.y, max_y));
		}

		ent->pos = next;
	}

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* proj = &room->entities[i];

		if (proj->type != E_PROJECTILE || proj->hp <= 0)
		{
			continue;
		}

		entity_projectile_interact(proj, NULL, room);

		if (proj->hp <= 0)
		{
			continue;
		}

		for (u32 j = 0; j < room->entity_count; j++)
		{
			entity_t* target = &room->entities[j];

			if (target->type != E_MONSTER || target->hp <= 0)
			{
				continue;
			}

			s32 previous_hp = target->hp;
			entity_projectile_interact(proj, target, room);

			if (target->hp < previous_hp)
			{
				game->hit_shake_remaining = 0.09f;
			}

			if (proj->hp <= 0)
			{
				break;
			}
		}
	}

	if (!room->exit_open)
	{
		b32 monsters_alive = 0;

		for (u32 i = 0; i < room->entity_count; i++)
		{
			entity_t* ent = &room->entities[i];

			if (ent->type == E_MONSTER && ent->hp > 0)
			{
				monsters_alive = 1;
				break;
			}
		}

		if (!monsters_alive)
		{
			room->exit_open		  = 1;
			room->wall_break_time = (f32)GetTime();

			game->wall_shake_remaining = 0.15f;
		}
	}
}
