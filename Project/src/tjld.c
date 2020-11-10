// TJLD
#include <stdio.h>

#include <puzl.h>

Point CursorPosition;
int Score;

int Process()
{
	Point cursorDelta;
	cursorDelta.x = KeyCodeStates[KEY_CODE_RIGHT] - KeyCodeStates[KEY_CODE_LEFT];
	cursorDelta.y = KeyCodeStates[KEY_CODE_DOWN] - KeyCodeStates[KEY_CODE_UP];

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
