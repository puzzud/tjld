// TJLD
#include <stdlib.h>
#include <stdio.h>

#include <puzl.h>

#include <speed.h>
#include <level.h>
#include <dwarf.h>

#define PLAYER_SPRITE_INDEX 1

// TODO: Hide this pragma from common code.
#pragma bss-name (push,"ZEROPAGE")
byte LoopIndex;

Vector2d IntendedDirection;

byte SpriteSpeedPatternIndex;
Vector2d SpriteTilePosition;

byte SpriteClimbing;
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

void CheckSpriteClimbing(void);
void SpriteClimbingAlignLadderTouching(void);
void SpriteClimbingAlignLadderAbove(void);

void InitializeNodeTree(void)
{
	InitalizeSpeed();

	LoadLevel();
	
	Score = 0;
	SpriteClimbing = 0;

	SpriteSpeedPatternIndex = 8;

	PrintColor = COLOR_WHITE;
	PrintText(AuthorFirstName, TARGET_SCREEN_TILE_OFFSET_X + 0, 0);

	SetSpriteSeconaryColor(COLOR_WHITE);
	SetSpriteTertiaryColor(COLOR_LIGHT_RED);

	CurrentSpriteIndex = PLAYER_SPRITE_INDEX;
	EnableSprite(1);
	SetSpritePosition
	(
		(TARGET_SCREEN_TILE_OFFSET_X * TILE_WIDTH) + (TILE_WIDTH * 2),
		TARGET_SCREEN_TILE_OFFSET_Y + TARGET_SCREEN_HEIGHT - (TILE_HEIGHT * 2) - SPRITE_HEIGHT
	);
	SetSpriteFrameIndex(1);
	SetSpriteColor(COLOR_RED);
	SetSpriteAnimationSet(DwarfAnimationSet);

	SpriteCollisionMasks[CurrentSpriteIndex] = COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_LADDER;

	CheckSpriteTile();

	PlayAudioPattern(0, Voice1Start, 1);
	PlayAudioPattern(2, Voice2Start, 1);
}

void Process(void)
{
	UpdateIntendedDirection();

	CurrentSpriteIndex = PLAYER_SPRITE_INDEX;

	LoopIndex = 2;
	do
	{
		if ((IntendedDirection.x | IntendedDirection.y) != 0)
		{
			if (IsMoving(SpriteSpeedPatternIndex) != 0)
			{
				CheckSpriteClimbing();

				SpriteVelocitiesX[CurrentSpriteIndex] = IntendedDirection.x;
				SpriteVelocitiesY[CurrentSpriteIndex] = IntendedDirection.y;

				MoveSprite();

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

	#ifdef HAS_KEYBOARD
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
	#endif
}

void UpdateSpriteAnimation(void)
{
	signed char intendedDirectionX = IntendedDirection.x;
	signed char intendedDirectionY = IntendedDirection.y;

	if (intendedDirectionX != 0)
	{
		if (intendedDirectionX < 0)
		{
			PlaySpriteAnimation(DWARF_ANIMATION_ID_LEFT_WALK, 1);
		}
		else if (intendedDirectionX > 0)
		{
			PlaySpriteAnimation(DWARF_ANIMATION_ID_RIGHT_WALK, 1);
		}
	}
	else
	{
		if (SpriteClimbing != 0)
		{
			if (intendedDirectionY != 0)
			{
				PlaySpriteAnimation(DWARF_ANIMATION_ID_BACK_CLIMB, 1);
			}
			else
			{
				//StopSpriteAnimation();
			}
		}
		else
		{
			PlaySpriteAnimation(DWARF_ANIMATION_ID_FRONT_IDLE, 1);
		}
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
	byte SpriteTilePositionX = (GetSpritePositionX() + 7) / TILE_WIDTH;
	if (SpriteTilePositionX >= TILEMAP_WIDTH)
	{
		SpriteTilePositionX = TILEMAP_WIDTH;
	}

	return SpriteTilePositionX;
}

byte GetSpriteTilePositionY(void)
{
	byte SpriteTilePositionY = (GetSpritePositionY() + TILE_HEIGHT + (TILE_HEIGHT * 3 / 4)) / TILE_HEIGHT;
	if (SpriteTilePositionY >= TILEMAP_HEIGHT)
	{
		SpriteTilePositionY = TILEMAP_HEIGHT;
	}

	return SpriteTilePositionY;
}

void CheckSpriteClimbing(void)
{
	if (IntendedDirection.y < 0)
	{
		// Pressing up.

		// Check if sprite is touching a ladder.
		if ((SpriteCollisions[CurrentSpriteIndex] & COLLISION_FLAG_LADDER) != 0)
		{
			// Touching a ladder.

			SpriteClimbing = 1;
			//IntendedDirection.x = 0;

			SpriteClimbingAlignLadderTouching();
		}
		else
		{
			SpriteClimbing = 0;
			IntendedDirection.y = 0;
		}
	}
	else if (IntendedDirection.y > 0)
	{
		// Pressing down.

		byte belowTileCollision = GetTileMapCellCollisionCode(SpriteTilePosition.x, SpriteTilePosition.y + 1);
		if ((belowTileCollision & COLLISION_FLAG_OBSTACLE) != 0)
		{
			// Pressing down into a platform.
			SpriteClimbing = 0;
		}
		else if ((belowTileCollision & COLLISION_FLAG_LADDER) != 0)
		{
			// Pressing down with ladder below.

			SpriteClimbing = 1;
			SpriteClimbingAlignLadderAbove();
		}
		else
		{
			SpriteClimbing = 0;
			IntendedDirection.y = 0;
		}
	}
	else if (SpriteClimbing != 0)
	{
		IntendedDirection.x = 0;
	}
}

void SpriteClimbingAlignLadderTouching(void)
{
	// When climbing, don't allow sprite to intentionally move horizontally.
	// Instead, move it horizontally to align with the ladder block.

	// Get direction from sprite to ladder.

	// If sprite does not line up evenly with 16x16 ladder block,
	// move it in the direction to that block.

	// Otherwise, set X direction to 0.

	word spriteX = GetSpritePositionX();
	word spriteY;

	if ((spriteX % 16) != 0)
	{
		spriteY = GetSpritePositionY();

		if ((GetTileMapCellCollisionCode(spriteX / 8, spriteY / 8) & COLLISION_FLAG_LADDER) != 0)
		{
			IntendedDirection.x = -1;
		}
		else
		{
			IntendedDirection.x = 1;
		}
	}
	else
	{
		IntendedDirection.x = 0;
	}
}

void SpriteClimbingAlignLadderAbove(void)
{
	word spriteX = GetSpritePositionX();
	
	if ((spriteX % 16) != 0)
	{
		if ((GetTileMapCellCollisionCode(spriteX / 8, SpriteTilePosition.y + 1) & COLLISION_FLAG_LADDER) != 0)
		{
			IntendedDirection.x = -1;
		}
		else
		{
			IntendedDirection.x = 1;
		}
	}
	else
	{
		IntendedDirection.x = 0;
	}
}
