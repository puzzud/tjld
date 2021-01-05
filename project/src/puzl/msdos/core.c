#include <msdos/core.h>

#include <i86.h>
#include <conio.h>
#include <stdio.h>

#include <puzl.h>

#define VIDEO_INT          0x10

#define SET_MODE           0x00
#define WRITE_DOT          0x0c

#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE          0x03

byte Running;

byte far* VideoBuffer = (byte far*)0xa0000l;

void InitializeVideo(void);
void ShutdownVideo(void);

void SetGraphicsMode(int mode);
void PlotPixel(int x, int y, byte color);
void FillScreen(byte color);

int main(int argc, char* args[])
{
	int key;

	InitializeVideo();

	//InitializeNodeTree();

	Running = 1;

	while(Running != 0)
	{
		if (kbhit() != 0)
		{
			key = getch();

			Running = 0;
		}

		//Process();
		Draw();
	}

	ShutdownVideo();

	return 0;
}

void InitializeVideo(void)
{
	SetGraphicsMode(VGA_256_COLOR_MODE);

	FillScreen(32);
}

void ShutdownVideo(void)
{
	SetGraphicsMode(TEXT_MODE);
}

void Draw(void)
{
	/*
	unsigned int y;
	unsigned int x;

	static unsigned int c = 0;
	
	for (y = 0; y < 200; ++y)
	{
		for (x = 0; x < 320; ++x)
		{
			VideoBuffer[((y * 320) + x)] = ++c;
		}
	}
	*/
}

void SetGraphicsMode(int mode)
{
	union REGS regs;
	regs.h.ah = SET_MODE; // Function 0x00 = mode set.
	regs.h.al = mode;
	int386(VIDEO_INT, &regs, &regs);
}

void PlotPixel(int x, int y, byte color)
{
  union REGS regs;

  regs.h.ah = WRITE_DOT;
  regs.h.al = color;
	regs.w.cx = x;
	regs.w.dx = y;
  int386(VIDEO_INT, &regs, &regs);
}

#pragma aux FillScreen = \
"mov [es:di], 0a000h" \
"les di, [es:di]" \
"mov ah, al" \
"mov cx, 320*200/2" \
"rep stosw" \
parm [al] \
modify [ah di cx es];
