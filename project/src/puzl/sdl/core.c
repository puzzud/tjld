#include <sdl/core.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdio.h>
#include <math.h>

#include <puzl.h>

#include <sdl/video.h>
#include <sdl/color.h>
#include <sdl/input.h>
#include <sdl/audio.h>
#include <sdl/sequencer.h>

#define FRAMES_PER_SECOND	60
#define USE_PERFORMANCE_COUNTER 1

byte Running;

int Initialize(void);
void Process(void);
void Shutdown(void);
void MainLoop(void);
void MainLoopIteration(void);

int main(int argc, char* args[])
{
	int result = Initialize();
	if (result != 0)
	{
		return result;
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(MainLoopIteration, 0, 1);
	return 0;
#else
	MainLoop();

	Shutdown();
	
	return 0;
#endif
}

int Initialize(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());

		return 1;
  }

	InitializeSequencer();

	InitializeInput();
	InitializeVideo();
	InitializeAudio();

	InitalizeSpeed();
	InitializeNodeTree();

	Running = 1;

	return 0;
}

void Shutdown(void)
{
	ShutdownVideo();
	ShutdownInput();
	ShutdownAudio();

	SDL_Quit();
}

inline void MainLoop(void)
{
	while (Running != 0)
	{
		MainLoopIteration();
	}
}

inline void MainLoopIteration(void)
{
#ifndef __EMSCRIPTEN__
#ifndef USE_PERFORMANCE_COUNTER
	Uint32 FrameStart = SDL_GetTicks();
#else
	Uint64 FrameStart = SDL_GetPerformanceCounter();
#endif
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
			case SDL_CONTROLLERAXISMOTION:
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
			{
				OnInputEvent(&event);

				break;
			}
		}
	}

	Process();

	Draw();
	ProcessSequences();

#ifndef __EMSCRIPTEN__
	// Cap to 60 FPS.

#ifndef USE_PERFORMANCE_COUNTER
	Uint32 FrameTime = SDL_GetTicks();
	float elapsedTimeMs = FrameTime - FrameStart;
#else
	Uint64 FrameTime = SDL_GetPerformanceCounter();
	float elapsedTimeMs = (FrameTime - FrameStart) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
#endif

	float delayTimeMs = floor((1000.0f / (float)FRAMES_PER_SECOND) - elapsedTimeMs);
	if (delayTimeMs > 0.0)
	{
		SDL_Delay(delayTimeMs);
	}

#endif
}
