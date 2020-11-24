#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <puzl.h>
#include <sdl/color.h>

SDL_Texture* SpriteSetTexture;

Sprite Sprites[NUMBER_OF_SPRITES];
byte SpriteSeconaryColor;
byte SpriteTertiaryColor;

void InitializeSprites(void);
void ShutdownSprites(void);

void PopulateSpriteSetSurfaceFromSpriteSet(SDL_Surface* spriteSetSurface);
void PopulateSpriteSurfaceFromSprite(SDL_Surface* spriteSurface, unsigned int spriteFrameIndex);

void DrawSpriteFrame(int x, int y, unsigned int spriteFrameIndex);

void InitializeSprites(void)
{
	SDL_Surface* spriteSetSurface = CreateSurface(SPRITE_WIDTH, SPRITE_HEIGHT * NUMBER_OF_SPRITE_FRAMES);
	if (spriteSetSurface == NULL)
	{
		// TODO: Generate error.
		return;
	}

	PopulateSpriteSetSurfaceFromSpriteSet(spriteSetSurface);

	SpriteSetTexture = SDL_CreateTextureFromSurface(Renderer, spriteSetSurface);

	SDL_FreeSurface(spriteSetSurface);
	spriteSetSurface = NULL;

	// Sprite controls.
	memset(Sprites, 0, NUMBER_OF_SPRITES * sizeof(Sprite));
}

void ShutdownSprites(void)
{
	SDL_DestroyTexture(SpriteSetTexture);
}

void PopulateSpriteSetSurfaceFromSpriteSet(SDL_Surface* spriteSetSurface)
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
		PopulateSpriteSurfaceFromSprite(spriteSurface, spriteFrameIndex);

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

void PopulateSpriteSurfaceFromSprite(SDL_Surface* spriteSurface, unsigned int spriteFrameIndex)
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

			*((unsigned int*)pixels) = colorCode != 0 ?
				SDL_MapRGBA(spriteSurface->format, 255 / colorCode, 255 / colorCode, 255 / colorCode, 255) :
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
			DrawSpriteFrame(sprite->x, sprite->y, sprite->frameIndex);
		}
	}
	while (--spriteIndex > -1);
}

void DrawSpriteFrame(int x, int y, unsigned int spriteFrameIndex)
{
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

	SDL_RenderCopy(Renderer, SpriteSetTexture, &sourceRect, &destinationRect);
}

void EnableSprite(byte spriteIndex, byte enable)
{
	Sprites[spriteIndex].enabled = enable;
}

void SetSpritePosition(byte spriteIndex, unsigned short x, unsigned short y)
{
	Sprite* sprite = &Sprites[spriteIndex];
	sprite->x = x;
	sprite->y = y;
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
	SpriteSeconaryColor = colorCode;
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SpriteTertiaryColor = colorCode;
}
