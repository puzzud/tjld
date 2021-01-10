#include <msdos/video.h>

#include <stdio.h>
#include <string.h>

#include <puzl.h>

byte TileMapShapeCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];
byte TileMapColorCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];
byte TileMapCollisionCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];

extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT];

byte PrintX;
byte PrintY;
byte PrintColor;

void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode);

void InitializeTilemap(void)
{
	memset(TileMapShapeCodes, 0, sizeof(TileMapShapeCodes));
	memset(TileMapColorCodes, 0, sizeof(TileMapColorCodes));
	memset(TileMapCollisionCodes, 0, sizeof(TileMapCollisionCodes));
}

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

void DrawTileMap()
{
	byte shapeCode;
	byte colorCode;

	unsigned int columnIndex;
	unsigned int rowIndex;

	for (rowIndex = 0; rowIndex < TILEMAP_HEIGHT; ++rowIndex)
	{
		for (columnIndex = 0; columnIndex < TILEMAP_WIDTH; ++columnIndex)
		{
			shapeCode = TileMapShapeCodes[(rowIndex * TILEMAP_WIDTH) + columnIndex];
			if (shapeCode != 0)
			{
				colorCode = TileMapColorCodes[(rowIndex * TILEMAP_WIDTH) + columnIndex];
				DrawCharacter(columnIndex * TILE_WIDTH, rowIndex * TILE_HEIGHT, shapeCode, colorCode);
			}
		}
	}
}

byte FASTCALL GetTileMapShapeCode(byte x, byte y)
{
	return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

byte FASTCALL GetTileMapColorCode(byte x, byte y)
{
	return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

byte FASTCALL GetTileMapCellCollisionCode(byte x, byte y)
{
	return TileMapCollisionCodes[(y * TILEMAP_WIDTH) + x];
}

void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
}

void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapColorCodes[tileMapOffset] = colorCode;
}

void FASTCALL SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapCollisionCodes[tileMapOffset] = collisionCode;
}

void FASTCALL PrintText(const char* text, byte x, byte y)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;
	
	byte characterCode;
	int charIndex = 0;

	while (1)
	{
		characterCode = text[charIndex];
		if (characterCode == 0)
		{
			break;
		}

		TileMapShapeCodes[tileMapOffset + charIndex] = characterCode;
		TileMapColorCodes[tileMapOffset + charIndex] = PrintColor;

		++charIndex;
	}
}
