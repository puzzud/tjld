#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <stdio.h>

#include <puzl.h>
#include <sdl/color.h>

#include <c/tilemap.c>

SDL_Texture* CharacterSetTexture;

void PopulateCharacterSetSurfaceFromCharacterSet(SDL_Surface* characterSetSurface);
void PopulateCharacterSurfaceFromCharacter(SDL_Surface* characterSurface, unsigned int characterIndex);

void InitializeCharacterSet(void)
{
	SDL_Surface* characterSetSurface = CreateSurface(TILE_WIDTH, NUMBER_OF_CHARACTERS * TILE_HEIGHT);
	if (characterSetSurface == NULL)
	{
		// TODO: Generate error.
		return;
	}

	PopulateCharacterSetSurfaceFromCharacterSet(characterSetSurface);

	CharacterSetTexture = SDL_CreateTextureFromSurface(Renderer, characterSetSurface);

	SDL_FreeSurface(characterSetSurface);
	characterSetSurface = NULL;
}

void ShutdownCharacterSet(void)
{
	SDL_DestroyTexture(CharacterSetTexture);
}

void PopulateCharacterSetSurfaceFromCharacterSet(SDL_Surface* characterSetSurface)
{
	unsigned int characterIndex;

	SDL_Rect characterDestinationRect;
	
	SDL_Surface* characterSurface = CreateSurface(TILE_WIDTH, TILE_HEIGHT);
	if (characterSurface == NULL)
	{
		// TODO: Generate error.
		return;
	}

	characterDestinationRect.w = TILE_WIDTH;
	characterDestinationRect.h = TILE_HEIGHT;

	// Initial destination location for the first character graphic.
	characterDestinationRect.x = 0;
	characterDestinationRect.y = 0;

	for (characterIndex = 0; characterIndex < NUMBER_OF_CHARACTERS; ++characterIndex)
	{
		PopulateCharacterSurfaceFromCharacter(characterSurface, characterIndex);

		// Copy it over.
		SDL_BlitSurface
			(
				characterSurface,
				NULL,
				characterSetSurface,
				&characterDestinationRect
			);
		
		// Determine destination location for next character graphic.
		characterDestinationRect.y += TILE_HEIGHT;
	}

	SDL_FreeSurface(characterSurface);
	characterSurface = NULL;
}

void PopulateCharacterSurfaceFromCharacter(SDL_Surface* characterSurface, unsigned int characterIndex)
{
	unsigned int x, y;
	byte colorCode;
	unsigned int* pixels;

	if (SDL_MUSTLOCK(characterSurface))
	{
		SDL_LockSurface(characterSurface);
	}

	pixels = (unsigned int*)characterSurface->pixels;

	for (y = 0; y < CHARACTER_HEIGHT; ++y)
	{
		for (x = 0; x < CHARACTER_WIDTH; ++x)
		{
			colorCode = CharacterSet[characterIndex][y][x];

			// TODO: Actually, check color code.
			*((unsigned int*)pixels) = colorCode != 0 ?
				SDL_MapRGBA(characterSurface->format, 255, 255, 255, 255) :
				SDL_MapRGBA(characterSurface->format, 0, 0, 0, 0);
			
			++pixels;
		}
	}

	if (SDL_MUSTLOCK(characterSurface))
	{
		SDL_UnlockSurface(characterSurface);
	}
}

void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode)
{
	SDL_Color* color = &Colors[colorCode];

	SDL_Rect sourceRect;
	SDL_Rect destinationRect;

	sourceRect.x = 0;
	sourceRect.y = shapeCode * TILE_HEIGHT;
	sourceRect.w = TILE_WIDTH;
	sourceRect.h = TILE_HEIGHT;

	destinationRect.x = x;
	destinationRect.y = y;
	destinationRect.w = TILE_WIDTH;
	destinationRect.h = TILE_HEIGHT;

	SDL_SetTextureColorMod(CharacterSetTexture, color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, CharacterSetTexture, &sourceRect, &destinationRect);
}
