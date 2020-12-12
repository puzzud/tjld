#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <puzl.h>
#include <sdl/color.h>

SDL_Texture* SpriteSetTextures[NUMBER_OF_SPRITE_COLORS];

Sprite Sprites[NUMBER_OF_SPRITES];
byte SpriteNonPrimaryColorCodes[NUMBER_OF_SPRITE_COLORS - 1];

void InitializeSprites(void);
void ShutdownSprites(void);

void InitializeSpriteTextures(void);

void PopulateSpriteSetSurfaceFromSpriteSet(SDL_Surface* spriteSetSurface, byte focusColorCode);
void PopulateSpriteSurfaceFromSprite(SDL_Surface* spriteSurface, unsigned int spriteFrameIndex, byte focusColorCode);

void DrawSpriteFrame(int x, int y, unsigned int spriteFrameIndex, byte primaryColorCode);

void InitializeSprites(void)
{
	InitializeSpriteTextures();

	// Sprite controls.
	memset(Sprites, 0, NUMBER_OF_SPRITES * sizeof(Sprite));
}

void ShutdownSprites(void)
{
	unsigned int spriteColorIndex;
	for (spriteColorIndex = 0; spriteColorIndex < NUMBER_OF_SPRITE_COLORS; ++spriteColorIndex)
	{
		SDL_DestroyTexture(SpriteSetTextures[spriteColorIndex]);
	}
}

void InitializeSpriteTextures(void)
{
	unsigned int spriteColorIndex;

	for (spriteColorIndex = 0; spriteColorIndex < NUMBER_OF_SPRITE_COLORS; ++spriteColorIndex)
	{
		SDL_Surface* spriteSetSurface = CreateSurface(SPRITE_WIDTH, SPRITE_HEIGHT * NUMBER_OF_SPRITE_FRAMES);
		// TODO: Generate error if failed.

		PopulateSpriteSetSurfaceFromSpriteSet(spriteSetSurface, (byte)(spriteColorIndex + 1));
		SpriteSetTextures[spriteColorIndex] = SDL_CreateTextureFromSurface(Renderer, spriteSetSurface);

		SDL_FreeSurface(spriteSetSurface);
		spriteSetSurface = NULL;
	}
}

// focusColorCode will only observe pixels matching this color code in the input sprite set.
void PopulateSpriteSetSurfaceFromSpriteSet(SDL_Surface* spriteSetSurface, byte focusColorCode)
{
	unsigned int spriteFrameIndex;

	SDL_Rect spriteDestinationRect;

	SDL_Surface* spriteSurface = CreateSurface(SPRITE_WIDTH, SPRITE_HEIGHT);
	if (spriteSurface == NULL)
	{
		// TODO: Generate error.
		return;
	}

	spriteDestinationRect.w = SPRITE_WIDTH;
	spriteDestinationRect.h = SPRITE_HEIGHT;

	// Initial destination location for the first character graphic.
	spriteDestinationRect.x = 0;
	spriteDestinationRect.y = 0;

	for (spriteFrameIndex = 0; spriteFrameIndex < NUMBER_OF_SPRITE_FRAMES; ++spriteFrameIndex)
	{
		PopulateSpriteSurfaceFromSprite(spriteSurface, spriteFrameIndex, focusColorCode);

		// Copy it over.
		SDL_BlitSurface
			(
				spriteSurface,
				NULL,
				spriteSetSurface,
				&spriteDestinationRect
			);
		
		// Determine destination location for next character graphic.
		spriteDestinationRect.y += SPRITE_HEIGHT;
	}

	SDL_FreeSurface(spriteSurface);
	spriteSurface = NULL;
}

// focusColorCode will only observe pixels matching this color code in the input sprite,
// resulting in full white wherever it appears and then everywhere else is transparent.
void PopulateSpriteSurfaceFromSprite(SDL_Surface* spriteSurface, unsigned int spriteFrameIndex, byte focusColorCode)
{
	unsigned int x, y;
	byte colorCode;
	unsigned int* pixels;

	if (SDL_MUSTLOCK(spriteSurface))
	{
		SDL_LockSurface(spriteSurface);
	}

	pixels = (unsigned int*)spriteSurface->pixels;

	for (y = 0; y < SPRITE_HEIGHT; ++y)
	{
		for (x = 0; x < SPRITE_WIDTH; ++x)
		{
			colorCode = SpriteSet[spriteFrameIndex][y][x];
			if (colorCode != focusColorCode)
			{
				colorCode = 0;
			}

			*((unsigned int*)pixels) = colorCode != 0 ?
				SDL_MapRGBA(spriteSurface->format, 255, 255, 255, 255) :
				SDL_MapRGBA(spriteSurface->format, 0, 0, 0, 0);
			
			++pixels;
		}
	}

	if (SDL_MUSTLOCK(spriteSurface))
	{
		SDL_UnlockSurface(spriteSurface);
	}
}

void DrawSprites(void)
{
	Sprite* sprite;
	int spriteIndex = NUMBER_OF_SPRITES - 1;

	do
	{
		sprite = &Sprites[spriteIndex];

		if (sprite->enabled != 0)
		{
			DrawSpriteFrame(sprite->position.x, sprite->position.y, sprite->frameIndex, sprite->colorCode);
		}
	}
	while (--spriteIndex > -1);
}

void DrawSpriteFrame(int x, int y, unsigned int spriteFrameIndex, byte primaryColorCode)
{
	unsigned int spriteColorIndex;

	SDL_Color* color;

	SDL_Rect sourceRect;
	SDL_Rect destinationRect;

	sourceRect.x = 0;
	sourceRect.y = spriteFrameIndex * SPRITE_HEIGHT;
	sourceRect.w = SPRITE_WIDTH;
	sourceRect.h = SPRITE_HEIGHT;

	destinationRect.x = x;
	destinationRect.y = y;
	destinationRect.w = SPRITE_WIDTH;
	destinationRect.h = SPRITE_HEIGHT;

	color = &Colors[primaryColorCode];
	SDL_SetTextureColorMod(SpriteSetTextures[1], color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, SpriteSetTextures[1], &sourceRect, &destinationRect);

	color = &Colors[SpriteNonPrimaryColorCodes[0]];
	SDL_SetTextureColorMod(SpriteSetTextures[0], color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, SpriteSetTextures[0], &sourceRect, &destinationRect);
	
	color = &Colors[SpriteNonPrimaryColorCodes[1]];
	SDL_SetTextureColorMod(SpriteSetTextures[2], color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, SpriteSetTextures[2], &sourceRect, &destinationRect);
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
	Vector2d upperLeftSpriteTile;
	Vector2d lowerRightSpriteTile;

	tempSpritePosition.x = spritePosition->x + spriteVelocity->x;
	tempSpritePosition.y = spritePosition->y + spriteVelocity->y;

	upperLeftSpriteTile.x = tempSpritePosition.x / TILE_WIDTH;
	upperLeftSpriteTile.y = tempSpritePosition.y / TILE_HEIGHT;

	lowerRightSpriteTile.x = (tempSpritePosition.x + 16 - 1) / TILE_WIDTH; // TODO: Need way to track sprite dimensions.
	lowerRightSpriteTile.y = (tempSpritePosition.y + 16 - 1) / TILE_HEIGHT;

	if (spriteVelocity->x < 0)
	{
		// Upper left.
		// Lower left.
		if ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) != 0)))
		{
			tempSpritePosition.x = spritePosition->x;
		}
	}
	else if (spriteVelocity->x > 0)
	{
		// Upper right.
		// Lower right.
		if ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) != 0)))
		{
			tempSpritePosition.x = spritePosition->x;
		}
	}

	if (spriteVelocity->y < 0)
	{
		// Upper left.
		// Upper right.
		if ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) != 0)))
		{
			tempSpritePosition.y = spritePosition->y;
		}
	}
	else if (spriteVelocity->y > 0)
	{
		// Lower left.
		// Lower right.
		if ((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) != 0) ||
			 ((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) != 0)))
		{
			tempSpritePosition.y = spritePosition->y;
		}
	}

	spritePosition->x = tempSpritePosition.x;
	spritePosition->y = tempSpritePosition.y;
}

void SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	Sprites[spriteIndex].frameIndex = frameIndex;
}

void SetSpriteColor(byte spriteIndex, byte colorCode)
{
	Sprites[spriteIndex].colorCode = colorCode;
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[1] = colorCode;
}
