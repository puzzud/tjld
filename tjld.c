// TJLD
#include <SDL2/SDL.h>
#include <stdio.h>

const uint ScreenWidth = 320;
const uint ScreenHeight = 200;

const uint FramesPerSecond = 60;
const uint FrameDelay = 1000 / FramesPerSecond;

int init();
int process();
void draw();
int shutdown();

SDL_Window* Window;
SDL_Surface* ScreenSurface;
SDL_Renderer* Renderer;

uint frameStart;
uint frameTime;

SDL_Rect CursorRect;

float CursorXPosition;

int main(int argc, char* args[])
{
	int result = init();
	if (result != 0)
	{
		return result;
	}
	
	CursorRect.x = CursorXPosition = 32;
	CursorRect.y = 32;
	CursorRect.w = 8;
	CursorRect.h = 8;
  
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

		process();

		draw();

		frameTime = SDL_GetTicks() - frameStart;

		if (FrameDelay > frameTime)
		{
			SDL_Delay(FrameDelay - frameTime);
		}
	}

	return shutdown();
}

int init()
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

int process()
{
	CursorXPosition += 0.5;

	CursorRect.x = (int)CursorXPosition;

	return 0;
}

void draw()
{
	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(Renderer);

	SDL_SetRenderDrawColor(Renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(Renderer, &CursorRect);

	SDL_RenderPresent(Renderer);
}

int shutdown()
{
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

	return 0;
}
