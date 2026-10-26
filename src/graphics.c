#include "entity.h"
#include "ui.h"
#include "vendor/raylib/raylib.h"
#include <string.h>

#include "graphics.h"
#include "gamestate.h"

Color get_color(entity_t* entity)
{
	return entity->type == E_MONSTER ? RED : SKYBLUE;
}

int get_size(entity_t* entity)
{
	return entity->type == E_PROJECTILE ? 8 : 16;
}

void graphics_init(void)
{
	InitWindow(SCREEN_WD, SCREEN_HT, "2026 Core Game Jam");
	SetTargetFPS(60);
}

void graphics_loop(gamestate* state)
{
	BeginDrawing();
	ClearBackground(DARKGRAY);

	DrawRectangle(PAD, PAD, SCREEN_WD - (2 * PAD), SCREEN_HT - (2 * PAD), BLACK);

	switch (state->ui_state)
	{
		case START_UI:
			draw_start_ui(state);
			break;
		case GAME:
			draw_game(state);
			break;
		case COMBAT_UI:
			draw_combat_ui(state, NULL);
			break;
		case OPTIONS_UI:
			draw_options_ui(state);
		case NO_OWNER:
			break;
	}

	EndDrawing();
}

void draw_start_ui(gamestate* state)
{
	render_current_ui(state->active_ui);
}

void draw_options_ui(gamestate* state)
{
	render_current_ui(state->active_ui);
}

void draw_combat_ui(gamestate* state, entity_t* entity)
{
	(void)entity;

	// todo render combat ui
	DrawRectangle(PAD, SCREEN_HT - 100 - PAD, SCREEN_WD - (PAD * 2), 100, LIGHTGRAY);
	DrawRectangle(PAD + 130, SCREEN_HT - 100 - PAD, 10, 100, BLACK);
	DrawRectangle(PAD + 500, SCREEN_HT - 100 - PAD, 10, 100, BLACK);
	render_current_ui(state->active_ui);
}

void draw_game(gamestate* state)
{
	map_room_t* active_room = state->active_room;

	for (int i = 0; i < 32; i++)
	{
		if (active_room->entities[i] == NULL)
		{
			continue;
		}
		double* pos = active_room->entities[i]->pos;
		DrawCircle(PAD + (int)pos[0], PAD + (int)pos[1], (f32)get_size(active_room->entities[i]),
				   get_color(active_room->entities[i]));
	}
}
