#pragma once

struct weapon_t;
typedef struct map_room_t map_room_t;

typedef struct entity_t {
  double pos[2];
  double vel[2];
  float speed;
  int hp;
  char* name;
  char* type;
  struct weapon_t *weapon;
} entity_t;

entity_t* entity_player_init(void);
entity_t* entity_init_monster(void);
void entity_interactions(map_room_t *room);

void entity_projectile_interact(entity_t *proj, entity_t *other, map_room_t *room);
void entity_player_interact(entity_t *player, entity_t *other, map_room_t *room);
void entity_monster_interact(entity_t *mon, entity_t *other, map_room_t *room);
