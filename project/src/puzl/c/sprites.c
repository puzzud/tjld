#include <puzl.h>

byte CurrentSpriteIndex;

byte SpriteEnabledFlags[NUMBER_OF_SPRITES];
ScreenPoint SpritePositions[NUMBER_OF_SPRITES];
byte SpriteFrameIndices[NUMBER_OF_SPRITES];
const byte** SpriteAnimationSets[NUMBER_OF_SPRITES];
byte SpriteAnimationIds[NUMBER_OF_SPRITES];
byte SpriteColorCodes[NUMBER_OF_SPRITES];

byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void BaseInitializeSprites(void);

void BaseInitializeSprites(void)
{
	// Sprite controls.
	memset(SpriteEnabledFlags, 0, sizeof(SpriteEnabledFlags));
	memset(SpritePositions, 0, sizeof(SpritePositions));
	memset(SpriteFrameIndices, 0, sizeof(SpriteFrameIndices));
	memset(SpriteAnimationSets, 0, sizeof(SpriteAnimationSets));
	memset(SpriteAnimationIds, 0, sizeof(SpriteAnimationIds));
	memset(SpriteColorCodes, 0, sizeof(SpriteColorCodes));
}

signed short GetSpritePositionX(void)
{
	return SpritePositions[CurrentSpriteIndex].x;
}

signed short GetSpritePositionY(void)
{
	return SpritePositions[CurrentSpriteIndex].y;
}
