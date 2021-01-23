#include <nds/video.h>

#include <stdio.h>
#include <string.h>

#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/trig_lut.h>

#include <puzl.h>

#include <nds/color.h>

int BackgroundId;

extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT];
byte TilesetData[NUMBER_OF_CHARACTERS * CHARACTER_HEIGHT * 4]; // 4 => 4bpp.

u16 BackgroundPalette[256];

word TilemapShapeCodes[1024];

byte TileMapCollisionCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];

byte PrintX;
byte PrintY;
byte PrintColor;

void DrawCharacterToTilesetData(byte shapeCode);

void InitializeBackgroundPalette(void);
void InitializeTilemapGraphics(void);
void InitializeTilemapShapeCodes(void);

void InitializeTilemap(void)
{
	// Background.
	vramSetBankA(VRAM_A_MAIN_BG);
	BackgroundId = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 0, 1);

	InitializeBackgroundPalette();
	InitializeTilemapGraphics();

	InitializeTilemapShapeCodes();

	memset(TileMapCollisionCodes, 0, sizeof(TileMapCollisionCodes));
}

void InitializeBackgroundPalette(void)
{
	unsigned int rowIndex;
	unsigned int index;

	//memset(BackgroundPalette, Colors[COLOR_BLACK], sizeof(BackgroundPalette));

	// Set first NUMBER_OF_COLORS to match ARGB values from engine colors.
	for (rowIndex = 0; rowIndex < NUMBER_OF_COLORS; ++rowIndex)
	{
		for (index = 0; index < NUMBER_OF_COLORS; ++index)
		{
			// NOTE: Currently, each row represents a single color
			// (with understanding that 0th column will represent transparency).
			// TODO: Switch to use actual palettes when all implementations use a
			// NES like palette system.
			BackgroundPalette[(rowIndex * NUMBER_OF_COLORS) + index] = Colors[rowIndex];//Colors[index];
		}
	}

	dmaCopy(BackgroundPalette, BG_PALETTE, sizeof(BackgroundPalette));
}

void InitializeTilemapGraphics(void)
{
	unsigned int characterIndex;

	for (characterIndex = 0; characterIndex < NUMBER_OF_CHARACTERS; ++characterIndex)
	{
		DrawCharacterToTilesetData(characterIndex);
	}

	dmaCopy((void*)&TilesetData, (void*)bgGetGfxPtr(BackgroundId), sizeof(TilesetData));
}

void DrawCharacterToTilesetData(byte shapeCode)
{
	unsigned int y;
	unsigned int rowPixels;
	unsigned int pixelIndex;
	byte colorCode;
	byte rowColorCodes[8];

	unsigned int charBaseBlockDataIndex = ((TILE_HEIGHT * 4) * shapeCode); // 4 => 4bpp.

	for (y = 0; y < TILE_HEIGHT; ++y)
	{
		rowPixels = CharacterSet[shapeCode][y];
		
		for (pixelIndex = 0; pixelIndex < 8; ++pixelIndex)
		{
			rowColorCodes[pixelIndex] = (rowPixels & 0x80) != 0 ? 1 : 0;
			rowPixels <<= 1;
		}

		for (pixelIndex = 0; pixelIndex < 8;)
		{
			colorCode = rowColorCodes[pixelIndex++];
			colorCode |= rowColorCodes[pixelIndex++] << 4;
			
			TilesetData[charBaseBlockDataIndex] = colorCode;

			++charBaseBlockDataIndex;
		}
	}
}

void InitializeTilemapShapeCodes(void)
{
	memset((void*)bgGetMapPtr(BackgroundId), 0x0000, (TILEMAP_WIDTH * TILEMAP_HEIGHT) / 2); 
}

byte GetTileMapShapeCode(byte x, byte y)
{
	word* screenBaseBlockData = (word*)bgGetMapPtr(BackgroundId);
	return screenBaseBlockData[(y * TILEMAP_WIDTH) + x] & 0x00ff;
}

byte GetTileMapColorCode(byte x, byte y)
{
	word* screenBaseBlockData = (word*)bgGetMapPtr(BackgroundId);
	return (screenBaseBlockData[(y * TILEMAP_WIDTH) + x] & 0xf000) >> 12;
}

byte GetTileMapCellCollisionCode(byte x, byte y)
{
	return TileMapCollisionCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	word* screenBaseBlockData = (word*)bgGetMapPtr(BackgroundId);
	unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;
	word cellDatum = screenBaseBlockData[tileMapOffset];

	// Clear lower 11 bits while retaining upper 4 of current cellDatum for this cell.
	// Overlay shapeCode over the lower 11 bits.
	screenBaseBlockData[tileMapOffset] = (cellDatum & 0xf300) | shapeCode;
}

void SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	word* screenBaseBlockData = (word*)bgGetMapPtr(BackgroundId);
	unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;
	word cellDatum = screenBaseBlockData[tileMapOffset];

	// Clear upper 4 bits while retaining lower 11 of current cellDatum for this cell.
	// Overlay colorCode over the upper 4 bits.
	screenBaseBlockData[tileMapOffset] = (colorCode << 12) | (cellDatum & 0x0fff); // 12th bit starts color palette index.
}

void SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapCollisionCodes[tileMapOffset] = collisionCode;
}

void PrintText(const char* text)
{
	word* screenBaseBlockData = (word*)bgGetMapPtr(BackgroundId);
	const unsigned int tileMapOffset = (PrintY * TILEMAP_WIDTH) + PrintX;
	
	word paletteBankOverlay = (PrintColor << 12);

	byte characterCode;
	int charIndex = 0;

	while (1)
	{
		characterCode = text[charIndex];
		if (characterCode == 0)
		{
			break;
		}

		// NOTE: This assumes flipping flags should be clear.
		// NOTE: PrintColor shifting could be optimized out.
		screenBaseBlockData[tileMapOffset + charIndex] = paletteBankOverlay | characterCode;

		++charIndex;
	}

	PrintX += charIndex;
}
