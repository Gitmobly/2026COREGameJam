#ifndef MAIN_H
#define MAIN_H

#include "input.h"

typedef struct game_state_t game_state_t;
typedef struct ui_state_t	ui_state_t;

typedef struct app_state_t
{
	input_state_t* input_state;
	ui_state_t*	   ui_state;
	game_state_t*  game_state;
} app_state_t;

#endif
