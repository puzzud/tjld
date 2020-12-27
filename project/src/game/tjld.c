// TJLD
#include <stdio.h>

#include <puzl.h>

#include <level.h>
#include <dwarf.h>

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

void UpdateIntendedDirection(void);
void UpdateSpriteAnimation(void);
byte GetSpriteTilePositionX(void);
byte GetSpriteTilePositionY(void);
void CheckSpriteTile(void);

void InitializeNodeTree(void)
{
	LoadLevel();
	
	Score = 0;

	SpriteSpeedPatternIndex = 8;

	PrintText(AuthorFirstName, TARGET_SCREEN_TILE_OFFSET_X + 0, 0);

	SetSpriteSeconaryColor(COLOR_WHITE);
	SetSpriteTertiaryColor(COLOR_LIGHT_RED);

	EnableSprite(PLAYER_SPRITE_INDEX, 1);
	SetSpritePosition(PLAYER_SPRITE_INDEX, 48, SCREEN_HEIGHT - SPRITE_HEIGHT - (TILE_HEIGHT * 2));
	SetSpriteFrameIndex(PLAYER_SPRITE_INDEX, 1);
	SetSpriteColor(PLAYER_SPRITE_INDEX, COLOR_RED);
	SetSpriteAnimationSet(PLAYER_SPRITE_INDEX, DwarfAnimationSet);

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
				SetSpriteVelocity(PLAYER_SPRITE_INDEX, IntendedDirection.x, 0);//IntendedDirection.y);

				MoveSprite(PLAYER_SPRITE_INDEX);

				CheckSpriteTile();
			}
		}

		CycleSpeedBit();
	}
	while (--LoopIndex != 0);

	UpdateSpriteAnimation();
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

void UpdateSpriteAnimation(void)
{
	signed char intendedDirectionX = IntendedDirection.x;

	if (intendedDirectionX != 0)
	{
		if (intendedDirectionX < 0)
		{
			PlaySpriteAnimation(PLAYER_SPRITE_INDEX, DWARF_ANIMATION_ID_LEFT_WALK, 1);
		}
		else if (intendedDirectionX > 0)
		{
			PlaySpriteAnimation(PLAYER_SPRITE_INDEX, DWARF_ANIMATION_ID_RIGHT_WALK, 1);
		}
	}
	else
	{
		PlaySpriteAnimation(PLAYER_SPRITE_INDEX, DWARF_ANIMATION_ID_FRONT_IDLE, 1);
	}
}

void CheckSpriteTile(void)
{
	SpriteTilePosition.x = GetSpriteTilePositionX();
	SpriteTilePosition.y = GetSpriteTilePositionY();

	if (GetTileMapShapeCode(SpriteTilePosition.x, SpriteTilePosition.y) != 0)
	{
		switch (GetTileMapColorCode(SpriteTilePosition.x, SpriteTilePosition.y))
		{
			case PICKUP_BLOCK_COLOR:
			{
				Score += 1;
				
				SetTileMapCellShape(SpriteTilePosition.x, SpriteTilePosition.y, 0);

				PlayAudioPattern(1, PickupSound, 0);

				break;
			}
		}
	}
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
