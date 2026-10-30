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
			entity_monster_interact(ent, level->player_entity, app);

			if (app->ui_state->current_ui->screen != GAME)
			{
				return;
			}
		}
	}

	game_state_t* game = app->game_state;

	game->wall_shake_remaining -= GetFrameTime();

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

		if (ent->type == E_PLAYER)
		{
			const f32 radius = 16.0f;

			const f32 min_x = radius;
			const f32 min_y = radius;

			const f32 max_x = SCREEN_WD - PAD * 2 - radius;
			const f32 max_y = SCREEN_HT - PAD * 2 - radius;

			b32 hit_wall = next.x < min_x || next.x > max_x || next.y < min_y || next.y > max_y;

			if (hit_wall && !game->player_hitting_wall)
			{
				game->wall_shake_remaining = 0.15f;
			}

			game->player_hitting_wall = hit_wall;

			next.x = fmaxf(min_x, fminf(next.x, max_x));
			next.y = fmaxf(min_y, fminf(next.y, max_y));
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

			entity_projectile_interact(proj, target, room);

			if (proj->hp <= 0)
			{
				break;
			}
		}
	}
}
