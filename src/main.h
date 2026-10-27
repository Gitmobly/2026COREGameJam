#ifndef MAIN_H
#define MAIN_H

typedef struct game_state_t game_state_t;
typedef struct ui_state_t	ui_state_t;

typedef struct app_state_t
{
	ui_state_t*	  ui_state;
	game_state_t* game_state;
} app_state_t;

#endif
