#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "entity.h"
#include "graphics.h"

char* projectile_name = "The Hero of Our Story";

void entity_projectile_interact(entity_t* proj, entity_t* other, map_room_t* room)
{
	(void)room;

	if (other == NULL)
	{
		if (proj->pos.x < 0 || proj->pos.x > SCREEN_WD - PAD * 2 || proj->pos.y < 0 ||
			proj->pos.y > SCREEN_HT - PAD * 2)
		{
			proj->hp = 0;
		}

		return;
	}

	if (other->type != E_MONSTER || other->hp <= 0)
	{
		return;
	}

	f32 dx = proj->pos.x - other->pos.x;
	f32 dy = proj->pos.y - other->pos.y;

	f32 distSq = dx * dx + dy * dy;

	if (distSq <= 24.0f * 24.0f)
	{
		other->hp -= 10;
		proj->hp = 0;
	}
}
