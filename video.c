#include "video.h"

#include <SDL2/SDL.h>

#include "color.h"

const uint ScreenWidth = 320;
const uint ScreenHeight = 200;

SDL_Window* Window;
SDL_Surface* ScreenSurface;
SDL_Renderer* Renderer;

SDL_Point RenderScale;

uint BackgroundColorCode;

const uint TileWidth = 8;
const uint TileHeight = 8;

uint TileMapWidth;
uint TileMapHeight;

char* TileMapShapeCodes;
char* TileMapColorCodes;

int InitializeVideo()
{
	// TODO: Expose this.
	BackgroundColorCode = COLOR_BLACK;

	RenderScale.x = 3;
	RenderScale.y = 3;

	Window = SDL_CreateWindow
		(
			"tjld", // TODO: Expose this.
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			ScreenWidth * RenderScale.x, ScreenHeight * RenderScale.y,
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

void InitializeTilemap()
{
	TileMapWidth = ScreenWidth / TileWidth;
	TileMapHeight = ScreenHeight / TileHeight;

	TileMapShapeCodes = (char*)calloc(TileMapWidth * TileMapHeight, sizeof(char));
	TileMapColorCodes = (char*)calloc(TileMapWidth * TileMapHeight, sizeof(char));
}

void ShutdownVideo()
{
	FreeTilemap();

	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
}

void FreeTilemap()
{
	free(TileMapShapeCodes);
	TileMapShapeCodes = NULL;

	free(TileMapColorCodes);
	TileMapColorCodes = NULL;
}

void Draw()
{
	ClearScreen();

	DrawTileMap();

	//SDL_Color color;
	//color.r = 0xff;
	//color.g = 0xff;
	//color.b = 0xff;
	//color.a = 0xff;
	//DrawRectangle((int)CursorXPosition, 32, 8, 8, &color);

	SDL_RenderPresent(Renderer);
}

void ClearScreen()
{
	SDL_Color* backgroundColor = &Colors[BackgroundColorCode];

	SDL_SetRenderDrawColor(Renderer, backgroundColor->r, backgroundColor->g, backgroundColor->b, 0xff);
	SDL_RenderClear(Renderer);
}

void DrawRectangle(uint x, uint y, uint width, uint height, SDL_Color* color)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, color->a);
	
	SDL_RenderFillRect(Renderer, &rect);
}

void DrawTileMap()
{
	char shapeCode;
	char colorCode;

	uint columnIndex;
	uint rowIndex;

	for (rowIndex = 0; rowIndex < TileMapHeight; ++rowIndex)
	{
		for (columnIndex = 0; columnIndex < TileMapWidth; ++columnIndex)
		{
			shapeCode = TileMapShapeCodes[(rowIndex * TileMapWidth) + columnIndex];
			colorCode = 0;

			if (shapeCode != 0)
			{
				colorCode = TileMapColorCodes[(rowIndex * TileMapWidth) + columnIndex];
			}
			
			DrawRectangle(columnIndex * TileWidth, rowIndex * TileHeight, TileWidth, TileHeight, &Colors[(uint)colorCode]);
		}
	}
}

char GetTileMapShapeCode(int x, int y)
{
	return TileMapShapeCodes[(y * TileMapWidth) + x];
}

char GetTileMapColorCode(int x, int y)
{
	return TileMapColorCodes[(y * TileMapWidth) + x];
}

void SetTileMapCell(int x, int y, char shapeCode, char colorCode)
{
	const uint tileMapOffset = (y * TileMapWidth) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
