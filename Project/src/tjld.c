// TJLD
#include <stdio.h>

#include <puzl.h>

Point CursorPosition;
int Score;

void InitializeNodeTree(void)
{
	// TODO: Call out custom logic?
	Score = 0;
	
	CursorPosition.x = 0;
	CursorPosition.y = 0;
	SetTileMapCellShape(CursorPosition.x, CursorPosition.y, 'O');
	SetTileMapCellColor(CursorPosition.x, CursorPosition.y, COLOR_BLUE);
	
	SetTileMapCellShape(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT / 2, '*');
	SetTileMapCellColor(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT / 2, COLOR_YELLOW);
	
	SetBackgroundColor(COLOR_GREY_1);

	PrintText("ANDREW", TILEMAP_WIDTH - sizeof("ANDREW") + 1, 0);
}

void Process(void)
{
	Point cursorDelta;
	cursorDelta.x = 0;//KeyCodeStates[KEY_CODE_RIGHT] - KeyCodeStates[KEY_CODE_LEFT];
	cursorDelta.y = 0;//KeyCodeStates[KEY_CODE_DOWN] - KeyCodeStates[KEY_CODE_UP];
	
	if (cursorDelta.x != 0 || cursorDelta.y != 0)
	{
		SetTileMapCellShape(CursorPosition.x, CursorPosition.y, 0);
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

				#ifndef __CC65__
				// TODO: Figure this out.
				printf("Score: %d", Score);
				printf("\n");
				#endif
			}
		}
		
		SetTileMapCellShape(CursorPosition.x, CursorPosition.y, 'O');
		SetTileMapCellColor(CursorPosition.x, CursorPosition.y, COLOR_BLUE);
	}
}
