#include <nds/video.h>

#include <stdio.h>
#include <string.h>

#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/trig_lut.h>

#include <puzl.h>

#include <nds/color.h>

//#include <c/tilemap.c>

//extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT];

word BackgroundPalette[256];

byte BackgroundVideoBuffer[256 * 256];

//byte TileMapShapeCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];
//byte TileMapColorCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];
byte TileMapCollisionCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];

byte PrintX;
byte PrintY;
byte PrintColor;

//extern void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode);

void InitializeBackgroundPalette(void);

void InitializeTilemap(void)
{
	//int backgroundId;

	// Background.
	vramSetBankA(VRAM_A_MAIN_BG);
	BGCTRL[0] = BG_TILE_BASE(1) | BG_MAP_BASE(0) | BG_COLOR_256 | BG_32x32;
	//backgroundId = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	InitializeBackgroundPalette();

	// Background.
	//memset(BackgroundVideoBuffer, COLOR_RED, sizeof(BackgroundVideoBuffer));
	//dmaCopy(BackgroundVideoBuffer, bgGetGfxPtr(backgroundId), 256 * 256);

	memset(TileMapCollisionCodes, 0, sizeof(TileMapCollisionCodes));
}

void InitializeBackgroundPalette(void)
{
	unsigned int index;

	memset(BackgroundPalette, 0x0000, sizeof(BackgroundPalette));

	// Set first NUMBER_OF_COLORS to match ARGB values from engine colors.
	for (index = 0; index < NUMBER_OF_COLORS; ++index)
	{
		BackgroundPalette[index] = Colors[index];
	}

	dmaCopy(BackgroundPalette, BG_PALETTE, sizeof(BackgroundPalette));
}

void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode)
{
	
}

/*void DrawTileMap(void)
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
}*/

byte GetTileMapShapeCode(byte x, byte y)
{
	return 0;
	//return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

byte GetTileMapColorCode(byte x, byte y)
{
	return 0;
	//return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

byte GetTileMapCellCollisionCode(byte x, byte y)
{
	return TileMapCollisionCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	//const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	//TileMapShapeCodes[tileMapOffset] = shapeCode;
}

void SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	//const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	//TileMapColorCodes[tileMapOffset] = colorCode;
}

void SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapCollisionCodes[tileMapOffset] = collisionCode;
}

void PrintText(const char* text, byte x, byte y)
{
	/*
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
	}*/
}
