// TJLD
#include <SDL2/SDL.h>
#include <stdio.h>

#include "core.h"
#include "video.h"
#include "color.h"
#include "input.h"

int Process();

void InitializeNodeTree();

int CursorXPosition;
int CursorYPosition;
int Score;

int Process()
{
	int cursorXDelta = KeyScanCodeStates[SDL_SCANCODE_RIGHT] - KeyScanCodeStates[SDL_SCANCODE_LEFT];
	int cursorYDelta = KeyScanCodeStates[SDL_SCANCODE_DOWN] - KeyScanCodeStates[SDL_SCANCODE_UP];

	if (cursorXDelta != 0 || cursorYDelta != 0)
	{
		SetTileMapCell(CursorXPosition, CursorYPosition, 0, COLOR_BLACK);
		CursorXPosition += cursorXDelta;
		CursorYPosition += cursorYDelta;

		if (GetTileMapShapeCode(CursorXPosition, CursorYPosition) != 0)
		{
			if (GetTileMapColorCode(CursorXPosition, CursorYPosition) == COLOR_YELLOW)
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

		SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	}

	return 0;
}

void InitializeNodeTree()
{
	// TODO: Call out custom logic?
	Score = 0;

	CursorXPosition = 0;
	CursorYPosition = 0;
	SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);

	SetTileMapCell(TileMapWidth / 2, TileMapHeight / 2, 1, COLOR_YELLOW);
}
