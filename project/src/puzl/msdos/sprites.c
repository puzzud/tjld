#include <msdos/video.h>

#include <string.h>

#include <puzl.h>

#include <c/sprites.c>
#include <c/sprites_physics.c>
#include <c/sprites_animation.c>

extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

void DrawSprite(Sprite* sprite);

void InitializeSprites(void)
{
	BaseInitializeSprites();
	BaseInitializeSpritesPhysics();
	BaseInitializeSpritesAnimation();
}

void DrawSprites(void)
{
	Sprite* sprite;
	int spriteIndex = NUMBER_OF_SPRITES - 1;

	do
	{
		sprite = &Sprites[spriteIndex];

		if (sprite->enabled != 0)
		{
			DrawSprite(sprite);
		}
	}
	while (--spriteIndex > -1);
}

inline void DrawSprite(Sprite* sprite)
{
	const byte* spriteFrameOffset = SpriteSet[sprite->frameIndex];
	byte FAR* videoBufferOffset = &DoubleBuffer[(sprite->position.y * SCREEN_WIDTH) + sprite->position.x];

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
						colorCode = sprite->colorCode;

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
	Sprites[CurrentSpriteIndex].enabled = enable;
}

void SetSpritePosition(signed short x, signed short y)
{
	ScreenPoint* position = &Sprites[CurrentSpriteIndex].position;
	position->x = x;
	position->y = y;
}

void SetSpriteFrameIndex(byte frameIndex)
{
	Sprites[CurrentSpriteIndex].frameIndex = frameIndex;
}

void SetSpriteColor(byte colorCode)
{
	Sprites[CurrentSpriteIndex].colorCode = colorCode;
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[1] = colorCode;
}
