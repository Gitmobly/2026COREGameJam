#pragma once

#define SCREEN_WD		  800
#define SCREEN_HT		  600
#define CENTER_X		  (SCREEN_WD / 2.0f)
#define CENTER_Y		  (SCREEN_HT / 2.0f)
#define COMP_CENTER_X(wd) (CENTER_X - (wd / 2.0f))
#define COMP_CENTER_Y(ht) (CENTER_Y - (ht / 2.0f))

#define PAD 32

typedef struct app_state_t	app_state_t;
typedef struct game_state_t game_state_t;
typedef struct ui_state_t	ui_state_t;

void render(app_state_t* app);
void draw_game(game_state_t* state);
void draw_combat_ui(ui_state_t* state);
