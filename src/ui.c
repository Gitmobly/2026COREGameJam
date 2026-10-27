#include "ui.h"
#include "arena.h"
#include "graphics.h"
#include "game.h"
#include "entity.h"
#include "main.h"
#include "actions.h"
#include "vendor/raylib/raylib.h"
#include "vendor/raylib/raymath.h"

// Should move this to a ui manager or something but for the sake of time just keeping them here
ui_t game_ui;
ui_t options_ui;
ui_t combat_ui;

button_t create_button(unsigned int x, unsigned int y, int w, int h, Color bg, Color fg, const char* text,
					   void (*click)(app_state_t*))
{
	button_t button = {
		.x			  = x,
		.y			  = y,
		.w			  = w,
		.h			  = h,
		.text		  = text,
		.click		  = click,
		.bg			  = bg,
		.fg			  = fg,
		.owner		  = START_UI,
		.font		  = GetFontDefault(),
		.font_size	  = 40.0,
		.button_muted = false,
	};

	reframe_button(&button);

	return button;
}

void reframe_button(button_t* button)
{
	Vector2 textSize = MeasureTextEx(button->font, button->text, button->font_size, button->font_size * .1f);

	Vector2 buttonSize = (Vector2){
		button->w + textSize.x,
		button->h + textSize.y,
	};

	Vector2 axisPos = calculate_axis_pos(button->x, button->y, buttonSize);

	Vector2 textPos = (Vector2){
		axisPos.x + Lerp(0.0f, buttonSize.x - textSize.x, (float)0.5),
		axisPos.y + Lerp(0.0f, buttonSize.y - textSize.y, (float)0.5),
	};

	Rectangle rect = {axisPos.x, axisPos.y, buttonSize.x, buttonSize.y};

	button->rect	= rect;
	button->textPos = textPos;
	button->x		= (int)axisPos.x;
	button->y		= (int)axisPos.y;
}

label_t create_label(unsigned int x, unsigned int y, const char* text, Color fg, float fontSize, Font font)
{
	label_t label = {
		.x		   = x,
		.y		   = y,
		.fg		   = fg,
		.font_size = fontSize,
		.font	   = font,
		.text	   = text,
	};

	update_label(&label);

	return label;
}

void update_label(label_t* label)
{
	Vector2 textSize = MeasureTextEx(label->font, label->text, label->font_size, label->font_size * .1f);

	Vector2 axisPos = calculate_axis_pos(label->x, label->y, textSize);
	label->x		= (s32)axisPos.x;
	label->y		= (s32)axisPos.y;
}

image_t create_image(Rectangle rect, Rectangle texture_pos, Texture2D texture, unsigned int rot)
{
	image_t image = {
		.pos		 = rect,
		.texture_pos = texture_pos,
		.rot		 = rot,
		.texture	 = texture,
	};

	Vector2 axisPos = calculate_axis_pos_rec(rect);

	image.pos.x = axisPos.x;
	image.pos.y = axisPos.y;

	return image;
}

Vector2 calculate_axis_pos(unsigned int x, unsigned int y, Vector2 widthHeight)
{
	Vector2		 ret = {(float)x, (float)y};
	unsigned int xv	 = x;
	unsigned int yv	 = y;

	xv &= ~PAD_MASK;
	yv &= ~PAD_MASK;

	switch (xv)
	{
		case CENTER_AXIS:
			ret.x = (float)COMP_CENTER_X(widthHeight.x);
			break;
		case POS_AXIS:
			// padding
			xv	  = (x & PAD_MASK);
			ret.x = (float)(SCREEN_WD - (widthHeight.x + xv));
			break;
		case NEG_AXIS:
			// padding
			xv	  = (x & PAD_MASK);
			ret.x = (float)(0 + xv);
			break;
		default:
			break;
	}
	switch (yv)
	{
		case CENTER_AXIS:
			ret.y = COMP_CENTER_Y(widthHeight.y);
			break;
		case POS_AXIS:
			// padding
			yv	  = (y & PAD_MASK);
			ret.y = (float)(0 + yv);
			break;
		case NEG_AXIS:
			// padding
			yv	  = (y & PAD_MASK);
			ret.y = (float)(SCREEN_HT - (widthHeight.y + yv));
			break;
		default:
			break;
	}

	return ret;
}

Vector2 calculate_axis_pos_rec(Rectangle rect)
{
	return calculate_axis_pos((unsigned int)rect.x, (unsigned int)rect.y, (Vector2){rect.width, rect.height});
}

void render_ui(ui_t* ui)
{
	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		button_t b = ui->buttons[i];
		if (b.owner == NO_OWNER)
		{
			continue;
		}
		draw_button(&b);
	}
	for (int i = 0; i < MAX_LABELS; i++)
	{
		label_t l = ui->labels[i];
		if (l.owner == NO_OWNER)
		{
			continue;
		}
		draw_label(&l);
	}
	for (int i = 0; i < MAX_IMAGES; i++)
	{
		image_t im = ui->images[i];
		if (im.owner == NO_OWNER)
		{
			continue;
		}
		draw_image(&im);
	}
}

void unload_ui(ui_t* ui)
{
	for (int i = 0; i < MAX_IMAGES; i++)
	{
		image_t img = ui->images[i];
		if (img.owner == NO_OWNER)
		{
			continue;
		}
		UnloadTexture(img.texture);
	}
}

void init_ui_components(ui_t* ui)
{
	// set all button owner to -1 (no owner)
	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		ui->buttons[i].owner = NO_OWNER;
	}
	for (int i = 0; i < MAX_LABELS; i++)
	{
		ui->labels[i].owner = NO_OWNER;
	}
	for (int i = 0; i < MAX_IMAGES; i++)
	{
		ui->images[i].owner = NO_OWNER;
	}
}

void ui_state_init(ui_state_t* ui_state, allocators_t* allocators)
{
	ui_state->allocators = allocators;
	ui_state->current_ui = build_start_ui(allocators);
}

ui_t* build_start_ui(allocators_t* allocators)
{
	ui_t* ui   = arena_push_struct(allocators->ui, ui_t);
	ui->screen = START_UI;

	init_ui_components(ui);

	button_t start_button =
		create_button(PAD_COMP(NEG_AXIS, 38), SCREEN_HT - 85, 10, 5, RAYWHITE, BLACK, "START GAME", start_click_start);
	button_t options_button =
		create_button(PAD_COMP(POS_AXIS, 38), SCREEN_HT - 85, 10, 5, RAYWHITE, BLACK, "OPTIONS", options_click_start);

	start_button.owner	 = START_UI;
	options_button.owner = START_UI;

	label_t label = create_label(CENTER_AXIS, CENTER_AXIS, "2026 GameJam", RAYWHITE, 32.0f, GetFontDefault());
	label.owner	  = START_UI;

	Texture2D texture  = LoadTexture("./assets/death_start_game.png");
	Rectangle pos	   = {CENTER_AXIS, 100, 150, 150};
	Rectangle text_pos = {0.0f, 0.0f, (f32)texture.width, (f32)texture.height};

	image_t image = create_image(pos, text_pos, texture, 0);
	image.owner	  = START_UI;

	ui->buttons[0] = start_button;
	ui->buttons[1] = options_button;
	ui->labels[0]  = label;
	ui->images[0]  = image;

	return ui;
}

ui_t* init_game_ui(void)
{
	ui_t ui = {
		.screen = GAME,
	};

	init_ui_components(&ui);
	game_ui = ui;
	return &game_ui;
}

ui_t* init_options_ui(void)
{
	ui_t ui = {
		.screen = OPTIONS_UI,
	};

	init_ui_components(&ui);
	button_t back_button =
		create_button(PAD_COMP(NEG_AXIS, 38), SCREEN_HT - 85, 10, 5, RAYWHITE, BLACK, "BACK", back_click_options);
	back_button.owner = OPTIONS_UI;

	ui.buttons[0] = back_button;

	options_ui = ui;
	return &options_ui;
}

void start_click_start(app_state_t* app)
{
	ui_state_t* ui = app->ui_state;

	unload_ui(ui->current_ui);
	ui->current_ui = init_game_ui();
}

// Not impl yet
void options_click_start(app_state_t* state)
{
	ui_state_t* ui_state = state->ui_state;

	unload_ui(ui_state->current_ui);
	ui_state->current_ui = init_options_ui();
}

void back_click_options(app_state_t* state)
{
	state->ui_state->current_ui->screen = START_UI;
	// blat textures blat unload ui too
	unload_ui(state->ui_state->current_ui);
	state->ui_state->current_ui = build_start_ui(state->ui_state->allocators);
}

ui_t* init_comabat_ui(entity_t* entity, app_state_t* state)
{
	ui_t ui = {
		.screen = COMBAT_UI,
	};

	init_ui_components(&ui);
	(void)entity;

	button_t attack =
		create_button(PAD_COMP(NEG_AXIS, 38), SCREEN_HT - 85, 10, 5, GRAY, RED, "Attacks", attack_click_combat);
	attack.font_size = 22;
	attack.owner	 = COMBAT_UI;
	reframe_button(&attack);

	button_t skills =
		create_button(PAD_COMP(NEG_AXIS, 38), SCREEN_HT - 120, 10, 5, GRAY, BLUE, "Skills", skills_click_combat);
	skills.font_size = 22;
	skills.owner	 = COMBAT_UI;
	reframe_button(&skills);

	ui.buttons[0] = attack;
	ui.buttons[1] = skills;

	// TODO event that updates this when player takes dmg
	const char* player_hp_str = TextFormat("PLAYER'S HP : %d", state->game_state->current_level.player_entity->hp);
	label_t		player_hp =
		create_label(PAD_COMP(POS_AXIS, 50), SCREEN_HT - 120, player_hp_str, BLUE, 22, GetFontDefault());
	const char* entity_hp_str = TextFormat("%s HP : %d", "ENEMY'S", state->game_state->current_level.player_entity->hp);
	label_t		ent_hp = create_label(PAD_COMP(POS_AXIS, 50), SCREEN_HT - 85, entity_hp_str, RED, 22, GetFontDefault());

	ui.labels[0] = player_hp;
	ui.labels[1] = ent_hp;

	combat_ui = ui;
	return &combat_ui;
}

void attack_click_combat(app_state_t* state)
{
	if (state->ui_state->current_ui->screen != COMBAT_UI)
	{
		return;
	}
	state->ui_state->current_ui->buttons[MAX_BUTTONS - 1].owner = NO_OWNER;
	state->ui_state->current_ui->buttons[MAX_BUTTONS - 2].owner = NO_OWNER;

	button_t punch =
		create_button(PAD_COMP(NEG_AXIS, 200), SCREEN_HT - 120, 10, 5, GRAY, RED, "Punch", combat_player_punch);
	punch.font_size = 22;
	punch.owner		= COMBAT_UI;
	reframe_button(&punch);

	button_t kick =
		create_button(PAD_COMP(NEG_AXIS, 200), SCREEN_HT - 85, 10, 5, GRAY, RED, "Kick", combat_player_kick);
	kick.font_size = 22;
	kick.owner	   = COMBAT_UI;
	reframe_button(&kick);

	state->ui_state->current_ui->buttons[MAX_BUTTONS - 1] = punch;
	state->ui_state->current_ui->buttons[MAX_BUTTONS - 2] = kick;
}

void skills_click_combat(app_state_t* state)
{
	if (state->ui_state->current_ui->screen != COMBAT_UI)
	{
		return;
	}
	state->ui_state->current_ui->buttons[MAX_BUTTONS - 1].owner = NO_OWNER;
	state->ui_state->current_ui->buttons[MAX_BUTTONS - 2].owner = NO_OWNER;

	button_t sm_heal =
		create_button(PAD_COMP(NEG_AXIS, 200), SCREEN_HT - 85, 10, 5, GRAY, BLUE, "Small Heal", skill_player_smheal);
	sm_heal.font_size = 22;
	sm_heal.owner	  = COMBAT_UI;
	reframe_button(&sm_heal);
	state->ui_state->current_ui->buttons[MAX_BUTTONS - 1] = sm_heal;
}
