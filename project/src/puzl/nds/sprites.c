#include <nds/video.h>

#include <string.h>

#include <puzl.h>

#include <c/sequencer.h>

#include <nds/color.h>

#define SPRITE_GRAPHICS_FRAME_LENGTH (SPRITE_WIDTH * SPRITE_HEIGHT * 2) // Not sure why 2 instead of 4.

extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

u16* SpriteGraphics;

Sprite Sprites[NUMBER_OF_SPRITES];

signed char SpriteVelocitiesX[NUMBER_OF_SPRITES];
signed char SpriteVelocitiesY[NUMBER_OF_SPRITES];

byte SpriteCollisionMasks[NUMBER_OF_SPRITES];
byte SpriteCollisions[NUMBER_OF_SPRITES];
byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void BaseInitializeSprites(void);
void InitializeSpriteGraphics(void);

void PopulateSpriteGraphicsFromSprite(unsigned int spriteFrameIndex);
void PopulateSpriteGraphicsCellFromSprite(unsigned int spriteFrameIndex, unsigned int cellXOffset, unsigned int cellYOffset);

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
	memset(SpriteVelocitiesX, 0, sizeof(SpriteVelocitiesX));
	memset(SpriteVelocitiesY, 0, sizeof(SpriteVelocitiesY));

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

	BaseInitializeSprites();

	vramSetBankD(VRAM_D_SUB_SPRITE);
	oamInit(&oamMain, SpriteMapping_1D_32, 0);
	
	InitializeSpriteGraphics();

	for (spriteIndex = 0; spriteIndex < NUMBER_OF_SPRITES; ++spriteIndex)
	{
		Sprites[spriteIndex].spriteGraphicsOffset = SpriteGraphics;

		UpdateOam(spriteIndex);
	}
}

void InitializeSpriteGraphics(void)
{
	unsigned int spriteFrameIndex;

	SpriteGraphics = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);

	for (spriteFrameIndex = 0; spriteFrameIndex < NUMBER_OF_SPRITE_FRAMES; ++spriteFrameIndex)
	{
		PopulateSpriteGraphicsFromSprite(spriteFrameIndex);
	}
}

void PopulateSpriteGraphicsFromSprite(unsigned int spriteFrameIndex)
{
	PopulateSpriteGraphicsCellFromSprite(spriteFrameIndex, 0, 0);
	PopulateSpriteGraphicsCellFromSprite(spriteFrameIndex, 1, 0);
	PopulateSpriteGraphicsCellFromSprite(spriteFrameIndex, 0, 1);
	PopulateSpriteGraphicsCellFromSprite(spriteFrameIndex, 1, 1);
}

void PopulateSpriteGraphicsCellFromSprite(unsigned int spriteFrameIndex, unsigned int cellXOffset, unsigned int cellYOffset)
{
	unsigned int targetGraphicsIndex =
		(spriteFrameIndex * SPRITE_GRAPHICS_FRAME_LENGTH) +
		(cellYOffset * 32) +
		(cellXOffset * SPRITE_WIDTH);

	unsigned int x, y;
	unsigned int spriteSetCellX, spriteSetCellY;
	byte colorCode;
	word targetColorDatum;

	const unsigned int cellOffsetMultiplier = 8;

	spriteSetCellX = (cellXOffset * cellOffsetMultiplier);
	spriteSetCellY = (cellYOffset * cellOffsetMultiplier);

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8;)
		{
			colorCode = SpriteSet[spriteFrameIndex][spriteSetCellY + y][spriteSetCellX + x++];
			targetColorDatum = colorCode;

			colorCode = SpriteSet[spriteFrameIndex][spriteSetCellY + y][spriteSetCellX + x++];
			targetColorDatum |= colorCode << 4;

			colorCode = SpriteSet[spriteFrameIndex][spriteSetCellY + y][spriteSetCellX + x++];
			targetColorDatum |= colorCode << 8;

			colorCode = SpriteSet[spriteFrameIndex][spriteSetCellY + y][spriteSetCellX + x++];
			targetColorDatum |= colorCode << 12;

			SpriteGraphics[targetGraphicsIndex++] = targetColorDatum;
		}
	}
}

void ShutdownSprites(void)
{
	oamFreeGfx(&oamMain, SpriteGraphics);
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
		sprite->spriteGraphicsOffset, // Pointer to graphics.
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

void MoveSprite(byte spriteIndex)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;

	static ScreenPoint tempSpritePosition;

	tempSpritePosition.x = spritePosition->x + SpriteVelocitiesX[spriteIndex];
	tempSpritePosition.y = spritePosition->y + SpriteVelocitiesY[spriteIndex];
	
	if (SpriteCollisionMasks[spriteIndex] != 0)
	{
		CheckSpriteCollision(spriteIndex, &tempSpritePosition);
	}

	SetSpritePosition(spriteIndex, tempSpritePosition.x, tempSpritePosition.y);
}

//--------------------------------------------------------------------------
// Checks for collision map overlap with temporary sprite position.
// Adjusts this position to original X or Y position, depending on velocity.
// Assumes sprite dimensions of 16x16.
void CheckSpriteCollision(byte spriteIndex, ScreenPoint* tempSpritePosition)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;
	Vector2d spriteVelocity;

	int x, y;

	Vector2d upperLeftSpriteTile;
	Vector2d lowerRightSpriteTile;

	spriteVelocity.x = SpriteVelocitiesX[spriteIndex];
	spriteVelocity.y = SpriteVelocitiesY[spriteIndex];

	CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);

	SpriteCollisions[spriteIndex] = 0;

	if ((SpriteCollisionMasks[spriteIndex] & COLLISION_FLAG_OBSTACLE) != 0)
	{
		if (spriteVelocity.x < 0)
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
		else if (spriteVelocity.x > 0)
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

		if (spriteVelocity.y < 0)
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
		else if (spriteVelocity.y > 0)
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

	Sprites[spriteIndex].spriteGraphicsOffset = &SpriteGraphics[SPRITE_GRAPHICS_FRAME_LENGTH * frameIndex];

	UpdateOam(spriteIndex);
}

void SetSpriteColor(byte spriteIndex, byte colorCode)
{
	Sprites[spriteIndex].colorCode = colorCode;
	
	SPRITE_PALETTE[(spriteIndex * 16) + 2] = Colors[colorCode];
}

void SetSpritePaletteColumnColorCodes(unsigned int columnIndex, byte colorCode)
{
	unsigned int index;

	// TODO: When switching to NES like palette system,
	// this should no longer be done this way.
	for (index = 0; index < NUMBER_OF_SPRITES; ++index)
	{
		SPRITE_PALETTE[(index * 16) + columnIndex] = Colors[colorCode];
	}
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;

	SetSpritePaletteColumnColorCodes(1, colorCode);
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
