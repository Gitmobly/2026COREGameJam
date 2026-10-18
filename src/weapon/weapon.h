#pragma once

#include "../entity/entity.h"
#include "../map/map.h"

typedef struct weapon_t {
  double fire_rate;
  double damage;
  double speed;

  double fire_last;
} weapon_t;

void weapon_fire(entity_t *ent, map_room_t *room, int dir[2]);

weapon_t *weapon_init_base(void);
