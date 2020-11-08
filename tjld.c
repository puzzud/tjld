// TJLD
#include <SDL2/SDL.h>
#include <stdio.h>

const uint ScreenWidth = 320;
const uint ScreenHeight = 200;

const uint FramesPerSecond = 60;
const uint FrameDelay = 1000 / FramesPerSecond;

int Init();
int Process();
void Draw();
int Shutdown();

void DrawRectangle(uint x, uint y, uint width, uint height, SDL_Color* color);

SDL_Window* Window;
SDL_Surface* ScreenSurface;
SDL_Renderer* Renderer;

uint frameStart;
uint frameTime;

float CursorXPosition;

int main(int argc, char* args[])
{
	int result = Init();
	if (result != 0)
	{
		return result;
	}

	CursorXPosition = 32.0;
  
	int quit = 0;

	while (quit == 0)
	{
		frameStart = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					quit = 1;

					break;
				}

				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							quit = 1;

							break;
						}
					}

					break;
				}
			}
		}

		Process();

		Draw();

		frameTime = SDL_GetTicks() - frameStart;

		if (FrameDelay > frameTime)
		{
			SDL_Delay(FrameDelay - frameTime);
		}
	}

	return Shutdown();
}

int Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());

    return 1;
  }

  Window = SDL_CreateWindow
		(
			"tjld",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			ScreenWidth, ScreenHeight,
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

	ScreenSurface = SDL_GetWindowSurface(Window);

	return 0;
}

int Process()
{
	CursorXPosition += 0.5;

	return 0;
}

void Draw()
{
	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(Renderer);

	SDL_Color color;
	color.r = 0xff;
	color.g = 0xff;
	color.b = 0xff;
	color.a = 0xff;
	DrawRectangle((int)CursorXPosition, 32, 8, 8, &color);

	SDL_RenderPresent(Renderer);
}

int Shutdown()
{
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

	return 0;
}

void DrawRectangle(uint x, uint y, uint width, uint height, SDL_Color* color)
{
	SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, color->a);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_RenderFillRect(Renderer, &rect);
}
