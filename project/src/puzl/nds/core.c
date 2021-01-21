#include <nds/core.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <nds.h>

#include <puzl.h>
#include <nds/input.h>

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
		
		if (KeysState & (KEY_L | KEY_R))
		{
			Running = 0;
		}

		Process();

		ProcessSequences();
	}
}
