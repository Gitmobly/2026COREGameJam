#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../map/map.h"
#include "../entity/entity.h"

map_world_t map_world;

map_world_t *map_init_world(void) {
  return &map_world;
}

bool map_add_entity(entity_t *entity, map_room_t *room) {
  for(int i = 0; i < MAP_ENTITY_MAX; i++) {
    if(room->entities[i] == NULL) {
      room->entities[i] = entity;
      return true;
    }
  }
  printf("[warn] no room for %s\n", entity->name);
  return false;
}
