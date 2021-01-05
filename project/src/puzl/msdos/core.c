#include <msdos/core.h>

#include <stdio.h>
#include <string.h>

#include <puzl.h>

byte Running;

void Initialize(void);
void Shutdown(void);
void MainLoop(void);
void MainLoopIteration(void);

int main(int argc, char* args[])
{
	Initialize();

	MainLoop();

	ShutdownVideo();

	return 0;
}

void Initialize(void)
{
	//InitializeSequencer();

	InitializeInput();
	InitializeVideo();

	InitializeNodeTree();

	Running = 1;
}

void Shutdown(void)
{
	ShutdownVideo();
	//ShutdownInput();
}

inline void MainLoop(void)
{
	while (Running != 0)
	{
		MainLoopIteration();
	}
}

inline void MainLoopIteration(void)
{
	ProcessInput();

	Process();

	Draw();
	//ProcessSequences();
}
