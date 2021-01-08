#include <msdos/video.h>

#include <stdio.h>
#include <string.h>
#include <i86.h>
#include <conio.h>

#include <puzl.h>

#define INPUT_STATUS_0 0x3da

byte FAR* VideoBuffer;
byte FAR* DoubleBuffer;

byte BackgroundColorCode;

void SetGraphicsMode(int mode);

void DisplayDoubleBuffer(void);
int InitializeVideoBuffers(void);
void ShutdownVideoBuffers(void);

void PlotPixel(word x, word y, byte color);
void FillScreen(byte color);
void ClearScreen(void);

void InitializeVideo(void)
{
	SetGraphicsMode(VGA_256_COLOR_MODE);

	InitializeVideoBuffers();

	BackgroundColorCode = COLOR_BLACK;
	ClearScreen();

	InitializeSprites();
}

void ShutdownVideo(void)
{
	SetGraphicsMode(TEXT_MODE);

	ShutdownVideoBuffers();
}

int InitializeVideoBuffers(void)
{
	VideoBuffer = (byte FAR*)0xa0000l;

	DoubleBuffer = (byte FAR*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
	if (DoubleBuffer == NULL)
	{
		printf("Failed to allocate double buffer.\n");
		return 1;
	}

	return 0;
}

void ShutdownVideoBuffers(void)
{
	if (DoubleBuffer != NULL)
	{
		free(DoubleBuffer);
	}
}

void DisplayDoubleBuffer(void)
{
	memcpy((void*)VideoBuffer, (void*)DoubleBuffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void Draw(void)
{
	// Wait for vertical re-trace.
	//while (inp(INPUT_STATUS_0) & 0x08);
	//while (!(inp(INPUT_STATUS_0) & 0x08));

	ClearScreen();

	DrawTileMap();
	DrawSprites();

	DisplayDoubleBuffer();
}

void SetGraphicsMode(int mode)
{
	union REGS regs;
	regs.h.ah = SET_MODE; // Function 0x00 = mode set.
	regs.h.al = mode;
	int386(VIDEO_INT, &regs, &regs);
}

void FillScreen(byte color)
{
	memset((void*)DoubleBuffer, color, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void ClearScreen(void)
{
	FillScreen(BackgroundColorCode);
}

void SetBackgroundColor(byte colorCode)
{
	BackgroundColorCode = colorCode;
}

void DrawRectangle(signed short x, signed short y, unsigned short width, unsigned short height, byte colorCode)
{
	//int xCounter;
	int yCounter = height;
	byte FAR* videoBufferOffset = &DoubleBuffer[(y * SCREEN_WIDTH) + x];

	do
	{
		/*
		xCounter = width;

		do
		{
			*videoBufferOffset = colorCode;

			++videoBufferOffset;
		}
		while (--xCounter != 0);
		*/

		memset((void*)videoBufferOffset, colorCode, width);

		videoBufferOffset += SCREEN_WIDTH;
	}
	while (--yCounter != 0);
}
