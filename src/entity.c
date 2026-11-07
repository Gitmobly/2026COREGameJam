#include <math.h>

#include "entity.h"

#include "game.h"
#include "monster.h"
#include "projectile.h"
#include "map.h"
#include "player.h"
#include "main.h"
#include "graphics.h"
#include "ui.h"

#include <raymath.h>

void entity_interactions(app_state_t* app)
{
	level_state_t* level = &app->game_state->current_level;
	map_room_t*	   room	 = level->active_room;
	game_state_t*  game	 = app->game_state;
	f32			   dt	 = GetFrameTime();

	fx_update(&room->fx, dt);
	game->hit_shake_remaining = ClampLo(game->hit_shake_remaining - dt, 0);

    if (level->entity_controlled->hp <= 0) {
		app->ui_state->current_ui = init_combat_ui(NULL, app);
        return;
    }

    if (game->soul_needsToBeSpawned) {
        game->soul_needsToBeSpawned = false;
        game->soul_active = true;

        entity_t *soul_entity = map_add_entity(&game->soul_entityToSpawn, room);

        level->entity_soul = soul_entity;
        level->entity_controlled = soul_entity;

        level->entity_player->vel = v2(0, 0);
        Vector2 dir = v2(GetRandomValue(-100, 100), GetRandomValue(-100, 100));
        level->entity_player->knockback = Vector2Scale(Vector2Normalize(dir), 50);
        level->entity_player->knockbackDragMultiplier = 0.5;

        TraceLog(LOG_INFO, "SPAWNED SOUL %p\n", soul_entity);
    }

    if (game->soul_needsToBeDespawned) {
        game->soul_needsToBeDespawned = false;
        game->soul_active = false;

        Vector2 direction = Vector2Subtract(level->entity_player->pos, level->entity_soul->pos);
        direction = Vector2Normalize(direction);

        level->entity_controlled = level->entity_player; 
        level->entity_player->hit_stun_remaining = 0.25;
        level->entity_player->knockback = Vector2Scale(direction, 500);
        level->entity_player->knockbackDragMultiplier = 10;
        // TODO: max health should be explicit (potentially upgradable?)
        level->entity_player->hp = Clamp(level->entity_player->hp + 2, 0, 100);

        level->entity_soul->hp = 0;
    }

    entity_player_interact(level->entity_controlled, room, app->input_state, dt);

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

		{
            ent->hit_flash_remaining = ClampLo(ent->hit_flash_remaining - dt, 0);
            ent->hit_stun_remaining  = ClampLo(ent->hit_stun_remaining  - dt, 0);
            ent->invulnerability_remaining  = ClampLo(ent->invulnerability_remaining - dt, 0);
            ent->dodge_cooldown  = ClampLo(ent->dodge_cooldown - dt, 0);
            ent->berserk_remaining  = ClampLo(ent->berserk_remaining - dt, 0);

			if (ent->hit_stun_remaining > 0)
			{
				ent->vel = (Vector2){0};
				continue;
			}

		}

        if (ent->type == E_MONSTER) {
			entity_monster_interact(ent, level->entity_controlled, app, dt);

			if (app->ui_state->current_ui->screen != GAME)
			{
				return;
			}
        }

        if (ent->type == E_SOUL) {
            f32 distance = Vector2Distance(ent->pos, level->entity_player->pos);
            if (distance < level->entity_player->radius_visible + level->entity_soul->radius_visible) {
                game->soul_needsToBeDespawned = true;
            }
        }

        if (ent->type == E_TURRET) {
            ent->reload_remaining = ClampLo(ent->reload_remaining - dt, 0);
            if (ent->reload_remaining <= 0) {
                ent->reload_remaining = 1.5;

                Vector2 dir = v2(GetRandomValue(-100, 100), GetRandomValue(-100, 100));
                dir = Vector2Normalize(dir);

                for (int i = 0; i < 4; i++) {
                    entity_t projectile = {
                        .hp	 = 1,
                        .pos = ent->pos,
                        .vel = Vector2Scale(Vector2Rotate(dir, (PI / 2) * (f32)i), 1.5),
                        .type = E_PROJECTILE,
                        .name = "projectile",

                        .isEnemyProjectile = true,

                        .projectileDamage = 10,

                        .radius_visible     = 10,
                        .radius_damage_deal = 8,
                        .radius_damage_take = 8,
                    };

                    map_add_entity(&projectile, room);
                }

            }
        }
	}

    game->wall_shake_remaining = ClampLo(game->wall_shake_remaining - dt, 0);

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* ent = &room->entities[i];

		if (ent->hp <= 0)
		{
			continue;
		}

        // TODO: arent we forgetting dt? it seems that we are, but i dont wanna touch it
        // (also need to increase speed by like 60 in that case)
		Vector2 next = {
			ent->pos.x + ent->vel.x,
			ent->pos.y + ent->vel.y,
		};

		{
			next.x += ent->knockback.x * dt;
			next.y += ent->knockback.y * dt;

            f32 radius = ent->radius_visible;
            next.x = Clamp(next.x, 0 + radius, GAME_WD - radius);
            next.y = Clamp(next.y, 0 + radius, GAME_HT - radius);

			f32 drag = ClampLo(1 - ent->knockbackDragMultiplier * dt, 0);
			ent->knockback.x *= drag;
			ent->knockback.y *= drag;
		}

		if (ent->type == E_MONSTER) {

        }

		if (ent->type == E_PLAYER)
		{
            f32 radius = ent->radius_visible;

            f32 clamp_next_x = Clamp(next.x, 0 + radius, GAME_WD - radius);
            f32 clamp_next_y = Clamp(next.y, 0 + radius, GAME_HT - radius);
            b32 hit_wall = (clamp_next_x != next.x) || (clamp_next_y != next.y);

			f32 exit_y		 = GAME_HT * 0.5f;
			f32 half_opening = MAP_EXIT_HALF_HEIGHT - radius;
			b32 inside_exit	 = next.y >= exit_y - half_opening && next.y <= exit_y + half_opening;

            // TODO: we should probably randomize which side has the exit, so that the player cant camp
            // (it's really not *that* important though)
			if (room->exit_open && inside_exit && next.x > (GAME_WD - 1.5 * radius) && !game->soul_active)
			{
				game_next_room(app);
				return;
			}

			if (hit_wall && !game->player_hitting_wall)
			{
				game->wall_shake_remaining = 0.15f;
			}

			game->player_hitting_wall = hit_wall;
            next.x = clamp_next_x;
            next.y = clamp_next_y;
		}

		ent->pos = next;
	}

	for (u32 i = 0; i < room->entity_count; i++)
	{
		entity_t* proj = &room->entities[i];

		if (proj->type != E_PROJECTILE || proj->hp <= 0)
		{
			continue;
		}

		entity_projectile_interact(proj, NULL, room);

		if (proj->hp <= 0)
		{
			continue;
		}

		for (u32 j = 0; j < room->entity_count; j++)
		{
			entity_t* target = &room->entities[j];

			if (target->hp <= 0)
			{
				continue;
			}

			s32 previous_hp = target->hp;
			entity_projectile_interact(proj, target, room);

			if (target->hp < previous_hp)
			{
				game->hit_shake_remaining = 0.09f;
			}

            // TODO: we spawn a soul upon killing a monster. At some point bullets
            // might not be the only way to kill monsters, and this should be moved
            // to a more appropriate place. This also applies to generally triggering
            // consequences upon killing an enemy
            if (target->hp <= 0 && target->type == E_MONSTER) {
                Vector2 projDir = Vector2Normalize(proj->vel);


                // TODO: okay i made this indirect in order to avoid modifying
                // the array while we're iterating on it, but is it really a
                // problem? Not sure, we might want to implement an entity-adding-queue
                // just in case

                game->soul_needsToBeSpawned = true;
                game->soul_entityToSpawn = (entity_t){
                    .pos        = target->pos,
                    .speed      = 3,
                    .knockback  = Vector2Scale(projDir, 700),
                    .knockbackDragMultiplier = 10,
                    .hit_stun_remaining = 0.25,

                    .hp         = 20,
                    .name       = "aboba",
                    .type       = E_SOUL,

                    .radius_visible     = 10,
                    .radius_damage_take = 10,
                    .radius_damage_deal = -1000,

                    .invulnerability_remaining = 0.3,
                };

                Consequence csq = GetRandomValue(CSQ_FIRST, CSQ_COUNT - 1);
                if (target->fixedConsequence != CSQ_RANDOM) {
                    csq = target->fixedConsequence;
                }

                if (csq == CSQ_TURRET) {
                    entity_t turretEntity = (entity_t){
                        .pos        = target->pos,
                        .speed      = 0,
                        .knockback  = Vector2Scale(projDir, -500),
                        .knockbackDragMultiplier = 10,

                        .hp         = 999999999,
                        .name       = "aboba",
                        .type       = E_TURRET,

                        .radius_visible     = 30,
                        .radius_damage_take = -1000,
                        .radius_damage_deal = -1000,

                        .reload_remaining = 1.5,
                    };

                    map_add_entity(&turretEntity, room);
                }
                else if (csq == CSQ_BERSERK) {
                    int monster_total = 0;
                    int monster_affected = 0;

                    for (u32 i = 0; i < room->entity_count; i++) {
                        entity_t* ent = &room->entities[i];
                        if (ent->hp <= 0 || ent->type != E_MONSTER) { continue; }

                        monster_total += 1;

                        if (GetRandomValue(0, 100) < 90) {
                            ent->berserk_remaining = 3;
                            monster_affected += 1;
                        }
                    }

                    // TODO: for some reason this doesnt really work?
                    if (monster_total != 0 && monster_affected == 0) {
                        int monster_quota = ClampLo(monster_total / 2, 1);

                        for (u32 i = 0; i < room->entity_count; i++) {
                            if (monster_quota <= 0) { break; }
                            entity_t* ent = &room->entities[i];
                            if (ent->hp <= 0 || ent->type != E_MONSTER) { continue; }

                            monster_quota -= 1;
                            ent->berserk_remaining = 3;
                        }
                    }
                }
            }

			if (proj->hp <= 0)
			{
				break;
			}
		}
	}

	if (!room->exit_open)
	{
		b32 monsters_alive = false;

		for (u32 i = 0; i < room->entity_count; i++)
		{
			entity_t* ent = &room->entities[i];

			if (ent->type == E_MONSTER && ent->hp > 0)
			{
				monsters_alive = true;
				break;
			}
		}

		if (!monsters_alive)
		{
			room->exit_open		  = true;
			room->wall_break_time = (f32)GetTime();

			game->wall_shake_remaining = 0.15f;
		}
	}
}
