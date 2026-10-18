#include "entity.h"
#include "vendor/raylib/raylib.h"
#include <stdlib.h>
#include <string.h>

#include "graphics.h"

#define SCREEN_WD 800
#define SCREEN_HT 600
#define PAD		  32

Color get_color(entity_t* entity)
{
	return entity->type == E_MONSTER ? RED : SKYBLUE;
}

int get_size(entity_t* entity)
{
	return entity->type == E_PROJECTILE ? 8 : 16;
}

void graphics_init(void)
{
	InitWindow(SCREEN_WD, SCREEN_HT, "2026 Core Game Jam");
	SetTargetFPS(60);
}

void graphics_loop(map_room_t* active_room)
{
	BeginDrawing();
	ClearBackground(DARKGRAY);

	DrawRectangle(PAD, PAD, SCREEN_WD - (2 * PAD), SCREEN_HT - (2 * PAD), BLACK);

	for (int i = 0; i < 32; i++)
	{
		if (active_room->entities[i] == NULL)
		{
			continue;
		}
		double* pos = active_room->entities[i]->pos;
		DrawCircle(PAD + (int)pos[0], PAD + (int)pos[1], (f32)get_size(active_room->entities[i]),
				   get_color(active_room->entities[i]));
	}

	EndDrawing();
}
