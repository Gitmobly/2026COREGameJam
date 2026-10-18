#pragma once

#include "../map/map.h"

int SCREEN_WD, SCREEN_HT;

void graphics_init(void);
void graphics_loop(map_room_t* active_room);
