#include "entity.h"
#include "ui.h"
#include "map.h"
#include "main.h"
#include "game.h"
#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "graphics.h"

Color get_color(entity_t* entity)
{
    switch (entity->type) {
    default:            return GRAY;

    case E_MONSTER:     {
                            f32 hp = (float)entity->hp;
                            f32 max_hp = 50;

                            Color color_full = RED;
                            Color color_dead = { 120, 90, 90, 255 };

                            return ColorLerp(color_dead, color_full, hp / max_hp);
                        }
    case E_PLAYER:      return BLUE;
    case E_SOUL:        return YELLOW;
    case E_TURRET:      return DARKGRAY;
    case E_PROJECTILE:  return entity->isEnemyProjectile ? RED : BLUE;
    }
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

	if (game->hit_shake_remaining > 0.0f)
	{
		f32 t		  = game->hit_shake_remaining / 0.09f;
		f32 amplitude = 1.4f * t * t;
		f32 time	  = (f32)GetTime();

		camera.offset.x += sinf(time * 153.0f) * amplitude;
		camera.offset.y += cosf(time * 127.0f) * amplitude;
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
        t = Clamp(t, 0, 1);

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

	DrawCircleV(mouse, 2.0f, PURPLE);
}

internal void draw_room_walls(const map_room_t* room)
{
	const f32 left		= (f32)BOUNDARY_WIDTH;
	const f32 right		= (f32)(SCREEN_WD - BOUNDARY_WIDTH);
	const f32 top		= (f32)BOUNDARY_WIDTH;
	const f32 bottom	= (f32)(SCREEN_HT - BOUNDARY_WIDTH);
	const f32 center_y	= SCREEN_HT * 0.5f;
	const f32 gap		= MAP_EXIT_HALF_HEIGHT;
	const f32 thickness = 4.0f;

	DrawLineEx(v2(left, top), v2(right, top), thickness, LIGHTGRAY);
	DrawLineEx(v2(left, bottom), v2(right, bottom), thickness, LIGHTGRAY);
	DrawLineEx(v2(left, top), v2(left, bottom), thickness, LIGHTGRAY);

	if (!room->exit_open)
	{
		DrawLineEx(v2(right, top), v2(right, bottom), thickness, LIGHTGRAY);
		return;
	}

	DrawLineEx(v2(right, top), v2(right, center_y - gap), thickness, LIGHTGRAY);
	DrawLineEx(v2(right, center_y + gap), v2(right, bottom), thickness, LIGHTGRAY);

	f32 elapsed = (f32)GetTime() - room->wall_break_time;
	if (elapsed >= 0.0f && elapsed < 0.4f)
	{
		f32 t = elapsed / 0.4f;
		for (int i = 0; i < 12; i++)
		{
			f32 x = right + elapsed * (35.0f + (f32)(i % 4) * 12.0f);
			f32 y = center_y - gap + 5.0f + (f32)i * 8.0f;
			y += ((i & 1) ? 1.0f : -1.0f) * elapsed * 35.0f;
			DrawRectangleV(v2(x, y), v2(5, 5), Fade(LIGHTGRAY, 1.0f - t));
		}
	}
}

void render(app_state_t* app)
{
	ui_state_t* ui = app->ui_state;

	BeginDrawing();
	ClearBackground(DARKGRAY);

	DrawRectangle(BOUNDARY_WIDTH, BOUNDARY_WIDTH, GAME_WD, GAME_HT, BLACK);

	b32 render_revolver = false;
    b32 render_crosshair = true;

	switch (ui->current_ui->screen)
	{
		case GAME:
			render_revolver = true;

            if (app->game_state->soul_active) {
                render_crosshair = false;
            }

			Camera2D camera = get_game_camera(app->game_state);

			BeginMode2D(camera);

			DrawRectangle(BOUNDARY_WIDTH, BOUNDARY_WIDTH, GAME_WD, GAME_HT, BLACK);

			draw_room_walls(app->game_state->current_level.active_room);
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

    if (!render_crosshair) {}
    else if (!render_revolver)
	{
		Vector2 mouse = GetMousePosition();

		DrawLineEx(v2(mouse.x - 8, mouse.y), v2(mouse.x + 8, mouse.y), 2.0f, RED);
		DrawLineEx(v2(mouse.x, mouse.y - 8), v2(mouse.x, mouse.y + 8), 2.0f, RED);
	}
	else
	{
		entity_t* player = app->game_state->current_level.entity_player;

		draw_crosshair(&player->weapon);
	}

	EndDrawing();
}

void draw_combat_ui(ui_state_t* state)
{
	// todo render combat ui
	DrawRectangle(BOUNDARY_WIDTH, SCREEN_HT - 100 - BOUNDARY_WIDTH, SCREEN_WD - (BOUNDARY_WIDTH * 2), 100, LIGHTGRAY);
	DrawRectangle(BOUNDARY_WIDTH + 130, SCREEN_HT - 100 - BOUNDARY_WIDTH, 10, 100, BLACK);
	DrawRectangle(BOUNDARY_WIDTH + 500, SCREEN_HT - 100 - BOUNDARY_WIDTH, 10, 100, BLACK);
	render_ui(state->current_ui);
}

void draw_game(game_state_t* state)
{
	map_room_t* room = state->current_level.active_room;

	DrawText(TextFormat("ROOM %u", state->room_number), BOUNDARY_WIDTH + 12, BOUNDARY_WIDTH + 12, 18, LIGHTGRAY);

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		Vector2 pos	   = {ent->pos.x + BOUNDARY_WIDTH, ent->pos.y + BOUNDARY_WIDTH};
		f32		radius = ent->radius_visible;
		Color	color  = get_color(ent);

		if (ent->type == E_MONSTER && ent->hit_flash_remaining > 0.0f)
		{
			f32 t = ent->hit_flash_remaining / MONSTER_HIT_FLASH_TIME;
			color = RAYWHITE;
			DrawCircleLines((int)pos.x, (int)pos.y, radius + (1.0f - t) * 7.0f, Fade(ORANGE, t * 0.8f));
		}

		DrawCircleV(pos, radius, color);
	}

	fx_draw(&room->fx, v2(BOUNDARY_WIDTH, BOUNDARY_WIDTH));


    {
        f32 player_health = state->current_level.entity_player->hp;
        f32 player_maxHealth = 100;
        f32 player_health_lerp = Clamp(player_health / player_maxHealth, 0, 1);



        f32 player_health_grayLerp = state->soul_active ? 0.7 : 0;
        Color player_health_color = ColorLerp(BLUE, (Color){ 100, 100, 100, 255 }, player_health_grayLerp);


        DrawRectangle(0, SCREEN_HT - BOUNDARY_WIDTH * 0.6, SCREEN_WD * player_health_lerp, BOUNDARY_WIDTH * 0.6, player_health_color);


        // TODO: this might get bugged if soul dies (overall run-ending sequence should be reworked)

        // TODO: ideally the soul mode should somehow feel fragile, idk how to do it though (particles? shaking?)
        if (state->soul_active) {
            f32 soul_health = state->current_level.entity_soul->hp;
            f32 soul_maxHealth = 20;
            f32 soul_health_lerp = Clamp(soul_health / soul_maxHealth, 0, 1);

            Color gradientStart = YELLOW;
            gradientStart.a = 80;

            Color gradientEnd = YELLOW;
            gradientEnd.a = 0;

            DrawRectangle(0, 0, SCREEN_WD * soul_health_lerp, BOUNDARY_WIDTH * 0.6, YELLOW);

            DrawRectangleGradientV(0, 0, SCREEN_WD, SCREEN_HT * 0.6, gradientStart, gradientEnd);
        }
        else {
            Color gradientStart = BLUE;
            gradientStart.a = 0;

            Color gradientEnd = BLUE;
            gradientEnd.a = 50;

            DrawRectangleGradientV(0, SCREEN_HT * 0.4, SCREEN_WD, SCREEN_HT * 0.6, gradientStart, gradientEnd);
        }
    }
}
