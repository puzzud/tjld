#include <c64/video.h>

#include <puzl.h>
#include <c64/c64.h>
#include <c64/core.h>

#include <stdio.h>

void SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(BGCOL0, color);
}

void SetSpritePosition(signed short x, signed short y)
{
	SetSpritePositionX(x);
	SetSpritePositionY(y);
}

void ClearTileMapCollisionCodes(void)
{
	unsigned int index;

	for (index = 0; index < SCREEN_CHAR_SIZE; ++index)
	{
		TileMapCollisionCodes[index] = 0;
	}
}
