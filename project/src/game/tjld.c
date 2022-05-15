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

byte SpriteSpeedPatternIndex;

Vector2d IntendedDirection;
Vector2d SpriteDirection;

Vector2d SpriteTilePosition;
Vector2d SpriteTilePositionBelowLeft;
Vector2d SpriteTilePositionBelowRight;

byte SpriteClimbing;
byte SpriteFalling;
#pragma bss-name (pop)

int Score;

const char AuthorFirstName[] = "ANDREW";

extern const byte Voice1Start[];
extern const byte Voice2Start[];
extern const byte PickupSound[];

void UpdateIntendedDirection(void);
void UpdateSpriteAnimation(void);
void CheckSpriteTiles(void);

void CheckSpriteFalling(void);

void CheckSpriteClimbing(void);
void SpriteClimbingAlignLadderTouching(void);
void SpriteClimbingAlignLadderAbove(void);

void InitializeNodeTree(void)
{
	InitalizeSpeed();

	LoadLevel();
	
	Score = 0;
	SpriteClimbing = 0;
	SpriteFalling = 0;

	SpriteSpeedPatternIndex = 8;

	PrintX = TARGET_SCREEN_TILE_OFFSET_X + 0;
	PrintY = 0;
	PrintColor = COLOR_WHITE;
	PrintText(AuthorFirstName);

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

	CheckSpriteTiles();

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
		if (CanMove(SpriteSpeedPatternIndex) != 0)
		{
			CheckSpriteClimbing();
			if (SpriteClimbing != 0)
			{
				SpriteVelocitiesX[CurrentSpriteIndex] = SpriteDirection.x;
				SpriteVelocitiesY[CurrentSpriteIndex] = SpriteDirection.y;
			}
			else
			{
				CheckSpriteFalling();
				if (SpriteFalling != 0)
				{
					SpriteVelocitiesX[CurrentSpriteIndex] = SpriteDirection.x = 0;
					SpriteVelocitiesY[CurrentSpriteIndex] = SpriteDirection.y = 1;
				}
				else
				{
					SpriteVelocitiesX[CurrentSpriteIndex] = SpriteDirection.x = IntendedDirection.x;
					SpriteVelocitiesY[CurrentSpriteIndex] = SpriteDirection.y = 0;	
				}
			}

			if ((SpriteVelocitiesX[CurrentSpriteIndex] | SpriteVelocitiesY[CurrentSpriteIndex]) != 0)
			{
				if (SpriteFalling != 0)
				{
					SpriteCollisionMasks[CurrentSpriteIndex] = 0;
				}

				MoveSprite();
				
				SpriteCollisionMasks[CurrentSpriteIndex] = COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_LADDER;
			}

			CheckSpriteTiles();
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
	signed char spriteDirectionX = SpriteDirection.x;
	signed char spriteDirectionY = SpriteDirection.y;

	if (SpriteFalling != 0)
	{
		PlaySpriteAnimation(DWARF_ANIMATION_ID_FRONT_JUMP, 1);
		return;
	}

	if ((spriteDirectionX | spriteDirectionY) == 0)
	{
		PlaySpriteAnimation(DWARF_ANIMATION_ID_FRONT_IDLE, 1);
	}
	else if (SpriteClimbing != 0)
	{
		PlaySpriteAnimation(DWARF_ANIMATION_ID_BACK_CLIMB, 1);
	}
	else if (spriteDirectionX < 0)
	{
		PlaySpriteAnimation(DWARF_ANIMATION_ID_LEFT_WALK, 1);
	}
	else if (spriteDirectionX > 0)
	{
		PlaySpriteAnimation(DWARF_ANIMATION_ID_RIGHT_WALK, 1);
	}
}

void CheckSpriteTiles(void)
{
	signed short spritePositionX = GetSpritePositionX();
	signed short spritePositionY = GetSpritePositionY();

	SpriteTilePosition.x = (spritePositionX + 7) / TILE_WIDTH;
	SpriteTilePosition.y = (spritePositionY + TILE_HEIGHT) / TILE_HEIGHT;

	SpriteTilePositionBelowLeft.x = spritePositionX / TILE_WIDTH;
	SpriteTilePositionBelowRight.y = SpriteTilePositionBelowLeft.y = (spritePositionY + (TILE_HEIGHT * 2)) / TILE_HEIGHT;
	SpriteTilePositionBelowRight.x = (spritePositionX + (TILE_WIDTH * 2) - 1) / TILE_WIDTH;
	
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

void CheckSpriteFalling(void)
{
	if (((GetTileMapCellCollisionCode(SpriteTilePositionBelowLeft.x, SpriteTilePositionBelowLeft.y) & (COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_LADDER)) != 0) ||
	    ((GetTileMapCellCollisionCode(SpriteTilePositionBelowRight.x, SpriteTilePositionBelowRight.y) & (COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_LADDER)) != 0))
	{
		SpriteFalling = 0;
		return;
	}

	SpriteFalling = 1;
	SpriteDirection.y = 1;
}

void CheckSpriteClimbing(void)
{
	if ((SpriteClimbing != 0 && SpriteDirection.y < 0) || (IntendedDirection.y < 0))
	{
		// Pressing up.

		// Check if sprite is touching a ladder.
		if ((SpriteCollisions[CurrentSpriteIndex] & COLLISION_FLAG_LADDER) != 0)
		{
			// Touching a ladder.
			SpriteClimbingAlignLadderTouching();
		}
		else
		{
			SpriteClimbing = 0;
			//SpriteDirection.y = 0;
		}
	}
	else if ((SpriteClimbing != 0 && SpriteDirection.y > 0) || (IntendedDirection.y > 0))
	{
		// Pressing down.

		byte belowTileCollisionBelowLeft = GetTileMapCellCollisionCode(SpriteTilePositionBelowLeft.x, SpriteTilePositionBelowLeft.y);
		byte belowTileCollisionBelowRight = GetTileMapCellCollisionCode(SpriteTilePositionBelowRight.x, SpriteTilePositionBelowRight.y);
		
		if (((belowTileCollisionBelowLeft & COLLISION_FLAG_OBSTACLE) != 0) &&
		    ((belowTileCollisionBelowRight & COLLISION_FLAG_OBSTACLE) != 0))
		{
			// Pressing down into a platform.
			SpriteClimbing = 0;
			SpriteDirection.y = 0;
		}
		else if (((belowTileCollisionBelowLeft & COLLISION_FLAG_LADDER) != 0) ||
		         ((belowTileCollisionBelowRight & COLLISION_FLAG_LADDER) != 0))
		{
			// Pressing down with ladder below.
			SpriteClimbingAlignLadderAbove();
		}
	}
	else if (SpriteClimbing != 0)
	{
		SpriteDirection.x = 0;
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
			SpriteDirection.x = -1;
		}
		else
		{
			SpriteDirection.x = 1;
		}
	}
	else
	{
		SpriteDirection.x = 0;
	}

	SpriteClimbing = 1;
	SpriteDirection.y = -1;
}

void SpriteClimbingAlignLadderAbove(void)
{
	word spriteX = GetSpritePositionX();
	
	if ((spriteX % 16) != 0)
	{
		if ((GetTileMapCellCollisionCode(spriteX / 8, SpriteTilePosition.y + 1) & COLLISION_FLAG_LADDER) != 0)
		{
			SpriteDirection.x = -1;
		}
		else
		{
			SpriteDirection.x = 1;
		}
	}
	else
	{
		SpriteDirection.x = 0;
	}

	SpriteClimbing = 1;
	SpriteDirection.y = 1;
}
