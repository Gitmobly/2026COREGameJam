#include <math.h>
#include <raymath.h>

#include "entity.h"
#include "map.h"
#include "projectile.h"
#include "graphics.h"

void entity_projectile_interact(entity_t* proj, entity_t* other, map_room_t* room)
{
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

    if (Vector2DistanceSqr(proj->pos, other->pos) > (24 * 24)) { return; }

	other->hp -= 10;
	proj->hp = 0;
	b32 fatal = other->hp <= 0;

    Vector2 dir = Vector2Normalize(proj->vel);
    fx_emit_monster_hit(&room->fx, other->pos, dir, fatal);

	if (!fatal)
	{
		other->knockback.x		   = dir.x * 300.0f;
		other->knockback.y		   = dir.y * 300.0f;
		other->hit_stun_remaining  = 0.10f;
		other->hit_flash_remaining = MONSTER_HIT_FLASH_TIME;
	}
}
