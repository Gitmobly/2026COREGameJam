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

	entity_t player = *level->player_entity;
	player.pos		= (Vector2){40.0f, (SCREEN_HT - PAD * 2) * 0.5f};
	player.vel		= (Vector2){0};

	game->room_number++;
	map_generate_room(room, &player, game->room_number);

	level->player_entity	   = &room->entities[0];
	game->player_hitting_wall  = 0;
	game->wall_shake_remaining = 0.0f;
}
