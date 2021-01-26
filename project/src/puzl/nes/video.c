#include <nes/video.h>

#include <puzl.h>
#include <nes/nes.h>
#include <nes/core.h>

#include <stdio.h>

#define PALETTE_SIZE (NUMBER_OF_COLORS_PER_PALETTE_ENTRY * NUMBER_OF_ENTRIES_PER_PALETTE)

enum PaletteUpdateMode
{
  PALETTE_UPDATE_MODE_ALL = 0,
  PALETTE_UPDATE_MODE_CHARACTER,
  PALETTE_UPDATE_MODE_SPRITE
};

byte CharacterPalette[PALETTE_SIZE];
byte SpritePalette[PALETTE_SIZE];

//extern byte PrintColorSet;
//#pragma zpsym ("PrintColorSet")

extern byte UpdatePaletteFlag;
#pragma zpsym ("UpdatePaletteFlag")

byte VideoIndex;

void FASTCALL UpdatePalette(byte id);

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
		//TileMapCollisionCodes[index] = 0;
	}
}

void FASTCALL DisableVideo(void)
{
  // Turn off the screen.
  SET_MEMORY_BYTE(PPU_CTRL1, 0);
	SET_MEMORY_BYTE(PPU_CTRL2, 0);
}

void FASTCALL EnableVideo(void)
{
  SET_MEMORY_BYTE(PPU_VRAM_ADDR1, 0x00);
  SET_MEMORY_BYTE(PPU_VRAM_ADDR1, 0x00);
  SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0x00);
  SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0x00);
  
  // Turn on screen.
  SET_MEMORY_BYTE(PPU_CTRL1, 0x90);
  SET_MEMORY_BYTE(PPU_CTRL2, 0x1e);
}

void FASTCALL UpdatePalette(byte updateMode)
{
  SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0x3f);
  
  VideoIndex = 0;
  
  if(updateMode == PALETTE_UPDATE_MODE_CHARACTER)
  {
    SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0x00);
    
    for(; VideoIndex < PALETTE_SIZE; ++VideoIndex)
    {
      SET_MEMORY_BYTE(PPU_VRAM_IO, CharacterPalette[VideoIndex]);
    }
  }
  else
  if(updateMode == PALETTE_UPDATE_MODE_SPRITE)
  {
    SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0x10);
    
    for(; VideoIndex < PALETTE_SIZE; ++VideoIndex)
    {
      SET_MEMORY_BYTE(PPU_VRAM_IO, SpritePalette[VideoIndex]);
    }
  }
  else // PALETTE_UPDATE_MODE_ALL
  {
    SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0x00);
    
    for(; VideoIndex < PALETTE_SIZE; ++VideoIndex)
    {
      SET_MEMORY_BYTE(PPU_VRAM_IO, CharacterPalette[VideoIndex]);
    }
    
    for(VideoIndex = 0; VideoIndex < PALETTE_SIZE; ++VideoIndex)
    {
      SET_MEMORY_BYTE(PPU_VRAM_IO, SpritePalette[VideoIndex]);
    }
  }
  
  // TODO: Why is this needed?
  SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0);
  SET_MEMORY_BYTE(PPU_VRAM_ADDR2, 0);
  
  //UpdatePaletteFlag = 0;
}

void FASTCALL SetCharacterColor(byte index, byte color)
{ 
  CharacterPalette[index] = color;
  
  // Flag video engine to update palette.
  UpdatePaletteFlag = 1;
}

void FASTCALL SetCharacterPrimaryColor(byte setIndex, byte color)
{ 
  CharacterPalette[(setIndex * 4) + 1] = color;
  
  // Flag video engine to update palette.
  UpdatePaletteFlag = 1;
}

void FASTCALL SetCharacterSecondaryColor(byte index, byte color)
{
  VideoIndex = 1 + index;
  CharacterPalette[VideoIndex] = color;
  
  VideoIndex += NUMBER_OF_COLORS_PER_PALETTE_ENTRY;
  CharacterPalette[VideoIndex] = color;
  
  VideoIndex += NUMBER_OF_COLORS_PER_PALETTE_ENTRY;
  CharacterPalette[VideoIndex] = color;
  
  VideoIndex += NUMBER_OF_COLORS_PER_PALETTE_ENTRY;
  CharacterPalette[VideoIndex] = color;
  
  // Flag video engine to update palette.
  UpdatePaletteFlag = 1;
}

void FASTCALL SetPrintColor(byte setIndex)
{
  //PrintColorSet = setIndex;
}
