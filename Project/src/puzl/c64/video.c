#include <c64/video.h>

//#include <SDL2/SDL.h>

#include <stdio.h>

#include <color.h>

Point ScreenDimensions;

//SDL_Window* Window;
//SDL_Surface* ScreenSurface;
//SDL_Renderer* Renderer;

//Point RenderScale;

char BackgroundColorCode;

Point TileDimensions;

Point TileMapDimensions;

char* TileMapShapeCodes;
char* TileMapColorCodes;

int InitializeVideo(void)
{
	// TODO: Expose this.
	BackgroundColorCode = COLOR_BLACK;

	ScreenDimensions.x = 320;
	ScreenDimensions.y = 200;

	TileDimensions.x = 8;
	TileDimensions.y = 8;

	//RenderScale.x = 3;
	//RenderScale.y = 3;
/*
	Window = SDL_CreateWindow
		(
			"tjld", // TODO: Expose this.
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			ScreenDimensions.x * RenderScale.x, ScreenDimensions.y * RenderScale.y,
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
*/
	return 0;
}

void InitializeTilemap(void)
{
	//TileMapDimensions.x = ScreenDimensions.x / TileDimensions.x;
	//TileMapDimensions.y = ScreenDimensions.y / TileDimensions.y;

	//TileMapShapeCodes = (char*)calloc(TileMapDimensions.x * TileMapDimensions.y, sizeof(char));
	//TileMapColorCodes = (char*)calloc(TileMapDimensions.x * TileMapDimensions.y, sizeof(char));
}

void ShutdownVideo(void)
{
	
}
/*
void Draw(void)
{
	ClearScreen();

	//DrawTileMap();

	SDL_RenderPresent(Renderer);
}
*/
void ClearScreen(void)
{
	
}

char GetTileMapShapeCode(unsigned int x, unsigned int y)
{
	return TileMapShapeCodes[(y * TileMapDimensions.x) + x];
}

char GetTileMapColorCode(unsigned int x, unsigned int y)
{
	return TileMapColorCodes[(y * TileMapDimensions.x) + x];
}

void SetTileMapCell(unsigned int x, unsigned int y, char shapeCode, char colorCode)
{
	const unsigned int tileMapOffset = (y * TileMapDimensions.x) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
