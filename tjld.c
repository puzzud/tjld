// TJLD
#include <SDL2/SDL.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

const uint ScreenWidth = 320;
const uint ScreenHeight = 200;

const uint FramesPerSecond = 60;
const uint FrameDelay = 1000 / FramesPerSecond;

enum ColorCode
{
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_CYAN,
	COLOR_PURPLE,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_BROWN,
	COLOR_LIGHT_RED,
	COLOR_GREY_1,
	COLOR_GREY_2,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_BLUE,
	COLOR_GREY_3
};

const uint TileWidth = 8;
const uint TileHeight = 8;

int Init();
int Process();
void Draw();
int Shutdown();

void InitializeColors();
void InitializeTilemap();

void InitializeNodeTree();

void InitializeInput();

void FreeTilemap();

void SetColorValuesFromWord(SDL_Color* color, uint word);

void DrawRectangle(uint x, uint y, uint width, uint height, SDL_Color* color);
void DrawTileMap();

void SetTileMapCell(int x, int y, char shapeCode, char colorCode);

SDL_Window* Window;
SDL_Surface* ScreenSurface;
SDL_Renderer* Renderer;

uint FrameStart;
uint FrameTime;

uint TileMapWidth;
uint TileMapHeight;

#define NUMBER_OF_COLORS 16
SDL_Color Colors[NUMBER_OF_COLORS];

int CursorXPosition;
int CursorYPosition;

char* TileMapShapeCodes;
char* TileMapColorCodes;

uint KeyScanCodeStates[SDL_NUM_SCANCODES];

int main(int argc, char* args[])
{
	int result = Init();
	if (result != 0)
	{
		return result;
	}
  
	int quit = 0;

	while (quit == 0)
	{
		FrameStart = SDL_GetTicks();

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
					if (event.key.repeat == 0)
					{
						KeyScanCodeStates[event.key.keysym.scancode] = 1;

						switch (event.key.keysym.scancode)
						{
							case SDL_SCANCODE_ESCAPE:
							{
								quit = 1;

								break;
							}
						}
					}

					break;
				}

				case SDL_KEYUP:
				{
					if (event.key.repeat == 0)
					{
						KeyScanCodeStates[event.key.keysym.scancode] = 0;
					}

					break;
				}
			}
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

	InitializeColors();
	InitializeTilemap();

	InitializeNodeTree();

	return 0;
}

int Process()
{
	if (KeyScanCodeStates[SDL_SCANCODE_LEFT] == 1)
	{
		SetTileMapCell(CursorXPosition, CursorYPosition, 0, COLOR_BLACK);
		CursorXPosition -= 1;
		SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	}
	else if (KeyScanCodeStates[SDL_SCANCODE_RIGHT] == 1)
	{
		SetTileMapCell(CursorXPosition, CursorYPosition, 0, COLOR_BLACK);
		CursorXPosition += 1;
		SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	}

	if (KeyScanCodeStates[SDL_SCANCODE_UP] == 1)
	{
		SetTileMapCell(CursorXPosition, CursorYPosition, 0, COLOR_BLACK);
		CursorYPosition -= 1;
		SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	}
	else if (KeyScanCodeStates[SDL_SCANCODE_DOWN] == 1)
	{
		SetTileMapCell(CursorXPosition, CursorYPosition, 0, COLOR_BLACK);
		CursorYPosition += 1;
		SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	}

	return 0;
}

void Draw()
{
	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(Renderer);

	DrawTileMap();

	//SDL_Color color;
	//color.r = 0xff;
	//color.g = 0xff;
	//color.b = 0xff;
	//color.a = 0xff;
	//DrawRectangle((int)CursorXPosition, 32, 8, 8, &color);

	SDL_RenderPresent(Renderer);
}

int Shutdown()
{
	FreeTilemap();

	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

	return 0;
}

void InitializeColors()
{
	SetColorValuesFromWord(&Colors[COLOR_BLACK], 0x000000);
	SetColorValuesFromWord(&Colors[COLOR_WHITE], 0xffffff);
	SetColorValuesFromWord(&Colors[COLOR_RED], 0x880000);
	SetColorValuesFromWord(&Colors[COLOR_CYAN], 0xaaffee);
	SetColorValuesFromWord(&Colors[COLOR_PURPLE], 0xcc44cc);
	SetColorValuesFromWord(&Colors[COLOR_GREEN], 0x00cc55);
	SetColorValuesFromWord(&Colors[COLOR_BLUE], 0x0000aa);
	SetColorValuesFromWord(&Colors[COLOR_YELLOW], 0xeeee77);
	SetColorValuesFromWord(&Colors[COLOR_ORANGE], 0xdd8855);
	SetColorValuesFromWord(&Colors[COLOR_BROWN], 0x664400);
	SetColorValuesFromWord(&Colors[COLOR_LIGHT_RED], 0xff7777);
	SetColorValuesFromWord(&Colors[COLOR_GREY_1], 0x333333);
	SetColorValuesFromWord(&Colors[COLOR_GREY_2], 0x777777);
	SetColorValuesFromWord(&Colors[COLOR_LIGHT_GREEN], 0xaaff66);
	SetColorValuesFromWord(&Colors[COLOR_LIGHT_BLUE], 0x0088ff);
	SetColorValuesFromWord(&Colors[COLOR_GREY_3], 0xbbbbbb);
}

void InitializeTilemap()
{
	TileMapWidth = ScreenWidth / TileWidth;
	TileMapHeight = ScreenHeight / TileHeight;

	TileMapShapeCodes = (char*)calloc(TileMapWidth * TileMapHeight, sizeof(char));
	TileMapColorCodes = (char*)calloc(TileMapWidth * TileMapHeight, sizeof(char));
}

void InitializeNodeTree()
{
	// TODO: Call out custom logic?
	int CursorXPosition = 0;
	int CursorYPosition = 0;
	SetTileMapCell(CursorXPosition, CursorYPosition, 1, COLOR_LIGHT_BLUE);
	
	SetTileMapCell(TileMapWidth / 2, TileMapHeight / 2, 1, COLOR_YELLOW);
}

void InitializeInput()
{
	memset(KeyScanCodeStates, 0, SDL_NUM_SCANCODES * sizeof(uint));
}

void FreeTilemap()
{
	free(TileMapShapeCodes);
	TileMapShapeCodes = NULL;

	free(TileMapColorCodes);
	TileMapColorCodes = NULL;
}

void SetColorValuesFromWord(SDL_Color* color, uint word)
{
	color->r = (word >> 16) & 0xff;
	color->g = (word >> 8) & 0xff;
	color->b = word & 0xff;
	color->a = 0xff;
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

void SetTileMapCell(int x, int y, char shapeCode, char colorCode)
{
	uint tileMapOffset = (y * TileMapWidth) + x;

	TileMapShapeCodes[tileMapOffset] = shapeCode;
	TileMapColorCodes[tileMapOffset] = colorCode;
}
