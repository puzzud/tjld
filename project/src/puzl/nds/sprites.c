#include <nds/video.h>

#include <string.h>

#include <puzl.h>

#include <c/sequencer.h>

#include <nds/color.h>

//extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

u16* SpriteGraphics;

Sprite Sprites[NUMBER_OF_SPRITES];
byte SpriteCollisionMasks[NUMBER_OF_SPRITES];
byte SpriteCollisions[NUMBER_OF_SPRITES];
byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void BaseInitializeSprites(void);

void ProcessSpriteAnimationDatum(unsigned int sequenceIndex, byte sequenceFetchDatum);

void CheckSpriteCollision(byte spriteIndex, ScreenPoint* tempSpritePosition);
void CalculateSpriteTileCorners(ScreenPoint* spritePosition, Vector2d* upperLeftSpriteTile, Vector2d* lowerRightSpriteTile);

void UpdateOam(unsigned int spriteIndex);

void SetSpritePaletteColumnColorCodes(unsigned int columnIndex, byte colorCode);

void BaseInitializeSprites(void)
{
	unsigned int index;

	// Sprite controls.
	memset(Sprites, 0, NUMBER_OF_SPRITES * sizeof(Sprite));

	// Initialize animation.
	// NOTE: This could just be referenced in an array at compile time.
	ProcessSequenceDatum[SEQUENCE_TYPE_ANIMATION] = &ProcessSpriteAnimationDatum;
	OnSequenceSegmentEnd[SEQUENCE_TYPE_ANIMATION] = NULL;

	for (index = 0; index < NUMBER_OF_SPRITES; ++index)
	{
		// Set all sprite animation IDs to -1.
		// NOTE: Hacky way to indicate initial unset
		// values for animation IDs, but it's fairly efficient
		// for 8 bit platform implementations.
		Sprites[index].animationId = -1;

		// Collision data.
		SpriteCollisionMasks[index] = 0;
		SpriteCollisions[index] = 0;
	}
}

void InitializeSprites(void)
{
	unsigned int spriteIndex;
	unsigned int i;

	BaseInitializeSprites();

	vramSetBankD(VRAM_D_SUB_SPRITE);
	oamInit(&oamMain, SpriteMapping_1D_32, 0);
	
	// Graphics.
	// TODO: Need to oamFreeGfx in a ShutdownSprites.
	SpriteGraphics = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
	for(i = 0; i < 16 * 16 / 2; i++)
	{
		//SpriteGraphics[i] = 1 | (1 << 8);
		SpriteGraphics[i] = (0x11 << 8) | 0x11;
	}

	for (spriteIndex = 0; spriteIndex < NUMBER_OF_SPRITES; ++spriteIndex)
	{
		UpdateOam(spriteIndex);
	}
}

void UpdateOam(unsigned int spriteIndex)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;

	oamSet
	(
		&oamMain,                  // Main graphics engine context.
		spriteIndex,               // OAM index (0 to 127).
		spritePosition->x,         // Pixel position X.
		spritePosition->y,         // Pixel position Y.
		0,                         // Render priority, lower renders last (on top).
		// TODO: Change from spriteIndex for when engine uses NES sprite palette like system.
		spriteIndex,               // Palette index (or alpha value for BMP).
		SpriteSize_16x16,          // Sprite size.
		SpriteColorFormat_16Color, // Color format.
		SpriteGraphics,            // Pointer to graphics.
		-1,                        // Affine index (for rotating).
		0,                         // Double size during rotation.
		sprite->enabled == 0 ? 1 : 0, // Hide sprite.
		0,                         // Vertical flip sprite.
		0,                         // Horizontal flip sprite.
		0	                         // Mosaic sprite.
	);
}

void EnableSprite(byte spriteIndex, byte enable)
{
	Sprites[spriteIndex].enabled = enable;
	
	// NOTE: oamSetHidden doesn't seem to work well,
	// so it seems full UpdateOam is needed.
	//oamSetHidden(&oamMain, spriteIndex, enable == 0 ? 1 : 0);
	
	UpdateOam(spriteIndex);
}

signed short GetSpritePositionX(byte spriteIndex)
{
	return Sprites[spriteIndex].position.x;
}

signed short GetSpritePositionY(byte spriteIndex)
{
	return Sprites[spriteIndex].position.y;
}

void SetSpritePosition(byte spriteIndex, signed short x, signed short y)
{
	ScreenPoint* position = &Sprites[spriteIndex].position;
	position->x = x;
	position->y = y;

	// NOTE: oamSetXY seems to work well; UpdateOam not needed!
	oamSetXY(&oamMain, spriteIndex, x, y);
	//UpdateOam(spriteIndex);
}

void SetSpriteVelocity(byte spriteIndex, signed char x, signed char y)
{
	Vector2d* velocity = &Sprites[spriteIndex].velocity;
	velocity->x = x;
	velocity->y = y;
}

void MoveSprite(byte spriteIndex)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;
	Vector2d* spriteVelocity = &sprite->velocity;

	static ScreenPoint tempSpritePosition;

	tempSpritePosition.x = spritePosition->x + spriteVelocity->x;
	tempSpritePosition.y = spritePosition->y + spriteVelocity->y;
	
	if (SpriteCollisionMasks[spriteIndex] != 0)
	{
		CheckSpriteCollision(spriteIndex, &tempSpritePosition);
	}

	SetSpritePosition(spriteIndex, tempSpritePosition.x, tempSpritePosition.y);
	//spritePosition->x = tempSpritePosition.x;
	//spritePosition->y = tempSpritePosition.y;
}

//--------------------------------------------------------------------------
// Checks for collision map overlap with temporary sprite position.
// Adjusts this position to original X or Y position, depending on velocity.
// Assumes sprite dimensions of 16x16.
void CheckSpriteCollision(byte spriteIndex, ScreenPoint* tempSpritePosition)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;
	Vector2d* spriteVelocity = &sprite->velocity;

	int x, y;

	Vector2d upperLeftSpriteTile;
	Vector2d lowerRightSpriteTile;

	CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);

	SpriteCollisions[spriteIndex] = 0;

	if ((SpriteCollisionMasks[spriteIndex] & COLLISION_FLAG_OBSTACLE) != 0)
	{
		if (spriteVelocity->x < 0)
		{
			// Upper left.
			// Lower left.
			if (((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->x = spritePosition->x;
				SpriteCollisions[spriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}
		else if (spriteVelocity->x > 0)
		{
			// Upper right.
			// Lower right.
			if (((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->x = spritePosition->x;
				SpriteCollisions[spriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}

		// TODO: Should be updated, but apparently doesn't matter much.
		//CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);

		if (spriteVelocity->y < 0)
		{
			// Upper left.
			// Upper right.
			if (((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->y = spritePosition->y;
				SpriteCollisions[spriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}
		else if (spriteVelocity->y > 0)
		{
			// Lower left.
			// Lower right.
			if (((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->y = spritePosition->y;
				SpriteCollisions[spriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}
	}

	if ((SpriteCollisionMasks[spriteIndex] & ~COLLISION_FLAG_OBSTACLE) != 0)
	{
		// TODO: Should be updated, but apparently doesn't matter much.
		//CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);

		// Cycle through all overlapped tiles and imprint their collision flags on
		// this sprite's collisions.
		for (y = upperLeftSpriteTile.y; y <= lowerRightSpriteTile.y; ++y)
		{
			for (x = upperLeftSpriteTile.x; x <= lowerRightSpriteTile.x; ++x)
			{
				SpriteCollisions[spriteIndex] |= GetTileMapCellCollisionCode(x, y);
			}	
		}
	}
}

inline void CalculateSpriteTileCorners(ScreenPoint* spritePosition, Vector2d* upperLeftSpriteTile, Vector2d* lowerRightSpriteTile)
{
	upperLeftSpriteTile->x = spritePosition->x / TILE_WIDTH;
	upperLeftSpriteTile->y = spritePosition->y / TILE_HEIGHT;

	// TODO: Need way to track sprite dimensions.
	lowerRightSpriteTile->x = (spritePosition->x + SPRITE_WIDTH - 1) / TILE_WIDTH;
	lowerRightSpriteTile->y = (spritePosition->y + SPRITE_HEIGHT - 1) / TILE_HEIGHT;	
}

void SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	Sprites[spriteIndex].frameIndex = frameIndex;
}

void SetSpriteColor(byte spriteIndex, byte colorCode)
{
	Sprites[spriteIndex].colorCode = colorCode;

	// TODO: 1 is probably not the right palette index.
	SPRITE_PALETTE[(spriteIndex * 16) + 1] = Colors[colorCode];
}

void SetSpritePaletteColumnColorCodes(unsigned int columnIndex, byte colorCode)
{
	unsigned int index;

	// TODO: When switching to NES like palette system,
	// this should no longer be done this way.
	for (index = 0; index < NUMBER_OF_SPRITES; ++index)
	{
		SPRITE_PALETTE[(index * 16) + columnIndex] = colorCode;
	}
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;

	SetSpritePaletteColumnColorCodes(2, colorCode);
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[1] = colorCode;

	SetSpritePaletteColumnColorCodes(3, colorCode);
}

void SetSpriteAnimationSet(byte spriteIndex, const byte** animationSet)
{
	Sprites[spriteIndex].animationSet = animationSet;
}

void PlaySpriteAnimation(byte spriteIndex, byte animationId, byte looping)
{
	const byte* animationStart;

	if (Sprites[spriteIndex].animationId == animationId)
	{
		return;
	}

	Sprites[spriteIndex].animationId = animationId;

	animationStart = Sprites[spriteIndex].animationSet[animationId];

	// TODO: Properly determine sequence from sprite index.
	PlaySequence(spriteIndex + 3, animationStart, looping);
}

void StopSpriteAnimation(byte spriteIndex)
{
	// TODO: Properly determine sequence from sprite index.
	StopSequence(spriteIndex + 3);
}

void ProcessSpriteAnimationDatum(unsigned int sequenceIndex, byte sequenceFetchDatum)
{
	byte spriteIndex = sequenceIndex - 3; // TODO: Properly determine sprite index from sequence index.

	// Cutoff bit 7.
	// The first seven bits of this byte are the animation frame index.
	SetSpriteFrameIndex(spriteIndex, sequenceFetchDatum & 0x7f); // %01111111

	// Now check bit 7.
	if ((sequenceFetchDatum & 0x80) != 0) // %10000000
	{
		// Fetch and store next byte.
		// Increase music pointer.
		SequenceSegmentDuration[sequenceIndex] = SequenceStart[sequenceIndex][++SequencePosition[sequenceIndex]];
	}

	SequenceSegmentDurationCounter[sequenceIndex] = SequenceSegmentDuration[sequenceIndex];
}
