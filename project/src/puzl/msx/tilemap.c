#include <msx/video.h>

#include <stdio.h>
#include <string.h>

//#include <msx.h>

#include <puzl.h>

#include <msx/color.h>

int BackgroundId;

extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT][CHARACTER_WIDTH];

byte TileMapCollisionCodes[TILEMAP_WIDTH * TILEMAP_HEIGHT];

byte PrintX;
byte PrintY;
byte PrintColor;

byte GetTileMapShapeCode(byte x, byte y)
{
	return 0;
}

byte GetTileMapColorCode(byte x, byte y)
{
	return 0;
}

byte GetTileMapCellCollisionCode(byte x, byte y)
{
	return TileMapCollisionCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	
}

void SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{
	
}

void PrintText(const char* text)
{
	
}
