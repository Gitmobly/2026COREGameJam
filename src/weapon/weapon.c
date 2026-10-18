#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../weapon/weapon.h"
#include "../map/map.h"

char* str_projectile = "projectile";

weapon_t *weapon_init_base(void) {
  weapon_t *base = malloc(sizeof(weapon_t));
  base->speed = 8;
  base->fire_rate = 1;
  return base;
}

void weapon_fire(entity_t *ent, map_room_t *room, int dir[2]) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  double now = (double) ts.tv_sec + (double) ts.tv_nsec / 1000000000;
  if(now < ent->weapon->fire_rate + ent->weapon->fire_last) return;
  ent->weapon->fire_last = now;

  entity_t *projectile = malloc(sizeof(entity_t));
  projectile->hp = INT_MAX;
  projectile->pos[0] = ent->pos[0];
  projectile->pos[1] = ent->pos[1];
  projectile->type = str_projectile;
  projectile->name = str_projectile;
  projectile->vel[0] = dir[0] * ent->weapon->speed;
  projectile->vel[1] = dir[1] * ent->weapon->speed;
  
  map_add_entity(projectile, room);
  // free(projectile);
}
