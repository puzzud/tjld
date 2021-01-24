#include <msdos/video.h>

#include <stdio.h>
#include <string.h>

#include <puzl.h>

#include <c/tilemap.c>

extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT][CHARACTER_WIDTH];

void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode)
{
	const byte* characterOffset = &CharacterSet[shapeCode];
	byte FAR* videoBufferOffset = &DoubleBuffer[(y * SCREEN_WIDTH) + x];

	int yCounter = TILE_HEIGHT;
	int xCounter;
	byte rowPixels;

	do
	{
		xCounter = TILE_WIDTH;

		rowPixels = *characterOffset;
		if (rowPixels != 0)
		{
			do
			{
				if ((rowPixels & 0x80) != 0)
				{
					*videoBufferOffset = colorCode;
				}

				++videoBufferOffset;

				rowPixels <<= 1;
			}
			while (--xCounter != 0);

			videoBufferOffset += SCREEN_WIDTH - TILE_WIDTH;
		}
		else
		{
			videoBufferOffset += SCREEN_WIDTH;
		}

		++characterOffset;
	}
	while (--yCounter != 0);
}
