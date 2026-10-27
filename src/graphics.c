#include "entity.h"
#include "ui.h"
#include "map.h"
#include "main.h"
#include "game.h"
#include "vendor/raylib/raylib.h"

#include "graphics.h"

Color get_color(entity_t* entity)
{
	return entity->type == E_MONSTER ? RED : SKYBLUE;
}

int get_size(entity_t* entity)
{
	return entity->type == E_PROJECTILE ? 8 : 16;
}

void render(app_state_t* app)
{
	ui_state_t* ui = app->ui_state;

	BeginDrawing();
	ClearBackground(DARKGRAY);

	DrawRectangle(PAD, PAD, SCREEN_WD - 2 * PAD, SCREEN_HT - 2 * PAD, BLACK);

	switch (ui->current_ui->screen)
	{
		case GAME:
			draw_game(app->game_state);
			break;

		case COMBAT_UI:
			draw_combat_ui(ui);
			break;

		default:
			render_ui(ui->current_ui);
			break;
	}

	EndDrawing();
}

void draw_combat_ui(ui_state_t* state)
{
	// todo render combat ui
	DrawRectangle(PAD, SCREEN_HT - 100 - PAD, SCREEN_WD - (PAD * 2), 100, LIGHTGRAY);
	DrawRectangle(PAD + 130, SCREEN_HT - 100 - PAD, 10, 100, BLACK);
	DrawRectangle(PAD + 500, SCREEN_HT - 100 - PAD, 10, 100, BLACK);
	render_ui(state->current_ui);
}

void draw_game(game_state_t* state)
{
	map_room_t* room = state->current_level.active_room;

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		DrawCircle(PAD + (int)ent->pos.x, PAD + (int)ent->pos.y, (f32)get_size(ent), get_color(ent));
	}
}
