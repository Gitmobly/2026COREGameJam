#include <raylib.h>
#include <math.h>
#include <stdio.h>

#include "graphics/graphics.h"
#include "map/map.h"
#include "entity/entity.h"
#include "input/input.h"
#include "weapon/weapon.h"

int main(void) {
  graphics_init();
  
  map_world_t *world = map_init_world();
  entity_t* monster = entity_init_monster();
  entity_t* player = entity_player_init();
  player->weapon = weapon_init_base();

  map_room_t *room = &world->rooms[8][8];
  room->entities[0] = player;
  room->entities[1] = monster;

  while (!WindowShouldClose()) {
    graphics_loop(room);
    entity_interactions(room);
  }

  CloseWindow();
}
