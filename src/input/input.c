#include "raylib.h"

#include "../input/input.h"
  
input_state_t input_update(void) {
  input_state_t state;
  state.move[0] = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
  state.move[1] = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

  state.fire[0] = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
  state.fire[1] = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
  
  state.action = IsKeyPressed(KEY_SPACE);
  return state;
}
