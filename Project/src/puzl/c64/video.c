#include <c64/video.h>

#include <puzl.h>

#include <c64/c64.h>
#include <stdio.h>

#include <color.h>

byte* TileMapShapeCodes;
byte* TileMapColorCodes;

int InitializeVideo(void)
{
	return 0;
}

void InitializeTilemap(void)
{

}

void ShutdownVideo(void)
{
	
}

void FASTCALL SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(VIC_BG_COLOR0, color)
	SET_MEMORY_BYTE(VIC_BORDERCOLOR, color)
}

byte GetTileMapShapeCode(byte x, byte y)
{
	return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

byte GetTileMapColorCode(byte x, byte y)
{
	return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	//TileMapShapeCodes[tileMapOffset] = shapeCode;
}

void SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	//TileMapColorCodes[tileMapOffset] = colorCode;
}
