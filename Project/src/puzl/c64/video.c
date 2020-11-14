#include <c64/video.h>

#include <stdio.h>

#include <color.h>

byte BackgroundColorCode;

byte* TileMapShapeCodes;
byte* TileMapColorCodes;

int InitializeVideo(void)
{
	// TODO: Expose this.
	BackgroundColorCode = COLOR_BLACK;

	return 0;
}

void InitializeTilemap(void)
{

}

void ShutdownVideo(void)
{
	
}

void ClearScreen(void)
{
	
}

byte GetTileMapShapeCode(byte x, byte y)
{
	return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

byte GetTileMapColorCode(byte x, byte y)
{
	return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCell(byte x, byte y, byte shapeCode, byte colorCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
