#include <sdl/video.h>

#include <SDL2/SDL.h>

#include <stdio.h>

#include <puzl.h>
#include <sdl/color.h>

SDL_Window* Window;
SDL_Surface* ScreenSurface;
SDL_Renderer* Renderer;

Point RenderScale;

char BackgroundColorCode;

char* TileMapShapeCodes;
char* TileMapColorCodes;

int InitializeVideo(void)
{
	// TODO: Expose this.
	BackgroundColorCode = COLOR_BLACK;

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

	return 0;
}

void InitializeTilemap(void)
{
	TileMapShapeCodes = (char*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(char));
	TileMapColorCodes = (char*)calloc(TILEMAP_WIDTH * TILEMAP_HEIGHT, sizeof(char));
}

void ShutdownVideo(void)
{
	FreeTilemap();

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

void ClearScreen()
{
	SDL_Color* backgroundColor = &Colors[BackgroundColorCode];

	SDL_SetRenderDrawColor(Renderer, backgroundColor->r, backgroundColor->g, backgroundColor->b, 0xff);
	SDL_RenderClear(Renderer);
}

void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char colorCode)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	SDL_Color* color = &Colors[colorCode];

	SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, color->a);
	
	SDL_RenderFillRect(Renderer, &rect);
}

void DrawTileMap()
{
	char shapeCode;
	char colorCode;

	unsigned int columnIndex;
	unsigned int rowIndex;

	for (rowIndex = 0; rowIndex < TILEMAP_HEIGHT; ++rowIndex)
	{
		for (columnIndex = 0; columnIndex < TILEMAP_WIDTH; ++columnIndex)
		{
			shapeCode = TileMapShapeCodes[(rowIndex * TILEMAP_WIDTH) + columnIndex];
			colorCode = 0;

			if (shapeCode != 0)
			{
				colorCode = TileMapColorCodes[(rowIndex * TILEMAP_WIDTH) + columnIndex];
			}
			
			DrawRectangle(columnIndex * TILE_WIDTH, rowIndex * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, colorCode);
		}
	}
}

char GetTileMapShapeCode(unsigned int x, unsigned int y)
{
	return TileMapShapeCodes[(y * TILEMAP_WIDTH) + x];
}

char GetTileMapColorCode(unsigned int x, unsigned int y)
{
	return TileMapColorCodes[(y * TILEMAP_WIDTH) + x];
}

void SetTileMapCell(unsigned int x, unsigned int y, char shapeCode, char colorCode)
{
	const unsigned int tileMapOffset = (y * TILEMAP_WIDTH) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
