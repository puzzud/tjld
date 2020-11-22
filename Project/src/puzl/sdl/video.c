#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <stdio.h>

#include <puzl.h>
#include <sdl/color.h>

SDL_Window* Window;
SDL_Surface* ScreenSurface;
SDL_Renderer* Renderer;

SDL_Point RenderScale;

byte BackgroundColorCode;

byte* TileMapShapeCodes;
byte* TileMapColorCodes;

SDL_Texture* CharacterSetTexture;

void InitializeTilemap(void);
void InitializeCharacterSet(void);
void InitializeSprites(void);

SDL_Surface* CreateSurface(unsigned int width, unsigned int height);

void FreeTilemap(void);

void ClearScreen(void);
void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, byte colorCode);
void DrawTileMap(void);

int InitializeVideo(void)
{
	// TODO: Expose this.
	RenderScale.x = 3;
	RenderScale.y = 3;

	Window = SDL_CreateWindow
		(
			"tjld", // TODO: Expose this.
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH * RenderScale.x, SCREEN_HEIGHT * RenderScale.y,
			SDL_WINDOW_SHOWN
		);
	
  if (Window == NULL)
	{
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    
		return 1;
  }
	
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == NULL)
	{
		SDL_DestroyWindow(Window);

		fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());

		return 1;
	}

	SDL_RenderSetScale(Renderer, (float)RenderScale.x, (float)RenderScale.y);

	ScreenSurface = SDL_GetWindowSurface(Window);

	InitializeColors();
	InitializeTilemap();
	InitializeCharacterSet();
	InitializeSprites();

	return 0;
}

void InitializeTilemap(void)
{
	TileMapShapeCodes = (byte*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(byte));
	TileMapColorCodes = (byte*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(byte));
}

SDL_Surface* CreateSurface(unsigned int width, unsigned int height)
{
	Uint32 rmask, gmask, bmask, amask;

	// SDL interprets each pixel as a 32-bit number,
	// so our masks must depend on the endianness (byte order) of the machine.
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	return SDL_CreateRGBSurface
		(
			0,
			width, height,
			32,
			rmask, gmask, bmask, amask
		);
}

void InitializeCharacterSet(void)
{
	unsigned int x, y, byteIndex, bitIndex;
	byte characterByte;
	byte* pixels;

	SDL_Rect characterDestinationRect;
	
	SDL_Surface* characterSurface = NULL;

	// TODO: Generalize these numbers:
	// 16 is square root of 256.
	// 256 is TILE_WIDTH * number of characters.
	SDL_Surface* characterSetSurface = CreateSurface(16 * TILE_WIDTH, 16 * TILE_HEIGHT);
	if (characterSetSurface == NULL)
	{
		// TODO: Generate error.
		return;
	}

	// Populate surface with data based on Charset contents.
	characterSurface = CreateSurface(TILE_WIDTH, TILE_HEIGHT);
	if (characterSurface == NULL)
	{
		// TODO: Generate error.
		return;
	}

	characterDestinationRect.w = TILE_WIDTH;
	characterDestinationRect.h = TILE_HEIGHT;

	for (y = 0; y < 16; ++y)
	{
		for (x = 0; x < 16; ++x)
		{
			// Determine destination location for this character graphic.
			characterDestinationRect.x = x * TILE_WIDTH;
			characterDestinationRect.y = y * TILE_HEIGHT;

			// Populate character pixel data from character set data.
			if (SDL_MUSTLOCK(characterSurface))
			{
				SDL_LockSurface(characterSurface);
			}

			for (byteIndex = 0; byteIndex < 8; ++byteIndex)
			{
				characterByte = CharacterSet[(y * 16) + x][byteIndex];

				for (bitIndex = 0; bitIndex < 8; ++bitIndex)
				{
					pixels = (byte*)characterSurface->pixels;
					pixels += (byteIndex * characterSurface->pitch) + (bitIndex * sizeof(unsigned int));

					*((unsigned int*)pixels) = ((characterByte >> (7 - bitIndex)) & 1) != 0 ?
						SDL_MapRGBA(characterSurface->format, 255, 255, 255, 255) :
						SDL_MapRGBA(characterSurface->format, 0, 0, 0, 0);
				}
			}

			if (SDL_MUSTLOCK(characterSurface))
			{
				SDL_UnlockSurface(characterSurface);
			}

			// Copy it over.
			SDL_BlitSurface
				(
					characterSurface,
					NULL,
					characterSetSurface,
					&characterDestinationRect
				);
		}
	}

	CharacterSetTexture = SDL_CreateTextureFromSurface(Renderer, characterSetSurface);

	SDL_FreeSurface(characterSetSurface);
	characterSetSurface = NULL;

	SDL_FreeSurface(characterSurface);
	characterSurface = NULL;
}

void InitializeSprites(void)
{
	
}

void ShutdownVideo(void)
{
	FreeTilemap();

	SDL_DestroyTexture(CharacterSetTexture);
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
}

void FreeTilemap(void)
{
	free(TileMapShapeCodes);
	TileMapShapeCodes = NULL;

	free(TileMapColorCodes);
	TileMapColorCodes = NULL;
}

void Draw(void)
{
	ClearScreen();

	DrawTileMap();

	SDL_RenderPresent(Renderer);
}

void SetBackgroundColor(byte colorCode)
{
	BackgroundColorCode = colorCode;
}

void ClearScreen(void)
{
	SDL_Color* backgroundColor = &Colors[BackgroundColorCode];

	SDL_SetRenderDrawColor(Renderer, backgroundColor->r, backgroundColor->g, backgroundColor->b, 0xff);
	SDL_RenderClear(Renderer);
}

void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, byte colorCode)
{
	SDL_Color* color = &Colors[colorCode];

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, color->a);
	
	SDL_RenderFillRect(Renderer, &rect);
}

void DrawCharacter(unsigned int x, unsigned int y, unsigned int width, unsigned int height, byte shapeCode, byte colorCode)
{
	SDL_Color* color = &Colors[colorCode];

	SDL_Rect sourceRect;
	SDL_Rect destinationRect;

	sourceRect.x = (shapeCode % 16) * TILE_WIDTH;
	sourceRect.y = (shapeCode / 16) * TILE_HEIGHT;
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

		++charIndex;
	}
}
