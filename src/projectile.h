#ifndef PROJECTILE_H
#define PROJECTILE_H

typedef struct entity_t	  entity_t;
typedef struct map_room_t map_room_t;

void entity_projectile_interact(entity_t* proj, entity_t* other, map_room_t* room);

#endif
