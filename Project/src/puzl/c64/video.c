#include <c64/video.h>

#include <puzl.h>
#include <c64/c64.h>
#include <c64/core.h>

#include <stdio.h>

#include <color.h>

void SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(BGCOL0, color);
}

byte GetTileMapShapeCode(byte x, byte y)
{
	const word cellMemoryAddress = (word)(SCREEN_CHAR + (y * TILEMAP_WIDTH) + x);

	return GET_MEMORY_BYTE(cellMemoryAddress);
}

byte FASTCALL GetTileMapColorCode(byte x, byte y)
{
	const word cellMemoryAddress = (word)(SCREEN_COLOR + (y * TILEMAP_WIDTH) + x);

	return GET_MEMORY_BYTE(cellMemoryAddress) & 0x0f; // Masked because upper nibble can be random.
}

void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	const word cellMemoryAddress = (word)(SCREEN_CHAR + (y * TILEMAP_WIDTH) + x);

	SET_MEMORY_BYTE(cellMemoryAddress, shapeCode)
}

void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	const word cellMemoryAddress = (word)(SCREEN_COLOR + (y * TILEMAP_WIDTH) + x);

	SET_MEMORY_BYTE(cellMemoryAddress, colorCode);
}

void EnableSprite(byte spriteIndex, byte enable)
{
	if (enable != 0)
	{
		SET_MEMORY_BYTE(SPENA, GET_MEMORY_BYTE(SPENA) | NthBitFlag[spriteIndex]);
	}
	else
	{
		SET_MEMORY_BYTE(SPENA, GET_MEMORY_BYTE(SPENA) & ~NthBitFlag[spriteIndex]);
	}
}

void SetSpritePosition(byte spriteIndex, unsigned short x, unsigned char y)
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

void SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	const word memoryAddress = (word)(SPRITE_POINTER_BASE + spriteIndex);

	// TODO: Need to investigate why 64 is a base.
	SET_MEMORY_BYTE(memoryAddress, 64 + frameIndex);
}

void SetSpriteColor(byte spriteIndex, byte colorCode)
{
	const word memoryAddress = (word)(SP0COL + spriteIndex);
	SET_MEMORY_BYTE(memoryAddress, colorCode);
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SET_MEMORY_BYTE(SPMC0, colorCode);
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SET_MEMORY_BYTE(SPMC1, colorCode);
}
