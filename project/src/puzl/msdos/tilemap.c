#include <msdos/video.h>

#include <stdio.h>
#include <string.h>

#include <puzl.h>

#include <c/tilemap.c>

extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT][CHARACTER_WIDTH];

void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode)
{
	const byte* characterOffset = (const byte*)&CharacterSet[shapeCode]; // NOTE: This only okay because it's a byte array?
	byte FAR* videoBufferOffset = &DoubleBuffer[(y * SCREEN_WIDTH) + x];

	unsigned int xIndex, yIndex;
	byte characterColorCode;

	for (yIndex = 0; yIndex < CHARACTER_HEIGHT; ++yIndex)
	{
		for (xIndex = 0; xIndex < CHARACTER_WIDTH; ++xIndex)
		{
			// TODO: Actually determine colorCode.
			characterColorCode = *characterOffset;
			if (characterColorCode != 0)
			{
				*videoBufferOffset = colorCode;
			}
			
			++videoBufferOffset;
			++characterOffset;
		}

		videoBufferOffset += SCREEN_WIDTH - TILE_WIDTH;
	}
}
