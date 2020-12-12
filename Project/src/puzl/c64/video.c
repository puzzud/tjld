#include <c64/video.h>

#include <puzl.h>
#include <c64/c64.h>
#include <c64/core.h>

#include <stdio.h>

#include <color.h>

// TODO: Should probably clear these values.
signed short SpritePositionsX[NUMBER_OF_SPRITES];
signed short SpritePositionsY[NUMBER_OF_SPRITES];

signed char SpriteVelocitiesX[NUMBER_OF_SPRITES];
signed char SpriteVelocitiesY[NUMBER_OF_SPRITES];

void SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(BGCOL0, color);
}

void SetSpritePosition(byte spriteIndex, signed short x, signed short y)
{
	SetSpritePositionX(spriteIndex, x);
	SetSpritePositionY(spriteIndex, y);
}

byte GetTileMapCellCollisionCode(byte x, byte y)
{
	return 0;
}

void SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{
	
}
