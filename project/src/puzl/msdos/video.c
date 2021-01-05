#include <msdos/video.h>

#include <i86.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

#include <puzl.h>

#define VIDEO_INT          0x10

#define SET_MODE           0x00
#define WRITE_DOT          0x0c

#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE          0x03

byte far* VideoBuffer = (byte far*)0xa0000l;

byte PrintX;
byte PrintY;
byte PrintColor;

void SetGraphicsMode(int mode);
void PlotPixel(word x, word y, byte color);
void FillScreen(byte color);

void InitializeVideo(void)
{
	SetGraphicsMode(VGA_256_COLOR_MODE);
}

void ShutdownVideo(void)
{
	SetGraphicsMode(TEXT_MODE);
}

void Draw(void)
{
	unsigned int y;
	unsigned int x;

	static unsigned int c = 0;

	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT / 2;
	VideoBuffer[((y * SCREEN_WIDTH) + x)] = ++c;

	//printf("%d\n", sizeof(x));
	/*
	for (y = 0; y < 200; ++y)
	{
		for (x = 0; x < 320; ++x)
		{
			//VideoBuffer[((y * 320) + x)] = ++c;
			//PlotPixel(x, y, ++c);
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
	memset(VideoBuffer, color, SCREEN_WIDTH * SCREEN_HEIGHT);
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

void FASTCALL EnableSprite(byte spriteIndex, byte enable)
{

}

signed short FASTCALL GetSpritePositionX(byte spriteIndex)
{
	return 0;
}

signed short FASTCALL GetSpritePositionY(byte spriteIndex)
{
	return 0;
}

void FASTCALL SetSpritePosition(byte spriteIndex, signed short x, signed short y)
{
	
}

void FASTCALL SetSpriteVelocity(byte spriteIndex, signed char x, signed char y)
{
	
}

void FASTCALL MoveSprite(byte spriteIndex)
{
	
}

void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	
}

void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode)
{
	
}

void FASTCALL SetSpriteSeconaryColor(byte colorCode)
{
	
}

void FASTCALL SetSpriteTertiaryColor(byte colorCode)
{
	
}

void FASTCALL SetSpriteAnimationSet(byte spriteIndex, const byte** animationSet)
{
	
}

void FASTCALL PlaySpriteAnimation(byte spriteIndex, byte animationId, byte looping)
{
	
}

void FASTCALL StopSpriteAnimation(byte spriteIndex)
{
	
}
