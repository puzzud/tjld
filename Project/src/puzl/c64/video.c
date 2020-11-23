#include <c64/video.h>

#include <puzl.h>

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
	SET_MEMORY_BYTE(VIC_SPR_ENA, 0xff);

	// # TODO: Some strange bug related to when spriteIndex is 0.
	/*
	byte enabledSpriteFlags = GET_MEMORY_BYTE(VIC_SPR_ENA);
	
	if (enable != 0)
	{
		SET_MEMORY_BYTE(VIC_SPR_ENA, enabledSpriteFlags | (1 << spriteIndex));
	}
	else
	{
		SET_MEMORY_BYTE(VIC_SPR_ENA, enabledSpriteFlags & ~(1 << spriteIndex));
	}
	*/

	SET_MEMORY_BYTE(VIC_SPR_MCOLOR, 0xff); // TODO: Do this separately.
}

void SetSpritePosition(byte spriteIndex, unsigned short x, unsigned short y)
{
	word memoryAddress;
	
	memoryAddress = (word)(VIC_SPR0_X + (spriteIndex * 2));
	SET_MEMORY_BYTE(memoryAddress, 24 + x);

	memoryAddress = (word)(VIC_SPR0_Y + (spriteIndex * 2));
	SET_MEMORY_BYTE(memoryAddress, 50 + y);
	
	memoryAddress = (word)(VIC_SPR_HI_X);
	SET_MEMORY_BYTE(memoryAddress, 0); // TODO: Impelement this logic.
}

void SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	const word memoryAddress = (word)(SPRITE_POINTER_BASE + spriteIndex);

	// TODO: Need to investigate why 64 is a base.
	SET_MEMORY_BYTE(memoryAddress, 64 + frameIndex);
}
