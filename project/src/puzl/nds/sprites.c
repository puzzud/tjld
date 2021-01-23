#include <nds/video.h>

#include <string.h>

#include <puzl.h>

#include <nds/color.h>

#include <c/sprites.c>
#include <c/sprites_physics.c>
#include <c/sprites_animation.c>

#define SPRITE_GRAPHICS_FRAME_LENGTH (SPRITE_WIDTH * SPRITE_HEIGHT * 2) // Not sure why 2 instead of 4.

extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

u16* SpriteGraphics;

void InitializeSpriteGraphics(void);

void PopulateSpriteGraphicsFromSprite(unsigned int spriteFrameIndex);
void PopulateSpriteGraphicsCellFromSprite(unsigned int spriteFrameIndex, unsigned int cellXOffset, unsigned int cellYOffset);

void UpdateOam(unsigned int spriteIndex);

void SetSpritePaletteColumnColorCodes(unsigned int columnIndex, byte colorCode);

void InitializeSprites(void)
{
	unsigned int spriteIndex;

	BaseInitializeSprites();
	BaseInitializeSpritesPhysics();
	BaseInitializeSpritesAnimation();

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

void EnableSprite(byte enable)
{
	Sprites[CurrentSpriteIndex].enabled = enable;
	
	// NOTE: oamSetHidden doesn't seem to work well,
	// so it seems full UpdateOam is needed.
	//oamSetHidden(&oamMain, CurrentSpriteIndex, enable == 0 ? 1 : 0);
	
	UpdateOam(CurrentSpriteIndex);
}

void SetSpritePosition(signed short x, signed short y)
{
	ScreenPoint* position = &Sprites[CurrentSpriteIndex].position;
	position->x = x;
	position->y = y;

	// NOTE: oamSetXY seems to work well; UpdateOam not needed!
	oamSetXY(&oamMain, CurrentSpriteIndex, x, y);
	//UpdateOam(CurrentSpriteIndex);
}

void SetSpriteFrameIndex(byte frameIndex)
{
	Sprites[CurrentSpriteIndex].frameIndex = frameIndex;

	Sprites[CurrentSpriteIndex].spriteGraphicsOffset = &SpriteGraphics[SPRITE_GRAPHICS_FRAME_LENGTH * frameIndex];

	UpdateOam(CurrentSpriteIndex);
}

void SetSpriteColor(byte colorCode)
{
	Sprites[CurrentSpriteIndex].colorCode = colorCode;
	
	SPRITE_PALETTE[(CurrentSpriteIndex * 16) + 2] = Colors[colorCode];
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
