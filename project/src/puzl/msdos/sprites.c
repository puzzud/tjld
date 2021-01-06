#include <msdos/video.h>

#include <string.h>

#include <puzl.h>

Sprite Sprites[NUMBER_OF_SPRITES];

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
	//VideoBuffer[((y * SCREEN_WIDTH) + x)] = sprite->colorCode + 4;
	DrawRectangle(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, sprite->colorCode + 4);
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

}

void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	
}

void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode)
{
	
}

void FASTCALL SetSpriteSeconaryColor(byte colorCode)
{
	
}

void FASTCALL SetSpriteTertiaryColor(byte colorCode)
{
	
}

void FASTCALL SetSpriteAnimationSet(byte spriteIndex, const byte** animationSet)
{
	
}

void FASTCALL PlaySpriteAnimation(byte spriteIndex, byte animationId, byte looping)
{
	
}

void FASTCALL StopSpriteAnimation(byte spriteIndex)
{
	
}
