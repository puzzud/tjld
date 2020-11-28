#include <c64/video.h>

#include <puzl.h>
#include <c64/c64.h>
#include <c64/core.h>

#include <stdio.h>

#include <color.h>

// TODO: Should probably clear these values.
signed char SpriteVelocitiesX[NUMBER_OF_SPRITES];
signed char SpriteVelocitiesY[NUMBER_OF_SPRITES];

void SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(BGCOL0, color);
}

signed short GetSpritePositionX(byte spriteIndex)
{
	word memoryAddress = (word)(SP0X + (spriteIndex * 2));

	unsigned short positionX = GET_MEMORY_BYTE(memoryAddress);

	if ((GET_MEMORY_BYTE(MSIGX) & NthBitFlag[spriteIndex]) != 0)
	{
		return positionX + 256 - 24;
	}

	return positionX - 24;
}

signed short GetSpritePositionY(byte spriteIndex)
{
	word memoryAddress = (word)(SP0Y + (spriteIndex * 2));
	return GET_MEMORY_BYTE(memoryAddress) - 50;
}

void SetSpritePosition(byte spriteIndex, signed short x, signed short y)
{
	word memoryAddress = (word)(SP0X + (spriteIndex * 2));
	
	x += 24;
	*((unsigned char*)memoryAddress) = x;
	if (x > 255)
	{
		SET_MEMORY_BYTE(MSIGX, GET_MEMORY_BYTE(MSIGX) | NthBitFlag[spriteIndex]);
	}
	else
	{
		SET_MEMORY_BYTE(MSIGX, GET_MEMORY_BYTE(MSIGX) & ~NthBitFlag[spriteIndex]);
	}

	memoryAddress = (word)(SP0Y + (spriteIndex * 2));
	SET_MEMORY_BYTE(memoryAddress, y + 50);
}

void SetSpriteVelocity(byte spriteIndex, signed char x, signed char y)
{
	SpriteVelocitiesX[spriteIndex] = x;
	SpriteVelocitiesY[spriteIndex] = y;
}

void MoveSprite(byte spriteIndex)
{
	SetSpritePosition
		(
			spriteIndex,
			GetSpritePositionX(spriteIndex) + SpriteVelocitiesX[spriteIndex],
			GetSpritePositionY(spriteIndex) + SpriteVelocitiesY[spriteIndex]
		);
}
