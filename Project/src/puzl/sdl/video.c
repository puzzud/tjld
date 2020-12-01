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

void ClearScreen(void);
void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, byte colorCode);

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
	
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == NULL)
	{
		SDL_DestroyWindow(Window);

		fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());

		return 1;
	}

	SDL_RenderSetScale(Renderer, (float)RenderScale.x, (float)RenderScale.y);
	SDL_RenderSetLogicalSize(Renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	ScreenSurface = SDL_GetWindowSurface(Window);

	InitializeColors();
	InitializeTilemap();
	InitializeCharacterSet();
	InitializeSprites();

	return 0;
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

void ShutdownVideo(void)
{
	FreeTilemap();

	ShutdownCharacterSet();
	ShutdownSprites();
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
}

void Draw(void)
{
	ClearScreen();

	DrawTileMap();
	DrawSprites();

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
