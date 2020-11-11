#include <sdl/core.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdio.h>

#include "video.h"
#include "color.h"
#include "input.h"

unsigned int Running;

const unsigned int FramesPerSecond = 60;
#ifndef __EMSCRIPTEN__
const unsigned int FrameDelay = 1000 / FramesPerSecond;
#endif

unsigned int FrameStart;
unsigned int FrameTime;

int Initialize();
int Process();
int Shutdown();
void MainLoop();
void MainLoopIteration();

int main(int argc, char* args[])
{
	int result = Initialize();
	if (result != 0)
	{
		return result;
	}
  
	Running = 1;

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(MainLoopIteration, 0, 1);
	return 0;
#else
	MainLoop();
	return Shutdown();
#endif
}

int Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

void MainLoop()
{
	while (Running != 0)
	{
		MainLoopIteration();
	}
}

inline void MainLoopIteration()
{
#ifndef __EMSCRIPTEN__
	FrameStart = SDL_GetTicks();
#endif

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				Running = 0;

				break;
			}

			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				OnInputEvent(&event);

				break;
			}
		}
	}

	Process();
	Draw();

#ifndef __EMSCRIPTEN__
	FrameTime = SDL_GetTicks() - FrameStart;
	if (FrameDelay > FrameTime)
	{
		SDL_Delay(FrameDelay - FrameTime);
	}
#endif
}
