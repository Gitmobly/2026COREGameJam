#include "ui.h"
#include "vendor/raylib/raylib.h"
#include "input.h"
#include "main.h"

void input_update_entities(input_state_t* prev)
{
	prev->move.x = (f32)IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	prev->move.y = (f32)IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		prev->fire = 1;
	}
	else
	{
		prev->fire = 0;
	}

	prev->action = IsKeyPressed(KEY_SPACE);
}

void input_update_ui(app_state_t* app)
{
	ui_t* ui = app->ui_state->current_ui;

	if (ui->screen == NO_OWNER)
	{
		return;
	}
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		Vector2 mouse_pos = GetMousePosition();
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			if (ui->buttons[i].owner == NO_OWNER)
			{
				continue;
			}
			button_t b = ui->buttons[i];
			if (b.button_muted)
			{
				continue;
			}

			if (!CheckCollisionPointRec(mouse_pos, b.rect))
			{
				continue;
			}
			b.bg.r		   = MUTE(b.bg.r);
			b.bg.g		   = MUTE(b.bg.g);
			b.bg.b		   = MUTE(b.bg.b);
			b.button_muted = true;
			ui->buttons[i] = b;
		}
	}

	if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
	{
		Vector2 mouse_pos = GetMousePosition();
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			if (ui->buttons[i].owner == NO_OWNER)
			{
				continue;
			}
			button_t b = ui->buttons[i];
			if (!b.button_muted)
			{
				continue;
			}

			if (!CheckCollisionPointRec(mouse_pos, b.rect))
			{
				continue;
			}
			b.bg.r		   = UNMUTE(b.bg.r);
			b.bg.g		   = UNMUTE(b.bg.g);
			b.bg.b		   = UNMUTE(b.bg.b);
			b.button_muted = false;
			ui->buttons[i] = b;
			b.click(app);
			return;
		}
	}
}
