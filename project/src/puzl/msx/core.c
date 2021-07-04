#include <msx/core.h>

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>

//#include <msx.h>

#include <puzl.h>
#include <msx/input.h>

#include <c/sequencer.h>

byte Running;

void Initialize(void);
void Shutdown(void);
void MainLoop(void);

void main(void)
{
	Initialize();

	Running = 1;

	while (Running != 0)
	{
		ProcessInput();
		Process();
		ProcessSequences();
	}

	Shutdown();
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
