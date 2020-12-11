// TJLD
#include <stdio.h>

#include <puzl.h>

#define CHARACTER_BLOCK 219

byte LoopIndex;

Direction IntendedDirection;

byte SpriteSpeedPatternIndex;
SpritePoint PreviousSpritePosition;
TilePoint PreviousSpriteTilePosition;
TilePoint SpriteTilePosition;

int Score;

const char AuthorFirstName[] = "ANDREW";

extern const byte Voice1Start[];
extern const byte Voice2Start[];
extern const byte PickupSound[];

void FASTCALL GenerateHWall(byte x, byte y, byte width);
void FASTCALL GenerateVWall(byte x, byte y, byte height);

void UpdateIntendedDirection(void);
byte GetSpriteTilePositionX(void);
byte GetSpriteTilePositionY(void);
void UpdateSpriteTile(void);

void InitializeNodeTree(void)
{
	// TODO: Call out custom logic?
	SetBackgroundColor(COLOR_BLACK);

	GenerateHWall(0, 1, TILEMAP_WIDTH);
	GenerateVWall(0, 2, TILEMAP_HEIGHT - 2);
	GenerateVWall(TILEMAP_WIDTH - 1, 2, TILEMAP_HEIGHT - 2);
	GenerateHWall(0, TILEMAP_HEIGHT - 1, TILEMAP_WIDTH);

	SetTileMapCellShape(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT - 2, CHARACTER_BLOCK);
	SetTileMapCellColor(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT - 2, COLOR_YELLOW);
	
	Score = 0;

	SpriteSpeedPatternIndex = 8;

	PrintText(AuthorFirstName, 0, 0);

	SetSpriteSeconaryColor(COLOR_WHITE);
	SetSpriteTertiaryColor(COLOR_LIGHT_RED);

	EnableSprite(0, 1);
	SetSpritePosition(0, 8 + 0, SCREEN_HEIGHT - SPRITE_HEIGHT - TILE_HEIGHT);
	SetSpriteFrameIndex(0, 0);
	SetSpriteColor(0, COLOR_RED);

	PreviousSpriteTilePosition.x = GetSpriteTilePositionX();
	PreviousSpriteTilePosition.y = GetSpriteTilePositionY();
	UpdateSpriteTile();

	PlayAudioPattern(0, Voice1Start, 1);
	PlayAudioPattern(2, Voice2Start, 1);
}

void Process(void)
{
	UpdateIntendedDirection();

	LoopIndex = 2;
	do
	{
		if (IntendedDirection.x != 0 || IntendedDirection.y != 0)
		{
			if (IsMoving(SpriteSpeedPatternIndex) != 0)
			{
				SetSpriteVelocity(0, IntendedDirection.x, IntendedDirection.y);

				PreviousSpritePosition.x = GetSpritePositionX(0);
				PreviousSpritePosition.y = GetSpritePositionY(0);

				PreviousSpriteTilePosition.x = SpriteTilePosition.x;
				PreviousSpriteTilePosition.y = SpriteTilePosition.y;
				
				MoveSprite(0);

				UpdateSpriteTile();
			}
		}

		CycleSpeedBit();
	}
	while (--LoopIndex != 0);
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
	IntendedDirection.x = ControllerAxisXState;
	IntendedDirection.y = ControllerAxisYState;

	#ifndef __C64__
	IntendedDirection.x += KeyCodeStates[KEY_CODE_RIGHT] - KeyCodeStates[KEY_CODE_LEFT];
	if (abs(IntendedDirection.x) > 1)
	{
		IntendedDirection.x /= 2;
	}

	IntendedDirection.y += KeyCodeStates[KEY_CODE_DOWN] - KeyCodeStates[KEY_CODE_UP];
	if (abs(IntendedDirection.y) > 1)
	{
		IntendedDirection.y /= 2;
	}

	if (KeyCodeStates[KEY_CODE_ESCAPE] != 0)
	{
		Running = 0;
	}
	#endif
}

void UpdateSpriteTile(void)
{
	SpriteTilePosition.x = GetSpriteTilePositionX();
	SpriteTilePosition.y = GetSpriteTilePositionY();

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

				PlayAudioPattern(1, PickupSound, 0);

				break;
			}

			case COLOR_WHITE:
			{
				// Obstacles prevent movement.
				SetSpritePosition(0, PreviousSpritePosition.x, PreviousSpritePosition.y);
				
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

byte GetSpriteTilePositionX(void)
{
	byte SpriteTilePositionX = (GetSpritePositionX(0) + 7) / TILE_WIDTH;
	if (SpriteTilePositionX >= TILEMAP_WIDTH)
	{
		SpriteTilePositionX = TILEMAP_WIDTH;
	}

	return SpriteTilePositionX;
}

byte GetSpriteTilePositionY(void)
{
	byte SpriteTilePositionY = (GetSpritePositionY(0) + TILE_HEIGHT + (TILE_HEIGHT * 3 / 4)) / TILE_HEIGHT;
	if (SpriteTilePositionY >= TILEMAP_HEIGHT)
	{
		SpriteTilePositionY = TILEMAP_HEIGHT;
	}

	return SpriteTilePositionY;
}
