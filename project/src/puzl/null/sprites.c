#include <null/video.h>

#include <string.h>

#include <puzl.h>

#include <null/color.h>

#include <c/sprites.c>
#include <c/sprites_physics.c>
#include <c/sprites_animation.c>

extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_HEIGHT];

void InitializeSpriteGraphics(void);

void SetSpritePaletteColumnColorCodes(unsigned int columnIndex, byte colorCode);

void InitializeSprites(void)
{
	
}

void InitializeSpriteGraphics(void)
{
	
}

void ShutdownSprites(void)
{
	
}

void EnableSprite(byte enable)
{
	Sprites[CurrentSpriteIndex].enabled = enable;
}

void SetSpritePosition(signed short x, signed short y)
{
	
}

void SetSpriteFrameIndex(byte frameIndex)
{
	
}

void SetSpriteColor(byte colorCode)
{
	
}

void SetSpritePaletteColumnColorCodes(unsigned int columnIndex, byte colorCode)
{
	
}

void SetSpriteSeconaryColor(byte colorCode)
{
	
}

void SetSpriteTertiaryColor(byte colorCode)
{
	
}
