#include <c64/video.h>

#include <puzl.h>

#include <c64/c64.h>
#include <stdio.h>

#include <color.h>

void FASTCALL SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(VIC_BG_COLOR0, color)
	SET_MEMORY_BYTE(VIC_BORDERCOLOR, 0) // TODO: Temporary border black.
}

byte GetTileMapShapeCode(byte x, byte y)
{
	const word cellMemoryAddress = (word)(SCREEN_CHAR + (y * TILEMAP_WIDTH) + x);

	return GET_MEMORY_BYTE(cellMemoryAddress);
}

byte GetTileMapColorCode(byte x, byte y)
{
	const word cellMemoryAddress = (word)(SCREEN_COLOR + (y * TILEMAP_WIDTH) + x);

	return GET_MEMORY_BYTE(cellMemoryAddress);
}

void SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	const word cellMemoryAddress = (word)(SCREEN_CHAR + (y * TILEMAP_WIDTH) + x);

	SET_MEMORY_BYTE(cellMemoryAddress, shapeCode)
}

void SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	const word cellMemoryAddress = (word)(SCREEN_COLOR + (y * TILEMAP_WIDTH) + x);

	SET_MEMORY_BYTE(cellMemoryAddress, colorCode);
}
