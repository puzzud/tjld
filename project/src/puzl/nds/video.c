#include <nds/video.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <nds.h>

#include <puzl.h>

#include <nds/color.h>

byte BackgroundColorCode;

void Draw(void);

void InitializeVideo(void)
{
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	
	//videoSetModeSub(MODE_0_2D);
	consoleDemoInit();
	iprintf("\x1b[2J"); // ANSI escape sequence to clear & home cursor.

	SetBackgroundColor(COLOR_BLACK);
	setBackdropColorSub(Colors[COLOR_BLACK]);

	InitializeTilemap();
	InitializeSprites();

	irqSet(IRQ_VBLANK, Draw);
}

void ShutdownVideo(void)
{
	
}

void Draw(void)
{
	
}

void SetBackgroundColor(byte colorCode)
{
	BackgroundColorCode = colorCode;
	setBackdropColor(Colors[colorCode]);
}
