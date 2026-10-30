#include "entity.h"
#include "ui.h"
#include "map.h"
#include "main.h"
#include "game.h"
#include "vendor/raylib/raylib.h"
#include <math.h>

#include "graphics.h"

Color get_color(entity_t* entity)
{
	return entity->type == E_MONSTER ? RED : SKYBLUE;
}

int get_size(entity_t* entity)
{
	return entity->type == E_PROJECTILE ? 8 : 16;
}

internal Camera2D get_game_camera(game_state_t* game)
{
	Camera2D camera = {0};

	camera.zoom = 1.0f;

	if (game->wall_shake_remaining > 0.0f)
	{
		f32 t = game->wall_shake_remaining / 0.15f;

		f32 shake_intensty = 5.0f;
		f32 amplitude	   = shake_intensty * t * t;

		f32 time = (f32)GetTime();

		camera.offset.x = sinf(time * 90.0f) * amplitude;
		camera.offset.y = cosf(time * 75.0f) * amplitude;
	}

	return camera;
}

internal void draw_crosshair(const weapon_t* weapon)
{
	Vector2 mouse = GetMousePosition();

	f64 now	 = GetTime();
	f32 time = (f32)now;

	const f32 step			= (2.0f * PI) / 6.0f;
	const f32 spin_duration = 0.12f;

	f32 angle  = time * 0.18f;
	f32 radius = 12.0f + sinf(time * 3.0f) * 0.7f;

	if (weapon->shots_fired > 0)
	{
		f32 t = (f32)((now - weapon->last_shot_time) / spin_duration);

		if (t < 0.0f)
		{
			t = 0.0f;
		}
		if (t > 1.0f)
		{
			t = 1.0f;
		}

		f32 remaining = 1.0f - t;
		f32 ease	  = 1.0f - remaining * remaining * remaining;
		angle += ((f32)(weapon->shots_fired - 1) + ease) * step;
		radius += remaining * 1.5f;
	}

	for (u32 i = 0; i < 6; i++)
	{
		f32 a = angle + (f32)i * step;

		Vector2 pos = {
			mouse.x + cosf(a) * radius,
			mouse.y + sinf(a) * radius,
		};

		Color color = i < weapon->ammo ? RAYWHITE : GRAY;

		DrawCircleV(pos, 3.0f, color);
	}

	DrawCircleV(mouse, 2.0f, RED);
}

void render(app_state_t* app)
{
	ui_state_t* ui = app->ui_state;

	BeginDrawing();
	ClearBackground(DARKGRAY);

	DrawRectangle(PAD, PAD, SCREEN_WD - 2 * PAD, SCREEN_HT - 2 * PAD, BLACK);

	b32 render_revolver = 0;

	switch (ui->current_ui->screen)
	{
		case GAME:
			render_revolver = 1;
			Camera2D camera = get_game_camera(app->game_state);

			BeginMode2D(camera);

			DrawRectangle(PAD, PAD, SCREEN_WD - PAD * 2, SCREEN_HT - PAD * 2, BLACK);

			draw_game(app->game_state);

			EndMode2D();
			break;

		case COMBAT_UI:
			draw_combat_ui(ui);
			break;

		default:
			render_ui(ui->current_ui);
			break;
	}

	if (!render_revolver)
	{
		Vector2 mouse = GetMousePosition();

		DrawLineEx((Vector2){mouse.x - 8, mouse.y}, (Vector2){mouse.x + 8, mouse.y}, 2.0f, RED);
		DrawLineEx((Vector2){mouse.x, mouse.y - 8}, (Vector2){mouse.x, mouse.y + 8}, 2.0f, RED);
	}
	else
	{
		entity_t* player = app->game_state->current_level.player_entity;

		draw_crosshair(&player->weapon);
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
