#ifndef MONSTER_H
#define MONSTER_H

#include "utils.h"

typedef struct entity_t	   entity_t;
typedef struct app_state_t app_state_t;

entity_t entity_init_monster(void);
entity_t entity_init_sniper(void);
void	 entity_monster_interact(entity_t* mon, entity_t* other, app_state_t* app, f32 dt);

void	 entity_sniper_interact(entity_t* mon, entity_t* other, app_state_t* app, f32 dt);

#endif
