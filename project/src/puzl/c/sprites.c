#include <puzl.h>

Sprite Sprites[NUMBER_OF_SPRITES];

byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void BaseInitializeSprites(void);

void BaseInitializeSprites(void)
{
	// Sprite controls.
	memset(Sprites, 0, NUMBER_OF_SPRITES * sizeof(Sprite));
}

signed short GetSpritePositionX(byte spriteIndex)
{
	return Sprites[spriteIndex].position.x;
}

signed short GetSpritePositionY(byte spriteIndex)
{
	return Sprites[spriteIndex].position.y;
}
