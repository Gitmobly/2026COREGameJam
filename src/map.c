#include "map.h"
#include "entity.h"
#include "monster.h"
#include "graphics.h"
#include <string.h>
#include <raymath.h>

b32 map_add_entity(entity_t* entity, map_room_t* room)
{
	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* slot = &room->entities[i];

		if (/*slot->type == E_PROJECTILE &&*/ slot->hp <= 0)
		{
			*slot = *entity;
			return true;
		}
	}

	if (room->entity_count >= MAP_ENTITY_MAX)
	{
		return false;
	}

	room->entities[room->entity_count++] = *entity;
	return true;
}

void map_generate_room(map_room_t* room, const entity_t* player, u32 depth)
{
	memset(room, 0, sizeof(*room));

	room->entities[0]  = *player;
	room->entity_count = 1;

	u32 monster_count = depth < 12 ? depth : 12;
	for (u32 i = 0; i < monster_count; i++)
	{
		entity_t monster = entity_init_monster();

		for (u32 attempt = 0; attempt < 64; attempt++)
		{
			monster.pos = (Vector2){
				(f32)GetRandomValue(220, SCREEN_WD - BOUNDARY_WIDTH * 2 - 50),
				(f32)GetRandomValue(60,  SCREEN_HT - BOUNDARY_WIDTH * 2 - 60),
			};

			b32 overlap = 0;
			for (u32 j = 0; j < room->entity_count; j++)
			{
				entity_t* other = &room->entities[j];
                if (Vector2DistanceSqr(other->pos, monster.pos) < (48 * 48))
				{
					overlap = 1;
					break;
				}
			}

			if (!overlap)
			{
				map_add_entity(&monster, room);
				break;
			}
		}
	}
}
