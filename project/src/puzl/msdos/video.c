#include <msdos/video.h>

#include <i86.h>
#include <stdio.h>
#include <string.h>

#include <puzl.h>

byte FAR* VideoBuffer = (byte FAR*)0xa0000l;

byte BackgroundColorCode;

void SetGraphicsMode(int mode);
void PlotPixel(word x, word y, byte color);
void FillScreen(byte color);
void ClearScreen(void);

void InitializeVideo(void)
{
	SetGraphicsMode(VGA_256_COLOR_MODE);

	BackgroundColorCode = COLOR_BLACK;
	ClearScreen();

	InitializeSprites();
}

void ShutdownVideo(void)
{
	SetGraphicsMode(TEXT_MODE);
}

void Draw(void)
{
	//ClearScreen();

	DrawTileMap();
	DrawSprites();
}

void SetGraphicsMode(int mode)
{
	union REGS regs;
	regs.h.ah = SET_MODE; // Function 0x00 = mode set.
	regs.h.al = mode;
	int386(VIDEO_INT, &regs, &regs);
}

/*
void PlotPixel(unsigned int x, unsigned int y, byte color)
{
  union REGS regs;

  regs.h.ah = WRITE_DOT;
  regs.h.al = color;
	regs.w.cx = x;
	regs.w.dx = y;
  int386(VIDEO_INT, &regs, &regs);
}
*/

#ifdef SLOW
#pragma aux PlotPixel = \
"mov ah, 0ch" \
"mov bh, 00h" \
"int 10h" \
parm [cx] [dx] [al] \
modify [ah bh];
#else
#pragma aux PlotPixel = \
"mov dx, 320" \
"mul dx" \
"add ax, bx" \
"mov di, ax" \
"mov ax, 0a000h" \
"mov es, ax" \
"mov [es:di], cl" \
parm [ax] [bx] [cl] \
modify [dx es di];
#endif

/*
#pragma aux FillScreen = \
"les di, 0a000h" \
"mov ah, al" \
"mov al, ah" \
"mov cx, 320*200/2" \
"rep stosw" \
parm [al] \
modify [es di ah cx];
*/

void FillScreen(byte color)
{
	memset((void*)VideoBuffer, color, SCREEN_WIDTH * SCREEN_HEIGHT);
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
	byte FAR* videoBufferOffset = &VideoBuffer[(y * SCREEN_WIDTH) + x];

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
