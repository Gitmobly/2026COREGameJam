#include <raylib.h>

#include "arena.h"
#include "utils.h"

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

int main(void)
{
	InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Game");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("2026 CORE GAME JAM", 10, 10, 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
