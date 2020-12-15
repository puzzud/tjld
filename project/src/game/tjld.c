// TJLD
#include <stdio.h>
#include <stdlib.h>

#include <puzl.h>

#define CHARACTER_BLOCK 219

#define PLAYER_SPRITE_INDEX 1

// TODO: Hide this pragma from common code.
#pragma bss-name (push,"ZEROPAGE")
byte LoopIndex;

Vector2d IntendedDirection;

byte SpriteSpeedPatternIndex;
Vector2d SpriteTilePosition;
#pragma bss-name (pop)

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
byte GetTileOffsetFromPoint(char point);
void CheckSpriteTile(void);

void AddNewPickup(void);

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

	EnableSprite(PLAYER_SPRITE_INDEX, 1);
	SetSpritePosition(PLAYER_SPRITE_INDEX, 8 + 0, SCREEN_HEIGHT - SPRITE_HEIGHT - TILE_HEIGHT);
	SetSpriteFrameIndex(PLAYER_SPRITE_INDEX, 0);
	SetSpriteColor(PLAYER_SPRITE_INDEX, COLOR_RED);

	CheckSpriteTile();

	PlayAudioPattern(0, Voice1Start, 1);
	PlayAudioPattern(2, Voice2Start, 1);
}

void Process(void)
{
	UpdateIntendedDirection();

	LoopIndex = 2;
	do
	{
		if ((IntendedDirection.x | IntendedDirection.y) != 0)
		{
			if (IsMoving(SpriteSpeedPatternIndex) != 0)
			{
				SetSpriteVelocity(PLAYER_SPRITE_INDEX, IntendedDirection.x, IntendedDirection.y);

				MoveSprite(PLAYER_SPRITE_INDEX);

				CheckSpriteTile();
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
		SetTileMapCellCollisionCode(x, y, 1);

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
		SetTileMapCellCollisionCode(x, y, 1);

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

void CheckSpriteTile(void)
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
				if (Score >= 8)
				{
					Running = 0;
				}
				else
				{
					AddNewPickup();

					SetTileMapCellShape(SpriteTilePosition.x, SpriteTilePosition.y, 0);
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
		}
	}
}

byte GetTileOffsetFromPoint(char point)
{
	return point << 3;
}

byte GetSpriteTilePositionX(void)
{
	byte SpriteTilePositionX = (GetSpritePositionX(PLAYER_SPRITE_INDEX) + 7) / TILE_WIDTH;
	if (SpriteTilePositionX >= TILEMAP_WIDTH)
	{
		SpriteTilePositionX = TILEMAP_WIDTH;
	}

	return SpriteTilePositionX;
}

byte GetSpriteTilePositionY(void)
{
	byte SpriteTilePositionY = (GetSpritePositionY(PLAYER_SPRITE_INDEX) + TILE_HEIGHT + (TILE_HEIGHT * 3 / 4)) / TILE_HEIGHT;
	if (SpriteTilePositionY >= TILEMAP_HEIGHT)
	{
		SpriteTilePositionY = TILEMAP_HEIGHT;
	}

	return SpriteTilePositionY;
}

void AddNewPickup(void)
{
	Vector2d randomTile;

	do
	{
		// TODO: Use of rand in cc65 C64 is warning about initialized data in BSS.
		randomTile.x = rand() % TILEMAP_WIDTH;
		randomTile.y = (rand() % (TILEMAP_HEIGHT - 1)) + 1;
	}
	while (GetTileMapShapeCode(randomTile.x, randomTile.y) == CHARACTER_BLOCK);

	SetTileMapCellShape(randomTile.x, randomTile.y, CHARACTER_BLOCK);
	SetTileMapCellColor(randomTile.x, randomTile.y, COLOR_YELLOW);
}
