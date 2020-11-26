// TJLD
#include <stdio.h>

#include <puzl.h>

#define CHARACTER_BLOCK 219

byte CursorSpeedPatternIndex;
TilePoint CursorPosition;
TilePoint PreviousCursorPosition;
int Score;

void FASTCALL GenerateHWall(byte x, byte y, byte width);
void FASTCALL GenerateVWall(byte x, byte y, byte height);

void InitializeNodeTree(void)
{
	// TODO: Call out custom logic?
	SetBackgroundColor(COLOR_GREY_1);

	GenerateHWall(0, 1, TILEMAP_WIDTH);
	GenerateVWall(0, 2, TILEMAP_HEIGHT - 2);
	GenerateVWall(TILEMAP_WIDTH - 1, 2, TILEMAP_HEIGHT - 2);
	GenerateHWall(0, TILEMAP_HEIGHT - 1, TILEMAP_WIDTH);

	SetTileMapCellShape(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT / 2, CHARACTER_BLOCK);
	SetTileMapCellColor(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT / 2, COLOR_YELLOW);
	
	Score = 0;
	
	CursorPosition.x = 4;
	CursorPosition.y = TILEMAP_HEIGHT / 2;
	SetTileMapCellShape(CursorPosition.x, CursorPosition.y, CHARACTER_BLOCK);
	SetTileMapCellColor(CursorPosition.x, CursorPosition.y, COLOR_BLUE);

	CursorSpeedPatternIndex = 2;

	PrintText("ANDREW", TILEMAP_WIDTH - sizeof("ANDREW") + 1, 0);

	SetSpriteSeconaryColor(COLOR_WHITE);
	SetSpriteTertiaryColor(COLOR_LIGHT_RED);

	EnableSprite(0, 1);
	SetSpritePosition(0, 8 + 0, SCREEN_HEIGHT - SPRITE_HEIGHT - TILE_HEIGHT);
	SetSpriteFrameIndex(0, 0); // 14 right0
	SetSpriteColor(0, COLOR_RED);
	
	EnableSprite(1, 1);
	SetSpritePosition(1, 8 + 32, SCREEN_HEIGHT - SPRITE_HEIGHT - TILE_HEIGHT);
	SetSpriteFrameIndex(1, 0);
	SetSpriteColor(1, COLOR_GREEN);
	
	EnableSprite(2, 1);
	SetSpritePosition(2, 8 + 64, SCREEN_HEIGHT - SPRITE_HEIGHT - TILE_HEIGHT);
	SetSpriteFrameIndex(2, 0); // 21 left0
	SetSpriteColor(2, COLOR_BLUE);
}

void Process(void)
{
	TilePoint cursorDelta;
	cursorDelta.x = cursorDelta.y = 0;

	#ifdef __C64__
	if ((KeyCodeStates[KEY_CODE_SHIFT_LEFT] | KeyCodeStates[KEY_CODE_SHIFT_RIGHT]) != 0)
	{
		cursorDelta.x = 0 - KeyCodeStates[KEY_CODE_RIGHT];
		cursorDelta.y = 0 - KeyCodeStates[KEY_CODE_DOWN];
	}
	else
	{
		cursorDelta.x = KeyCodeStates[KEY_CODE_RIGHT];
		cursorDelta.y = KeyCodeStates[KEY_CODE_DOWN];
	}
	#else
	cursorDelta.x = KeyCodeStates[KEY_CODE_RIGHT] - KeyCodeStates[KEY_CODE_LEFT];
	cursorDelta.y = KeyCodeStates[KEY_CODE_DOWN] - KeyCodeStates[KEY_CODE_UP];
	#endif

	if (cursorDelta.x != 0 || cursorDelta.y != 0)
	{
		if (IsMoving(CursorSpeedPatternIndex) != 0)
		{
			PreviousCursorPosition.x = CursorPosition.x;
			PreviousCursorPosition.y = CursorPosition.y;

			CursorPosition.x += cursorDelta.x;
			CursorPosition.y += cursorDelta.y;
			
			if (GetTileMapShapeCode(CursorPosition.x, CursorPosition.y) != 0)
			{
				switch (GetTileMapColorCode(CursorPosition.x, CursorPosition.y))
				{
					case COLOR_YELLOW:
					{
						Score += 1;
						if (Score >= 3)
						{
							Running = 0;
						}

						#ifndef __CC65__
						// TODO: Figure this out.
						printf("Score: %d", Score);
						printf("\n");
						#endif

						SetBackgroundColor(COLOR_RED);

						break;
					}

					case COLOR_WHITE:
					{
						// Obstacles prevent movement.
						CursorPosition.x = PreviousCursorPosition.x;
						CursorPosition.y = PreviousCursorPosition.y;

						break;
					}
				}
			}
			
			if (CursorPosition.x != PreviousCursorPosition.x || CursorPosition.y != PreviousCursorPosition.y)
			{
				SetTileMapCellColor(CursorPosition.x, CursorPosition.y, COLOR_BLUE);
				SetTileMapCellShape(CursorPosition.x, CursorPosition.y, CHARACTER_BLOCK);
				SetTileMapCellShape(PreviousCursorPosition.x, PreviousCursorPosition.y, 0);
			}
		}
	}
}

void GenerateHWall(byte x, byte y, byte width)
{
	do
	{
		SetTileMapCellColor(x, y, COLOR_WHITE);
		SetTileMapCellShape(x, y, CHARACTER_BLOCK);

		++x;
	}
	while (--width > 0);
}

void GenerateVWall(byte x, byte y, byte height)
{
	do
	{
		SetTileMapCellColor(x, y, COLOR_WHITE);
		SetTileMapCellShape(x, y, CHARACTER_BLOCK);

		++y;
	}
	while (--height > 0);
}
