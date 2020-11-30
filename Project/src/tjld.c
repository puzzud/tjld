// TJLD
#include <stdio.h>

#include <puzl.h>

#define CHARACTER_BLOCK 219

byte LoopIndex;

Direction IntendedDirection;

byte SpriteSpeedPatternIndex;
TilePoint SpriteTilePosition;
TilePoint PreviousSpriteTilePosition;
int Score;

const char AuthorFirstName[] = "ANDREW";

void FASTCALL GenerateHWall(byte x, byte y, byte width);
void FASTCALL GenerateVWall(byte x, byte y, byte height);

void UpdateIntendedDirection(void);
void UpdateSpriteTile(void);

void InitializeNodeTree(void)
{
	// TODO: Call out custom logic?
	SetBackgroundColor(COLOR_BLACK);

	GenerateHWall(0, 1, TILEMAP_WIDTH);
	GenerateVWall(0, 2, TILEMAP_HEIGHT - 2);
	GenerateVWall(TILEMAP_WIDTH - 1, 2, TILEMAP_HEIGHT - 2);
	GenerateHWall(0, TILEMAP_HEIGHT - 1, TILEMAP_WIDTH);

	SetTileMapCellShape(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT / 2, CHARACTER_BLOCK);
	SetTileMapCellColor(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT / 2, COLOR_YELLOW);
	
	Score = 0;
	
	SpriteTilePosition.x = 4;
	SpriteTilePosition.y = TILEMAP_HEIGHT / 2;
	SetTileMapCellShape(SpriteTilePosition.x, SpriteTilePosition.y, CHARACTER_BLOCK);
	SetTileMapCellColor(SpriteTilePosition.x, SpriteTilePosition.y, COLOR_BLUE);

	SpriteSpeedPatternIndex = 8;

	PrintText(AuthorFirstName, 0, 0);

	SetSpriteSeconaryColor(COLOR_WHITE);
	SetSpriteTertiaryColor(COLOR_LIGHT_RED);

	EnableSprite(0, 1);
	SetSpritePosition(0, 8 + 0, SCREEN_HEIGHT - SPRITE_HEIGHT - TILE_HEIGHT);
	SetSpriteFrameIndex(0, 0);
	SetSpriteColor(0, COLOR_RED);
}

void Process(void)
{
	UpdateIntendedDirection();

	if (IntendedDirection.x != 0 || IntendedDirection.y != 0)
	{
		if (IsMoving(SpriteSpeedPatternIndex) != 0)
		{
			SetSpriteVelocity(0, IntendedDirection.x, IntendedDirection.y);

			PreviousSpriteTilePosition.x = SpriteTilePosition.x;
			PreviousSpriteTilePosition.y = SpriteTilePosition.y;

			LoopIndex = 2;
			do
			{
				MoveSprite(0);
			}
			while (--LoopIndex != 0);

			UpdateSpriteTile();
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

void UpdateIntendedDirection(void)
{
	IntendedDirection.x = IntendedDirection.y = 0;

	#ifdef __C64__
	if ((KeyCodeStates[KEY_CODE_SHIFT_LEFT] | KeyCodeStates[KEY_CODE_SHIFT_RIGHT]) != 0)
	{
		IntendedDirection.x = 0 - KeyCodeStates[KEY_CODE_RIGHT];
		IntendedDirection.y = 0 - KeyCodeStates[KEY_CODE_DOWN];
	}
	else
	{
		IntendedDirection.x = KeyCodeStates[KEY_CODE_RIGHT];
		IntendedDirection.y = KeyCodeStates[KEY_CODE_DOWN];
	}
	#else
	IntendedDirection.x = KeyCodeStates[KEY_CODE_RIGHT] - KeyCodeStates[KEY_CODE_LEFT];
	IntendedDirection.y = KeyCodeStates[KEY_CODE_DOWN] - KeyCodeStates[KEY_CODE_UP];

	if (KeyCodeStates[KEY_CODE_ESCAPE] != 0)
	{
		Running = 0;
	}
	#endif
}

void UpdateSpriteTile(void)
{
	SpriteTilePosition.x += IntendedDirection.x;
	SpriteTilePosition.y += IntendedDirection.y;
	
	if (GetTileMapShapeCode(SpriteTilePosition.x, SpriteTilePosition.y) != 0)
	{
		switch (GetTileMapColorCode(SpriteTilePosition.x, SpriteTilePosition.y))
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

				SetBackgroundColor(COLOR_GREY_1);

				break;
			}

			case COLOR_WHITE:
			{
				// Obstacles prevent movement.
				SpriteTilePosition.x = PreviousSpriteTilePosition.x;
				SpriteTilePosition.y = PreviousSpriteTilePosition.y;

				break;
			}
		}
	}
	
	if (SpriteTilePosition.x != PreviousSpriteTilePosition.x || SpriteTilePosition.y != PreviousSpriteTilePosition.y)
	{
		SetTileMapCellColor(SpriteTilePosition.x, SpriteTilePosition.y, COLOR_BLUE);
		SetTileMapCellShape(SpriteTilePosition.x, SpriteTilePosition.y, CHARACTER_BLOCK);
		SetTileMapCellShape(PreviousSpriteTilePosition.x, PreviousSpriteTilePosition.y, 0);
	}
}
