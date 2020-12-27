#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <stdio.h>

#include <puzl.h>
#include <sdl/color.h>

byte* TileMapShapeCodes;
byte* TileMapColorCodes;
byte* TileMapCollisionCodes;

SDL_Texture* CharacterSetTexture;

byte PrintColor;

void PopulateCharacterSetSurfaceFromCharacterSet(SDL_Surface* characterSetSurface);
void PopulateCharacterSurfaceFromCharacter(SDL_Surface* characterSurface, unsigned int characterIndex);

void InitializeTilemap(void)
{
	TileMapShapeCodes = (byte*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(byte));
	TileMapColorCodes = (byte*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(byte));
	TileMapCollisionCodes = (byte*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(byte));
}

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
	const byte* characterBytes = &CharacterSet[characterIndex][0];
	unsigned int byteIndex, bitIndex;
	byte characterByte;
	unsigned int* pixels;

	if (SDL_MUSTLOCK(characterSurface))
	{
		SDL_LockSurface(characterSurface);
	}

	pixels = (unsigned int*)characterSurface->pixels;

	for (byteIndex = 0; byteIndex < CHARACTER_HEIGHT; ++byteIndex)
	{
		characterByte = *(characterBytes + byteIndex);

		for (bitIndex = 0; bitIndex < 8; ++bitIndex)
		{
			*((unsigned int*)pixels) = ((characterByte >> (7 - bitIndex)) & 1) != 0 ?
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

void FreeTilemap(void)
{
	free(TileMapShapeCodes);
	TileMapShapeCodes = NULL;

	free(TileMapColorCodes);
	TileMapColorCodes = NULL;
}

void DrawCharacter(unsigned int x, unsigned int y, unsigned int width, unsigned int height, byte shapeCode, byte colorCode)
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
	destinationRect.w = width;
	destinationRect.h = height;

	SDL_SetTextureColorMod(CharacterSetTexture, color->r, color->g, color->b);
	SDL_RenderCopy(Renderer, CharacterSetTexture, &sourceRect, &destinationRect);
}

void DrawTileMap()
{
	byte shapeCode;
	byte colorCode;

	unsigned int columnIndex;
	unsigned int rowIndex;

	for (rowIndex = 0; rowIndex < TILEMAP_HEIGHT; ++rowIndex)
	{
		for (columnIndex = 0; columnIndex < TILEMAP_WIDTH; ++columnIndex)
		{
			shapeCode = TileMapShapeCodes[(rowIndex * TILEMAP_WIDTH) + columnIndex];
			if (shapeCode != 0)
			{
				colorCode = TileMapColorCodes[(rowIndex * TILEMAP_WIDTH) + columnIndex];
				DrawCharacter(columnIndex * TILE_WIDTH, rowIndex * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, shapeCode, colorCode);
			}
		}
	}
}

byte GetTileMapShapeCode(byte x, byte y)
{
	return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

byte GetTileMapColorCode(byte x, byte y)
{
	return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

byte GetTileMapCellCollisionCode(byte x, byte y)
{
	return TileMapCollisionCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCellShape(byte x, byte y, byte shapeCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
}

void SetTileMapCellColor(byte x, byte y, byte colorCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapColorCodes[tileMapOffset] = colorCode;
}

void SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapCollisionCodes[tileMapOffset] = collisionCode;
}

void PrintText(const char* text, byte x, byte y)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;
	
	byte characterCode;
	int charIndex = 0;

	while (1)
	{
		characterCode = text[charIndex];
		if (characterCode == 0)
		{
			break;
		}

		TileMapShapeCodes[tileMapOffset + charIndex] = characterCode;
		TileMapColorCodes[tileMapOffset + charIndex] = PrintColor;

		++charIndex;
	}
}
