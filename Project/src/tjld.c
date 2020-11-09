// TJLD
#include <SDL2/SDL.h>
#include <stdio.h>

#include "core.h"
#include "video.h"
#include "color.h"
#include "input.h"

SDL_Point CursorPosition;
int Score;

int Process()
{
	SDL_Point cursorDelta;
	cursorDelta.x = KeyScanCodeStates[SDL_SCANCODE_RIGHT] - KeyScanCodeStates[SDL_SCANCODE_LEFT];
	cursorDelta.y = KeyScanCodeStates[SDL_SCANCODE_DOWN] - KeyScanCodeStates[SDL_SCANCODE_UP];

	if (cursorDelta.x != 0 || cursorDelta.y != 0)
	{
		SetTileMapCell(CursorPosition.x, CursorPosition.y, 0, COLOR_BLACK);
		CursorPosition.x += cursorDelta.x;
		CursorPosition.y += cursorDelta.y;

		if (GetTileMapShapeCode(CursorPosition.x, CursorPosition.y) != 0)
		{
			if (GetTileMapColorCode(CursorPosition.x, CursorPosition.y) == COLOR_YELLOW)
			{
				Score += 1;
				if (Score == 1)
				{
					Running = 0;
				}

				printf("Score: %d", Score);
				printf("\n");
			}
		}

		SetTileMapCell(CursorPosition.x, CursorPosition.y, 1, COLOR_LIGHT_BLUE);
	}

	return 0;
}

void InitializeNodeTree()
{
	// TODO: Call out custom logic?
	Score = 0;

	CursorPosition.x = 0;
	CursorPosition.y = 0;
	SetTileMapCell(CursorPosition.x, CursorPosition.y, 1, COLOR_LIGHT_BLUE);

	SetTileMapCell(TileMapDimensions.x / 2, TileMapDimensions.y / 2, 1, COLOR_YELLOW);
}
