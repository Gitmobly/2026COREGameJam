#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "../map/map.h"

int SCREEN_WD = 800, SCREEN_HT = 600, PAD = 32;

Color get_color(entity_t* entity) {
  return strcmp(entity->type, "monster") == 0 ? RED : SKYBLUE;
}

int get_size(entity_t* entity) {
  return strcmp(entity->type, "projectile") == 0 ? 8 : 16;
}

void graphics_init(void) {
  InitWindow(SCREEN_WD, SCREEN_HT, "2026 Core Game Jam");
  SetTargetFPS(60);
}

void graphics_loop(map_room_t* active_room) {
  BeginDrawing();
  ClearBackground(DARKGRAY);

  DrawRectangle(PAD, PAD, SCREEN_WD - (2 * PAD), SCREEN_HT - (2 * PAD), BLACK);
  
  for(int i = 0; i < 32; i++) {
    if(active_room->entities[i] == NULL) continue;
    double *pos = active_room->entities[i]->pos;
    DrawCircle(PAD + (int)pos[0], PAD + (int)pos[1], get_size(active_room->entities[i]), get_color(active_room->entities[i]));
  }

  EndDrawing();
}
