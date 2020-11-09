// TJLD
#include <SDL2/SDL.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "video.h"
#include "color.h"
#include "input.h"

const uint FramesPerSecond = 60;
const uint FrameDelay = 1000 / FramesPerSecond;

uint FrameStart;
uint FrameTime;

int Init();
int Process();
int Shutdown();

void InitializeNodeTree();

uint Quit;

int CursorXPosition;
int CursorYPosition;
int Score;

int main(int argc, char* args[])
{
	int result = Init();
	if (result != 0)
	{
		return result;
	}
  
	Quit = 0;

	while (Quit == 0)
	{
		FrameStart = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				Quit = 1;
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

int Process()
{
	int cursorXDelta = KeyScanCodeStates[SDL_SCANCODE_RIGHT] - KeyScanCodeStates[SDL_SCANCODE_LEFT];
	int cursorYDelta = KeyScanCodeStates[SDL_SCANCODE_DOWN] - KeyScanCodeStates[SDL_SCANCODE_UP];

	if (cursorXDelta != 0 || cursorYDelta != 0)
	{
		SetTileMapCell(CursorXPosition, CursorYPosition, 0, COLOR_BLACK);
		CursorXPosition += cursorXDelta;
		CursorYPosition += cursorYDelta;

		if (GetTileMapShapeCode(CursorXPosition, CursorYPosition) != 0)
		{
			if (GetTileMapColorCode(CursorXPosition, CursorYPosition) == COLOR_YELLOW)
			{
				Score += 1;
				if (Score == 1)
				{
					Quit = 1;
				}

				printf("Score: %d", Score);
				printf("\n");
			}
		}

		SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	}

	return 0;
}

int Shutdown()
{
	ShutdownVideo();

	SDL_Quit();

	return 0;
}

void InitializeNodeTree()
{
	// TODO: Call out custom logic?
	Score = 0;

	CursorXPosition = 0;
	CursorYPosition = 0;
	SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);

	SetTileMapCell(TileMapWidth / 2, TileMapHeight / 2, 1, COLOR_YELLOW);
}
