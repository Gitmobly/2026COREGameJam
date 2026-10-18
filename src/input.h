#pragma once

#include "utils.h"

typedef struct
{
	s32 move[2];
	s32 fire[2];
	b32 action;
} input_state_t;

input_state_t input_update(void);
