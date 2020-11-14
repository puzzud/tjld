#include <c64/video.h>

#include <stdio.h>

#include <color.h>

char BackgroundColorCode;

char* TileMapShapeCodes;
char* TileMapColorCodes;

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

char GetTileMapShapeCode(unsigned int x, unsigned int y)
{
	return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

char GetTileMapColorCode(unsigned int x, unsigned int y)
{
	return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCell(unsigned int x, unsigned int y, char shapeCode, char colorCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
