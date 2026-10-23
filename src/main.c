#include "input.h"
#include "vendor/raylib/raylib.h"

#include "graphics.h"
#include "map.h"
#include "entity.h"
#include "weapon.h"
#include "arena.h"
#include "gamestate.h"
#include "ui.h"

int main(void)
{
	graphics_init();

	arena_t* permanent_arena = arena_init(MEGABYTES(64), ARENA_NO_FLAGS);

	map_world_t* world	 = map_init_world();
	entity_t*	 monster = entity_init_monster(permanent_arena);
	entity_t*	 player	 = entity_player_init(permanent_arena);
	player->weapon		 = weapon_init_base(permanent_arena);

	map_room_t* room  = &world->rooms[8][8];
	room->entities[0] = player;
	room->entities[1] = monster;

	ui_t* start = init_start_ui();
	
	gamestate state = {
		.ui_state = START_UI,
		.active_room = room,
		.active_ui = start,
		.player = player,
	};

	while (!WindowShouldClose())
	{
		graphics_loop(&state);
		input_update_ui(&state);

		if(state.ui_state == GAME) {
			// Pause entity interactions if ui switches
			entity_interactions(&state);
		}
	}

	CloseWindow();
}
