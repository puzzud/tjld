#include <c64/video.h>

#include <stdio.h>

#include <color.h>

Point ScreenDimensions;

char BackgroundColorCode;

Point TileDimensions;

Point TileMapDimensions;

char* TileMapShapeCodes;
char* TileMapColorCodes;

int InitializeVideo(void)
{
	// TODO: Expose this.
	BackgroundColorCode = COLOR_BLACK;

	ScreenDimensions.x = 320;
	ScreenDimensions.y = 200;

	TileDimensions.x = 8;
	TileDimensions.y = 8;

	return 0;
}

void InitializeTilemap(void)
{
	
}

void ShutdownVideo(void)
{
	
}
/*
void Draw(void)
{
	ClearScreen();

	//DrawTileMap();

	SDL_RenderPresent(Renderer);
}
*/
void ClearScreen(void)
{
	
}

char GetTileMapShapeCode(unsigned int x, unsigned int y)
{
	return TileMapShapeCodes[(y * TileMapDimensions.x) + x];
}

char GetTileMapColorCode(unsigned int x, unsigned int y)
{
	return TileMapColorCodes[(y * TileMapDimensions.x) + x];
}

void SetTileMapCell(unsigned int x, unsigned int y, char shapeCode, char colorCode)
{
	const unsigned int tileMapOffset = (y * TileMapDimensions.x) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
