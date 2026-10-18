#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "entity.h"

char *projectile_name = "The Hero of Our Story", *projectile_type = "projectile";

void entity_projectile_interact(entity_t* proj, entity_t* other, map_room_t* room)
{
	(void)room;

	if (other == NULL)
	{
		if (proj->pos[0] < 0 || proj->pos[0] > 1000 || proj->pos[1] < 0 || proj->pos[1] > 1000)
		{
			proj->hp = 0;
		}
		return;
	}

	if (other->type == E_PLAYER)
	{
		double dx	= proj->pos[0] - other->pos[0];
		double dy	= proj->pos[1] - other->pos[1];
		double dist = sqrt(dx * dx + dy * dy);
		if (dist > 24)
		{
			return;
		}
		other->hp -= 10;
		proj->hp = 0;
	}
}
