#pragma once

#include "utils.h"
#include "ui.h"

typedef struct
{
	s32 move[2];
	s32 fire[2];
	b32 action;
} input_state_t;

input_state_t input_update_entity(void);

void input_update_ui(ui_t* ui);
