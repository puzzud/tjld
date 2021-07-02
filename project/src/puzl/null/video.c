#include <null/video.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include <null.h>

#include <puzl.h>

#include <null/color.h>

byte BackgroundColorCode;

void Draw(void);

void InitializeVideo(void)
{
	InitializeTilemap();
	InitializeSprites();
}

void ShutdownVideo(void)
{
	ShutdownSprites();
}

void Draw(void)
{
	
}

void SetBackgroundColor(byte colorCode)
{
	BackgroundColorCode = colorCode;
}
