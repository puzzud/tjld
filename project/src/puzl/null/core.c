#include <null/core.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include <null.h>

#include <puzl.h>
#include <null/input.h>

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
		ProcessInput();

		Process();

		ProcessSequences();
	}
}
