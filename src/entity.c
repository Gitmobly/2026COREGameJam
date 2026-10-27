#include <math.h>

#include "entity.h"

#include "game.h"
#include "monster.h"
#include "projectile.h"
#include "map.h"
#include "player.h"
#include "main.h"
#include "ui.h"

void entity_interactions(app_state_t* app)
{
	level_state_t* level = &app->game_state->current_level;
	map_room_t*	   room	 = level->active_room;

	entity_player_interact(level->player_entity, NULL, room);

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

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		ent->pos.x += ent->vel.x;
		ent->pos.y += ent->vel.y;
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
