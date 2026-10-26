#include "input.h"
#include "vendor/raylib/raylib.h"

#include "graphics.h"
#include "map.h"
#include "entity.h"
#include "weapon.h"
#include "arena.h"
#include "ui.h"
#include "game.h"
#include "main.h"

int main(void)
{
	arena_t* permanent_arena = arena_init(MEGABYTES(16), ARENA_NO_FLAGS);
	arena_t* level_arena	 = arena_init(MEGABYTES(16), ARENA_NO_FLAGS);
	arena_t* scratch_arena	 = arena_init(MEGABYTES(16), ARENA_NO_FLAGS);
	arena_t* ui_arena		 = arena_init(MEGABYTES(16), ARENA_NO_FLAGS);

	allocators_t allocators = {
		.permanent = permanent_arena,
		.level	   = level_arena,
		.scratch   = scratch_arena,
		.ui		   = ui_arena,
	};

	InitWindow(SCREEN_WD, SCREEN_HT, "2026 Core Game Jam");
	SetTargetFPS(60);

	app_state_t*  app_state	 = arena_push_struct(permanent_arena, app_state_t);
	game_state_t* game_state = arena_push_struct(permanent_arena, game_state_t);
	ui_state_t*	  ui_state	 = arena_push_struct(permanent_arena, ui_state_t);

	game_state_init(game_state);
	ui_state_init(ui_state, &allocators);

	app_state->game_state = game_state;
	app_state->ui_state	  = ui_state;

	map_world_t* world = arena_push_struct_zero(level_arena, map_world_t);

	entity_t* monster = entity_init_monster(permanent_arena);
	entity_t* player  = entity_player_init(permanent_arena);
	player->weapon	  = weapon_init_base();

	map_room_t* room = &world->rooms[8][8];

	map_add_entity(player, room);
	map_add_entity(monster, room);

	game_state->current_level.active_room	= room;
	game_state->current_level.player_entity = &room->entities[0];

	while (!WindowShouldClose())
	{
		render(app_state);
		input_update_ui(app_state);

		if (app_state->ui_state->current_ui->screen == GAME)
		{
			// Pause entity interactions if ui switches
			entity_interactions(app_state);
		}
	}

	CloseWindow();
}
