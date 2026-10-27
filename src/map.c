#include "map.h"
#include "entity.h"

map_world_t map_world;

b32 map_add_entity(entity_t* entity, map_room_t* room)
{
	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* slot = &room->entities[i];

		if (slot->type == E_PROJECTILE && slot->hp <= 0)
		{
			*slot = *entity;
			return 1;
		}
	}

	if (room->entity_count >= MAP_ENTITY_MAX)
	{
		return 0;
	}

	room->entities[room->entity_count++] = *entity;
	return 1;
}
