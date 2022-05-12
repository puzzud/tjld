#include <msdos/video.h>

#include <string.h>

#include <puzl.h>

#include <c/sprites.c>
#include <c/sprites_physics.c>
#include <c/sprites_animation.c>

extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

void DrawSprite(int spriteIndex);

void InitializeSprites(void)
{
	BaseInitializeSprites();
	BaseInitializeSpritesPhysics();
	BaseInitializeSpritesAnimation();
}

void DrawSprites(void)
{
	int spriteIndex = NUMBER_OF_SPRITES - 1;

	do
	{
		if (SpriteEnabledFlags[spriteIndex] != 0)
		{
			DrawSprite(spriteIndex);
		}
	}
	while (--spriteIndex > -1);
}

// NOTE: Had to make this function non-inline,
// because spriteIndex was resolving differently (irratically) than when it was a Sprite pointer.
void DrawSprite(int spriteIndex)
{
	const byte* spriteFrameOffset = SpriteSet[SpriteFrameIndices[spriteIndex]];
	ScreenPoint* spritePosition = &SpritePositions[spriteIndex];
	byte FAR* videoBufferOffset = &DoubleBuffer[(spritePosition->y * SCREEN_WIDTH) + spritePosition->x];

	byte colorCode;
	int yCounter = SPRITE_HEIGHT;
	int xCounter;

	do
	{
		xCounter = SPRITE_WIDTH;

		do
		{
			colorCode = *spriteFrameOffset;
			if (colorCode != 0)
			{
				switch (colorCode)
				{
					case 2:
					{
						colorCode = SpriteColorCodes[spriteIndex];

						break;
					}

					case 1:
					{
						colorCode = SpriteNonPrimaryColorCodes[0];

						break;
					}

					case 3:
					{
						colorCode = SpriteNonPrimaryColorCodes[1];

						break;
					}
				}

				*videoBufferOffset = colorCode;
			}

			++videoBufferOffset;
			++spriteFrameOffset;
		}
		while (--xCounter != 0);

		videoBufferOffset += SCREEN_WIDTH - SPRITE_WIDTH;
	}
	while (--yCounter != 0);
}

void EnableSprite(byte enable)
{
	SpriteEnabledFlags[CurrentSpriteIndex] = enable;
}

void SetSpritePosition(signed short x, signed short y)
{
	ScreenPoint* position = &SpritePositions[CurrentSpriteIndex];
	position->x = x;
	position->y = y;
}

void SetSpriteFrameIndex(byte frameIndex)
{
	SpriteFrameIndices[CurrentSpriteIndex] = frameIndex;
}

void SetSpriteColor(byte colorCode)
{
	SpriteColorCodes[CurrentSpriteIndex] = colorCode;
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[1] = colorCode;
}
