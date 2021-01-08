#include <msdos/core.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include <puzl.h>

#define TIMER_COUNTER_0 0x40  // Counter 0.

#define TIMER_CONTROL   0x43  // The 8253's control register.
#define TIMER_SET_BITS  0x3c  // The bit pattern that will place the timer into
                              // binary counting with counter load sequence of
                              // low byte then high byte.

#define TIMER_120HZ 0x26d7 // 120 hz
#define TIMER_60HZ  0x4dae // 60 hz
#define WAIT_TIME 3

byte Running;

void _WCINTERRUPT (FAR* OldTimerIsr)(void); // Holds old timer interrupt handler.

void Initialize(void);
void Shutdown(void);
void MainLoop(void);

void TimerProgram(int timer, unsigned int rate);
long GetTime(void);

void InitializeFrameInterrupt(void);
void ShutdownFrameInterrupt(void);
void _WCINTERRUPT FAR FrameInterrupt(void);

int main(int argc, char* args[])
{
	Initialize();
	MainLoop();
	Shutdown();

	return 0;
}

void Initialize(void)
{
	//InitializeSequencer();

	InitializeInput();
	InitializeVideo();

	InitializeNodeTree();

	InitializeFrameInterrupt();
}

void Shutdown(void)
{
	ShutdownFrameInterrupt();

	ShutdownVideo();
	ShutdownInput();
}

void InitializeFrameInterrupt(void)
{
	// Save old timekeeper ISR.
	OldTimerIsr = _dos_getvect(0x1c);

	// Install our new ISR.
	_dos_setvect(0x1c, FrameInterrupt);

	TimerProgram(TIMER_COUNTER_0, TIMER_60HZ);
}

void ShutdownFrameInterrupt(void)
{
	_dos_setvect(0x1c, OldTimerIsr);
}

inline void MainLoop(void)
{
	long startTime = GetTime();
	long nextTime = startTime + 1;

	Running = 1;

	while (Running != 0)
	{
		ProcessInput();

		Process();

		while (GetTime() < nextTime);

		//Draw();

		startTime = nextTime;
		++nextTime;
	}
}

void _WCINTERRUPT FAR FrameInterrupt(void)
{
	//_asm cli;

	if (Running == 0)
	{
		return;
	}

	Draw();
	//ProcessSequences();

	//_asm sti;

	_chain_intr(OldTimerIsr);
}

void TimerProgram(int timer, unsigned int rate)
{
	// Re-program the internal timer.

	// First program the timer to mode 2 - binary and data loading sequence of
	// low byte then high byte.
	outp(TIMER_CONTROL, TIMER_SET_BITS);

	// Write least significant byte of the new rate to the counter register.
	outp(timer, LowByte(rate));

	// And now the the most significant byte.
	outp(timer, HighByte(rate));
}

long GetTime(void)
{
	return *(long FAR*)0x0000046cl;
}
