#include <c64/core.h>

//#include <stdio.h>

#include <c64/video.h>
#include <c64/input.h>

byte Running;

int Initialize(void);
int Process(void);
int Shutdown(void);
void MainLoop(void);
void MainLoopIteration(void);

extern void TestMain(void);

//int main(int argc, char* args[])
void TestMain(void)
{
	int result = Initialize();
	if (result != 0)
	{
		return;
	}
  
	Running = 1;

	MainLoop();
	Shutdown();
}

int Initialize(void)
{
	/*
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());

    return 1;
  }
	*/

	InitializeVideo();

	InitializeNodeTree();

	return 0;
}

int Shutdown(void)
{
	ShutdownVideo();

	//SDL_Quit();

	return 0;
}

void MainLoop(void)
{
	while (Running != 0)
	{
		MainLoopIteration();
	}
}

void MainLoopIteration(void)
{
/*
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
*/
}
