#include "gamestate.h"
#include "ui.h"
#include "vendor/raylib/raylib.h"
#include "input.h"


input_state_t input_update_entity(void)
{
	input_state_t state;
	state.move[0] = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	state.move[1] = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

	state.fire[0] = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
	state.fire[1] = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);

	state.action = IsKeyPressed(KEY_SPACE);
	return state;
}


void input_update_ui(gamestate* state) {
	if(state->ui_state == NO_OWNER) return;
	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
		Vector2 mouse_pos = GetMousePosition();
		ui_t* ui = state->active_ui;
		for (int i = 0; i < MAX_BUTTONS; i++) {
			if(ui->buttons[i].owner == NO_OWNER)continue;
			button_t b = ui->buttons[i];
			if(b.button_muted) continue;

			if(!CheckCollisionPointRec(mouse_pos, b.rect)) continue;
			b.bg.r = MUTE(b.bg.r);
			b.bg.g = MUTE(b.bg.g);
			b.bg.b = MUTE(b.bg.b);
			b.button_muted = true;
			ui->buttons[i] = b;
		}
	}

	if(IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
		Vector2 mouse_pos = GetMousePosition();
		ui_t* ui = state->active_ui;
		for (int i = 0; i < MAX_BUTTONS; i++) {
			if(ui->buttons[i].owner == NO_OWNER)continue;
			button_t b = ui->buttons[i];
			if(!b.button_muted) continue;

			if(!CheckCollisionPointRec(mouse_pos, b.rect)) continue;
			b.bg.r = UNMUTE(b.bg.r);
			b.bg.g = UNMUTE(b.bg.g);
			b.bg.b = UNMUTE(b.bg.b);
			b.button_muted = false;
			b.click(state);
			ui->buttons[i] = b;
		}
	}
}
