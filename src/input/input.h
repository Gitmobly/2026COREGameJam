#pragma once

#include <stdbool.h>

typedef struct {
  int move[2];
  int fire[2];
  bool action;
} input_state_t;

input_state_t input_state;

input_state_t input_update(void);
