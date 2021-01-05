#include <msdos/video.h>

#include <i86.h>
#include <stdio.h>
#include <string.h>

#include <puzl.h>

byte far* VideoBuffer = (byte far*)0xa0000l;

byte PrintX;
byte PrintY;
byte PrintColor;

void SetGraphicsMode(int mode);
void PlotPixel(word x, word y, byte color);
void FillScreen(byte color);
void ClearScreen(void);

void DrawTileMap(void);

void InitializeVideo(void)
{
	SetGraphicsMode(VGA_256_COLOR_MODE);

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
	FillScreen(0);
}

void SetBackgroundColor(byte colorCode)
{
	
}

byte FASTCALL GetTileMapShapeCode(byte x, byte y)
{
	return 0;
}

byte FASTCALL GetTileMapColorCode(byte x, byte y)
{
	return 0;
}

byte FASTCALL GetTileMapCellCollisionCode(byte x, byte y)
{
	return 0;
}

void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode)
{

}

void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode)
{

}

void FASTCALL SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{

}

void FASTCALL PrintText(const char* text, byte x, byte y)
{

}

void DrawTileMap(void)
{

}
