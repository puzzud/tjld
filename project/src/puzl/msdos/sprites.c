#include <msdos/video.h>

#include <string.h>

#include <puzl.h>

#include <c/sprites.c>

extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

void InitializeSprites(void)
{
	BaseInitializeSprites();
}

void DrawSprite(Sprite* sprite)
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
