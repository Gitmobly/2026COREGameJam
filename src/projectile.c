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
        if (IsOutOfBounds(proj->pos, proj->radius_visible)) {
            proj->hp = 0;
        }
		return;
	}

	if (other->type != E_MONSTER || other->hp <= 0)
	{
		return;
	}

    float distance = Vector2Distance(proj->pos, other->pos);
    if (distance > (proj->radius_damage_deal + other->radius_damage_take)) { return; }

	other->hp -= 10;
	proj->hp = 0;
	b32 fatal = other->hp <= 0;

    Vector2 dir = Vector2Normalize(proj->vel);
    fx_emit_monster_hit(&room->fx, other->pos, dir, fatal);

	if (!fatal)
	{
		other->knockback.x		   = dir.x * 300.0f;
		other->knockback.y		   = dir.y * 300.0f;
        other->knockbackDragMultiplier = 10,
		other->hit_stun_remaining  = 0.10f;
		other->hit_flash_remaining = MONSTER_HIT_FLASH_TIME;
	}

    if (fatal) {
        // TODO: queue soul spawning
    }
}
