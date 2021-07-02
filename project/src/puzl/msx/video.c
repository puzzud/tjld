#include <msx/video.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include <msx.h>

#include <puzl.h>

#include <msx/color.h>

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
