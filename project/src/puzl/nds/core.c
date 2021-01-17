#include <nds/core.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <nds.h>

#include <puzl.h>

#include <c/sequencer.h>

byte Running;

void Initialize(void);
void Shutdown(void);
void MainLoop(void);

int main(void)
{
	Initialize();
	MainLoop();
	Shutdown();

	return 0;
}

void Initialize(void)
{
	InitializeSequencer();

	InitializeInput();
	InitializeVideo();
	InitializeAudio();

	InitalizeSpeed();
	InitializeNodeTree();
}

void Shutdown(void)
{
	ShutdownAudio();
	ShutdownVideo();
	ShutdownInput();
}

inline void MainLoop(void)
{
	Running = 1;

	while (Running != 0)
	{
		swiWaitForVBlank();

		ProcessInput();
		
		if (ControllerButtonState & KEY_START)
		{
			SetBackgroundColor(COLOR_RED);
		}

		//if ((keys & KEY_L) && (keys & KEY_R))
		{
			//Running = 0;
		}

		Process();

		ProcessSequences();
	}
}
