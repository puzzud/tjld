#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <puzl.h>

#include <sdl/color.h>

#include <c/sprites.c>
#include <c/sprites_physics.c>
#include <c/sprites_animation.c>

SDL_Texture* SpriteSetTextures[NUMBER_OF_SPRITE_COLORS];

void InitializeSprites(void);
void ShutdownSprites(void);

void InitializeSpriteTextures(void);

void PopulateSpriteSetSurfaceFromSpriteSet(SDL_Surface* spriteSetSurface, byte focusColorCode);
void PopulateSpriteSurfaceFromSprite(SDL_Surface* spriteSurface, unsigned int spriteFrameIndex, byte focusColorCode);

void DrawSprite(int spriteIndex);

void InitializeSprites(void)
{
	BaseInitializeSprites();
	BaseInitializeSpritesPhysics();
	BaseInitializeSpritesAnimation();

	InitializeSpriteTextures();
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
	int spriteIndex = NUMBER_OF_SPRITES - 1;

	do
	{
		if (SpriteEnabledFlags[spriteIndex] != 0)
		{
			DrawSprite(spriteIndex);
		}
	}
	while (--spriteIndex > -1);
}

inline void DrawSprite(int spriteIndex)
{
	SDL_Color* color;

	SDL_Rect sourceRect;
	SDL_Rect destinationRect;

	ScreenPoint* spritePosition = &SpritePositions[CurrentSpriteIndex];

	sourceRect.x = 0;
	sourceRect.y = SpriteFrameIndices[spriteIndex] * SPRITE_HEIGHT;
	sourceRect.w = SPRITE_WIDTH;
	sourceRect.h = SPRITE_HEIGHT;

	destinationRect.x = spritePosition->x;
	destinationRect.y = spritePosition->y;
	destinationRect.w = SPRITE_WIDTH;
	destinationRect.h = SPRITE_HEIGHT;

	color = &Colors[SpriteColorCodes[spriteIndex]];
	SDL_SetTextureColorMod(SpriteSetTextures[1], color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, SpriteSetTextures[1], &sourceRect, &destinationRect);

	color = &Colors[SpriteNonPrimaryColorCodes[0]];
	SDL_SetTextureColorMod(SpriteSetTextures[0], color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, SpriteSetTextures[0], &sourceRect, &destinationRect);
	
	color = &Colors[SpriteNonPrimaryColorCodes[1]];
	SDL_SetTextureColorMod(SpriteSetTextures[2], color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, SpriteSetTextures[2], &sourceRect, &destinationRect);
}

void EnableSprite(byte enable)
{
	SpriteEnabledFlags[CurrentSpriteIndex] = enable;
}

void SetSpritePosition(signed short x, signed short y)
{
	ScreenPoint* position = &SpritePositions[CurrentSpriteIndex];
	position->x = x;
	position->y = y;
}

void SetSpriteFrameIndex(byte frameIndex)
{
	SpriteFrameIndices[CurrentSpriteIndex] = frameIndex;
}

void SetSpriteColor(byte colorCode)
{
	SpriteColorCodes[CurrentSpriteIndex] = colorCode;
}

void SetSpriteSeconaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[0] = colorCode;
}

void SetSpriteTertiaryColor(byte colorCode)
{
	SpriteNonPrimaryColorCodes[1] = colorCode;
}
