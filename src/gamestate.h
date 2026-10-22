#ifndef GAMESTATE
#define GAMESTATE

#include "entity.h"
#include <stdbool.h>

typedef struct ui_t ui_t;

// Current UI
// just adding a note *We could remove this just added because might need it in future*
typedef enum {
  NO_OWNER = -1,
  START_UI = 0,
  GAME = 1,
  COMBAT_UI = 2,
  OPTIONS_UI = 3,
} UI_STATE;



// game values/flags
typedef struct gamestate {
  /*
    Flag for ui render switching
    PS: If switching from start to game should init game first or switching from game to
    another ui should save game state
  */ 
  UI_STATE ui_state;

  /* Current Active Room/lvl */
  map_room_t* active_room; 
  struct ui_t* active_ui;

} gamestate; 





#endif 
