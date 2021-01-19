#include <nds/audio.h>

#include <puzl.h>

#include <stdlib.h>
#include <stdio.h>

#include <nds.h>

void InitializeAudio(void)
{
	soundEnable();

	InitializeMusicEngine();
}

void ShutdownAudio(void)
{
	soundDisable();
}
