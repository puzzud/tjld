#include <puzl.h>

Sprite Sprites[NUMBER_OF_SPRITES];

byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void BaseInitializeSprites(void);
void CalculateSpriteTileCorners(ScreenPoint* spritePosition, Vector2d* upperLeftSpriteTile, Vector2d* lowerRightSpriteTile);

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

inline void CalculateSpriteTileCorners(ScreenPoint* spritePosition, Vector2d* upperLeftSpriteTile, Vector2d* lowerRightSpriteTile)
{
	upperLeftSpriteTile->x = spritePosition->x / TILE_WIDTH;
	upperLeftSpriteTile->y = spritePosition->y / TILE_HEIGHT;

	// TODO: Need way to track sprite dimensions.
	lowerRightSpriteTile->x = (spritePosition->x + SPRITE_WIDTH - 1) / TILE_WIDTH;
	lowerRightSpriteTile->y = (spritePosition->y + SPRITE_HEIGHT - 1) / TILE_HEIGHT;	
}
