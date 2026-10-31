#pragma once

#include "utils.h"
#include "ui.h"

typedef struct app_state_t app_state_t;

typedef struct
{
	Vector2 move;
	b32		fire;
	b32		action;
	b32		mouse_down;
} input_state_t;

void input_update_entities(input_state_t* prev);

void input_update_ui(app_state_t* app);
