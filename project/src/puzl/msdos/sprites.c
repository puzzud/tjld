#include <msdos/video.h>

#include <string.h>

#include <puzl.h>

Sprite Sprites[NUMBER_OF_SPRITES];
byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void DrawSprite(Sprite* sprite);

void CheckSpriteCollision(byte spriteIndex, ScreenPoint* tempSpritePosition);

void InitializeSprites(void)
{
	// Sprite controls.
	memset((void*)Sprites, 0, NUMBER_OF_SPRITES * sizeof(Sprite));
}

void DrawSprites(void)
{
	unsigned int spriteIndex;
	Sprite* sprite;

	for (spriteIndex = 0; spriteIndex < NUMBER_OF_SPRITES; ++spriteIndex)
	{
		sprite = &Sprites[spriteIndex];

		if (sprite->enabled != 0)
		{
			DrawSprite(sprite);	
		}
	}
}

void DrawSprite(Sprite* sprite)
{
	unsigned int x = sprite->position.x;
	unsigned int y = sprite->position.y;
	//VideoBuffer[((y * SCREEN_WIDTH) + x)] = sprite->colorCode;
	DrawRectangle(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, sprite->colorCode);
}

void EnableSprite(byte spriteIndex, byte enable)
{
	Sprites[spriteIndex].enabled = enable;
}

signed short GetSpritePositionX(byte spriteIndex)
{
	return Sprites[spriteIndex].position.x;
}

signed short GetSpritePositionY(byte spriteIndex)
{
	return Sprites[spriteIndex].position.y;
}

void SetSpritePosition(byte spriteIndex, signed short x, signed short y)
{
	ScreenPoint* position = &Sprites[spriteIndex].position;
	position->x = x;
	position->y = y;
}

void SetSpriteVelocity(byte spriteIndex, signed char x, signed char y)
{
	Vector2d* velocity = &Sprites[spriteIndex].velocity;
	velocity->x = x;
	velocity->y = y;
}

void MoveSprite(byte spriteIndex)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;
	Vector2d* spriteVelocity = &sprite->velocity;

	ScreenPoint tempSpritePosition;

	tempSpritePosition.x = spritePosition->x + spriteVelocity->x;
	tempSpritePosition.y = spritePosition->y + spriteVelocity->y;
	
	// TODO: Make collision map optional.
	//if (1)
	{
		CheckSpriteCollision(spriteIndex, &tempSpritePosition);
	}

	spritePosition->x = tempSpritePosition.x;
	spritePosition->y = tempSpritePosition.y;
}

void CheckSpriteCollision(byte spriteIndex, ScreenPoint* tempSpritePosition)
{
	Sprite* sprite = &Sprites[spriteIndex];
	ScreenPoint* spritePosition = &sprite->position;
	Vector2d* spriteVelocity = &sprite->velocity;

	Vector2d upperLeftSpriteTile;
	Vector2d lowerRightSpriteTile;

	upperLeftSpriteTile.x = tempSpritePosition->x / TILE_WIDTH;
	upperLeftSpriteTile.y = tempSpritePosition->y / TILE_HEIGHT;

	// TODO: Need way to track sprite dimensions.
	lowerRightSpriteTile.x = (tempSpritePosition->x + 16 - 1) / TILE_WIDTH;
	lowerRightSpriteTile.y = (tempSpritePosition->y + 16 - 1) / TILE_HEIGHT;

	if (spriteVelocity->x < 0)
	{
		// Upper left.
		// Lower left.
		if ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) != 0)))
		{
			tempSpritePosition->x = spritePosition->x;
		}
	}
	else if (spriteVelocity->x > 0)
	{
		// Upper right.
		// Lower right.
		if ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) != 0)))
		{
			tempSpritePosition->x = spritePosition->x;
		}
	}

	if (spriteVelocity->y < 0)
	{
		// Upper left.
		// Upper right.
		if ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) != 0)))
		{
			tempSpritePosition->y = spritePosition->y;
		}
	}
	else if (spriteVelocity->y > 0)
	{
		// Lower left.
		// Lower right.
		if ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) != 0)))
		{
			tempSpritePosition->y = spritePosition->y;
		}
	}
}

void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	Sprites[spriteIndex].frameIndex = frameIndex;
}

void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode)
{
	Sprites[spriteIndex].colorCode = colorCode;
}

void FASTCALL SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;
}

void FASTCALL SetSpriteTertiaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[1] = colorCode;
}

void FASTCALL SetSpriteAnimationSet(byte spriteIndex, const byte** animationSet)
{
	Sprites[spriteIndex].animationSet = animationSet;
}

void FASTCALL PlaySpriteAnimation(byte spriteIndex, byte animationId, byte looping)
{
	const byte* animationStart;

	if (Sprites[spriteIndex].animationId == animationId)
	{
		return;
	}

	Sprites[spriteIndex].animationId = animationId;

	animationStart = Sprites[spriteIndex].animationSet[animationId];

	// TODO: Properly determine sequence from sprite index.
	//PlaySequence(spriteIndex + 3, animationStart, looping);
}

void FASTCALL StopSpriteAnimation(byte spriteIndex)
{
	// TODO: Properly determine sequence from sprite index.
	//StopSequence(spriteIndex + 3);
}
