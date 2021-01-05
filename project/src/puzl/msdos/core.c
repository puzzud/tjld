#include <msdos/core.h>

#include <stdio.h>

#include <puzl.h>

byte Running;

int main(int argc, char* args[])
{
	int key;

	InitializeVideo();

	//InitializeNodeTree();

	Running = 1;

	while(Running != 0)
	{
		if (kbhit() != 0)
		{
			key = getch();

			Running = 0;
		}

		//Process();
		Draw();
	}

	ShutdownVideo();

	return 0;
}
