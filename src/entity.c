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

#include <raymath.h>

void entity_interactions(app_state_t* app)
{
	level_state_t* level = &app->game_state->current_level;
	map_room_t*	   room	 = level->active_room;
	game_state_t*  game	 = app->game_state;
	f32			   dt	 = GetFrameTime();

	fx_update(&room->fx, dt);
	game->hit_shake_remaining = ClampLo(game->hit_shake_remaining - dt, 0);

	entity_player_interact(level->player_entity, room, app->input_state);

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		if (ent->type == E_MONSTER)
		{
            ent->hit_flash_remaining = ClampLo(ent->hit_flash_remaining - dt, 0);
            ent->hit_stun_remaining  = ClampLo(ent->hit_stun_remaining  - dt, 0);

			if (ent->hit_stun_remaining > 0)
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

    game->wall_shake_remaining = ClampLo(game->wall_shake_remaining - dt, 0);

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

        // TODO: arent we forgetting dt? it seems that we are, but i dont wanna touch it
        // (also need to increase speed by like 60 in that case)
		Vector2 next = {
			ent->pos.x + ent->vel.x,
			ent->pos.y + ent->vel.y,
		};

		if (ent->type == E_MONSTER)
		{
			next.x += ent->knockback.x * dt;
			next.y += ent->knockback.y * dt;

            f32 radius = ent->radius_visible;
            next.x = Clamp(next.x, 0 + radius, GAME_WD - radius);
            next.y = Clamp(next.y, 0 + radius, GAME_HT - radius);

			f32 drag = ClampLo(1 - 10 * dt, 0);
			ent->knockback.x *= drag;
			ent->knockback.y *= drag;
		}

		if (ent->type == E_PLAYER)
		{
            f32 radius = ent->radius_visible;

            f32 clamp_next_x = Clamp(next.x, 0 + radius, GAME_WD - radius);
            f32 clamp_next_y = Clamp(next.y, 0 + radius, GAME_HT - radius);
            b32 hit_wall = (clamp_next_x != next.x) || (clamp_next_y != next.y);

			f32 exit_y		 = GAME_HT * 0.5f;
			f32 half_opening = MAP_EXIT_HALF_HEIGHT - radius;
			b32 inside_exit	 = next.y >= exit_y - half_opening && next.y <= exit_y + half_opening;

			if (room->exit_open && inside_exit && next.x > (GAME_WD - radius))
			{
				game_next_room(app);
				return;
			}

			if (hit_wall && !game->player_hitting_wall)
			{
				game->wall_shake_remaining = 0.15f;
			}

			game->player_hitting_wall = hit_wall;
            next.x = clamp_next_x;
            next.y = clamp_next_y;
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
