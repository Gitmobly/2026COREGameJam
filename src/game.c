#include "game.h"
#include "entity.h"
#include "map.h"
#include "main.h"
#include "graphics.h"

void game_state_init(game_state_t* state)
{
	*state = (game_state_t){0};
}

void game_next_room(app_state_t* app)
{
	game_state_t*  game	 = app->game_state;
	level_state_t* level = &game->current_level;
	map_room_t*	   room	 = level->active_room;

	entity_t player = *level->entity_player;
	player.pos		= (Vector2){40.0f, (SCREEN_HT - BOUNDARY_WIDTH * 2) * 0.5f};
	player.vel		= (Vector2){0};

	game->room_number++;
	entity_t *spawned_player = map_generate_room(room, &player, game->room_number);

	level->entity_player	   = spawned_player;
    level->entity_controlled   = level->entity_player;
	game->player_hitting_wall  = 0;
	game->wall_shake_remaining = 0.0f;
}
