#include <math.h>

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

	f32 dx		= proj->pos.x - other->pos.x;
	f32 dy		= proj->pos.y - other->pos.y;
	f32 dist_sq = dx * dx + dy * dy;

	if (dist_sq > 24.0f * 24.0f)
	{
		return;
	}

	other->hp -= 10;
	proj->hp = 0;

	f32		len_sq = proj->vel.x * proj->vel.x + proj->vel.y * proj->vel.y;
	Vector2 dir	   = {0.0f, 0.0f};
	if (len_sq > 0.0f)
	{
		f32 inv_len = 1.0f / sqrtf(len_sq);
		dir.x		= proj->vel.x * inv_len;
		dir.y		= proj->vel.y * inv_len;
	}

	b32 fatal = other->hp <= 0;
	fx_emit_monster_hit(&room->fx, other->pos, dir, fatal);

	if (!fatal)
	{
		other->knockback.x		   = dir.x * 300.0f;
		other->knockback.y		   = dir.y * 300.0f;
		other->hit_stun_remaining  = 0.10f;
		other->hit_flash_remaining = MONSTER_HIT_FLASH_TIME;
	}
}
