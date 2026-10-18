#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "entity.h"
#include "input.h"
#include "weapon.h"

char* player_name = "The Hero of Our Story";

entity_t* entity_player_init(void)
{
	entity_t* pla = malloc(sizeof(entity_t));
	pla->hp		  = 100;
	pla->speed	  = 4;
	pla->name	  = player_name;
	pla->type	  = E_PLAYER;
	pla->pos[0]	  = 40;
	pla->pos[1]	  = 40;
	return pla;
}

void entity_player_interact(entity_t* player, entity_t* other, map_room_t* room)
{
	(void)room;
	(void)player;
	(void)other;
	if (other == NULL)
	{
		input_state_t inp = input_update();
		player->vel[0]	  = player->speed * inp.move[0];
		player->vel[1]	  = player->speed * inp.move[1];

		if (inp.fire[0] != 0 || inp.fire[1] != 0)
		{
			weapon_fire(player, room, inp.fire);
		}

		return;
	}

	// if(strcmp(other->type, "monster") == 0) {
	//   player->hp -= 10;
	// }
}
