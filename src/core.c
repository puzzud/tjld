#include "core.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#include "video.h"
#include "color.h"
#include "input.h"

uint Running;

const uint FramesPerSecond = 60;
const uint FrameDelay = 1000 / FramesPerSecond;

uint FrameStart;
uint FrameTime;

int Init();
int Process();
int Shutdown();

int main(int argc, char* args[])
{
	int result = Init();
	if (result != 0)
	{
		return result;
	}
  
	Running = 1;

	while (Running != 0)
	{
		FrameStart = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				Running = 0;
			}

			OnInputEvent(&event);
		}

		Process();

		Draw();

		FrameTime = SDL_GetTicks() - FrameStart;

		if (FrameDelay > FrameTime)
		{
			SDL_Delay(FrameDelay - FrameTime);
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

	InitializeVideo();

	InitializeNodeTree();

	return 0;
}

int Shutdown()
{
	ShutdownVideo();

	SDL_Quit();

	return 0;
}
