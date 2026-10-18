#include "vendor/raylib/raylib.h"
#include <math.h>

#include "graphics.h"
#include "map.h"
#include "entity.h"
#include "weapon.h"
#include "arena.h"

int main(void)
{
	graphics_init();

	arena_t* permanent_arena = arena_init(MEGABYTES(64), ARENA_NO_FLAGS);

	map_world_t* world	 = map_init_world();
	entity_t*	 monster = entity_init_monster();
	entity_t*	 player	 = entity_player_init();
	player->weapon		 = weapon_init_base(permanent_arena);

	map_room_t* room  = &world->rooms[8][8];
	room->entities[0] = player;
	room->entities[1] = monster;

	while (!WindowShouldClose())
	{
		graphics_loop(room);
		entity_interactions(room);
	}

	CloseWindow();
}
