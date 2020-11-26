#include <c64/video.h>

#include <puzl.h>
#include <core.h>

#include <c64/c64.h>
#include <stdio.h>

#include <color.h>

void SetBackgroundColor(byte color)
{
	SET_MEMORY_BYTE(VIC_BG_COLOR0, color)
	SET_MEMORY_BYTE(VIC_BORDERCOLOR, 0) // TODO: Temporary border black.
}

byte GetTileMapShapeCode(byte x, byte y)
{
	const word cellMemoryAddress = (word)(SCREEN_CHAR + (y * TILEMAP_WIDTH) + x);

	return GET_MEMORY_BYTE(cellMemoryAddress);
}

byte FASTCALL GetTileMapColorCode(byte x, byte y)
{
	const word cellMemoryAddress = (word)(SCREEN_COLOR + (y * TILEMAP_WIDTH) + x);

	return GET_MEMORY_BYTE(cellMemoryAddress);
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
		SET_MEMORY_BYTE(VIC_SPR_ENA, GET_MEMORY_BYTE(VIC_SPR_ENA) | NthBitFlag[spriteIndex]);
	}
	else
	{
		SET_MEMORY_BYTE(VIC_SPR_ENA, GET_MEMORY_BYTE(VIC_SPR_ENA) & ~NthBitFlag[spriteIndex]);
	}

	SET_MEMORY_BYTE(VIC_SPR_MCOLOR, 0xff); // TODO: Do this separately.
}

void SetSpritePosition(byte spriteIndex, unsigned short x, unsigned char y)
{
	word memoryAddress = (word)(VIC_SPR0_X + (spriteIndex * 2));
	
	x += 24;
	*((unsigned char*)memoryAddress) = x;
	if (x > 255)
	{
		SET_MEMORY_BYTE(VIC_SPR_HI_X, GET_MEMORY_BYTE(VIC_SPR_HI_X) | NthBitFlag[spriteIndex]);
	}
	else
	{
		SET_MEMORY_BYTE(VIC_SPR_HI_X, GET_MEMORY_BYTE(VIC_SPR_HI_X) & ~NthBitFlag[spriteIndex]);
	}

	memoryAddress = (word)(VIC_SPR0_Y + (spriteIndex * 2));
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
	const word memoryAddress = (word)(VIC_SPR0_COLOR + spriteIndex);
	SET_MEMORY_BYTE(memoryAddress, colorCode);
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SET_MEMORY_BYTE(VIC_SPR_MCOLOR0, colorCode);
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SET_MEMORY_BYTE(VIC_SPR_MCOLOR1, colorCode);
}
