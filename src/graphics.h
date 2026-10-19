#pragma once

#include "entity.h"
#include "map.h"
#include "gamestate.h"

#define SCREEN_WD 800.0f
#define SCREEN_HT 600.0f
#define CENTER_X (SCREEN_WD / 2.0f)
#define CENTER_Y (SCREEN_HT / 2.0f)
#define COMP_CENTER_X(wd) (CENTER_X - (wd / 2.0f))
#define COMP_CENTER_Y(ht) (CENTER_Y - (ht / 2.0f))

#define PAD		  32

void graphics_init(void);
void graphics_loop(gamestate* state);
void draw_start_ui(gamestate* state);
void draw_combat_ui(gamestate* state, entity_t* entity);
void draw_game(gamestate* state);
