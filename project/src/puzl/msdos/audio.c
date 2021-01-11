#include <msdos/audio.h>

#include <puzl.h>

#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>

#define SB_RESET            0x06
#define SB_READ_DATA        0x0a
#define SB_READ_DATA_STATUS 0x0e
#define SB_WRITE_DATA       0x0c

#define SB_ENABLE_SPEAKER   0xd1
#define SB_DISABLE_SPEAKER  0xd3

#define SB_SET_PLAYBACK_FREQUENCY 0x40
#define SB_SINGLE_CYCLE_PLAYBACK  0x14

#define MASK_REGISTER       0x0a
#define MODE_REGISTER       0x0b
#define MSB_LSB_FLIP_FLOP   0x0c
#define DMA_CHANNEL_0       0x87
#define DMA_CHANNEL_1       0x83
#define DMA_CHANNEL_3       0x82

const byte SpecialSbIrqNumbers[] =
{
	2,
	10,
	11
};

const byte SpecialSbIrqTableIndexes[] =
{
	0x71,
	0x72,
	0x73
};

const byte SpecialSbIrqPicBitMasks[] =
{
	0x02,
	0x04,
	0x08
};

word SbBase; // Default 220h.
byte SbIrq;  // Default 7.
byte SbDma;  // Default 1.

byte IrqTableIndex;
byte PicBitMask;

void _WCINTERRUPT (FAR* OldIsr)(void); // Holds old interrupt handler that audio one replaces.

volatile int IsPlaying;
volatile long AmountToBePlayed;

byte* DmaBuffer;
word Page;
word Offset;

void InitializeSoundBlaster(void);
void ShutdownSoundBlaster(void);

void InitializeFmSynthesis(void);

int IsSoundBlasterPresent(void);

int ResetDsp(word port);

void InitializeIrq(void);
void ShutdownIrq(void);
void AssignDmaBuffer(void);
void WriteDsp(byte command);

void PlaySoundFile(const char* fileName);
void SingleCyclePlayback(void);

void _WCINTERRUPT FAR AudioInterrupt(void);

void InitializeAudio(void)
{
	if (IsSoundBlasterPresent() != 0)
	{
		InitializeSoundBlaster();
		InitializeFmSynthesis();
		InitializeMusicEngine();
	}
}

void ShutdownAudio(void)
{
	ShutdownSoundBlaster();
}

int IsSoundBlasterPresent(void)
{
	byte index;
	char* blasterString;

	SbBase = 0;

	// Possible values: 210, 220, 230, 240, 250, 260, 280.
	for (index = 1; index < 9; ++index)
	{
		if (index != 7) // Skip 270.
		{
			if (ResetDsp(0x200 + (index << 4)))
			{
				break;
			}
		}
	}

	if (index == 9)
	{
		return 0;
	}

	blasterString = getenv("BLASTER");
	SbDma = 0;
	for (index = 0; index < strlen(blasterString); ++index)
	{
		if ((blasterString[index] | 32) == 'd')
		{
			SbDma = blasterString[index + 1] - '0';
		}
	}

	for (index = 0; index < strlen(blasterString); ++index)
	{
		if ((blasterString[index] | 32) == 'i')
		{
			SbIrq = blasterString[index + 1] - '0';

			if (blasterString[index + 2] != ' ')
			{
				SbIrq = (SbIrq * 10) + blasterString[index + 2] - '0';
			}
		}
	}

	return SbBase;
}

int ResetDsp(word port)
{
	outp(port + SB_RESET, 1);
	delay(10);
	outp(port + SB_RESET, 0);
	delay(10);

	if (((inp(port + SB_READ_DATA_STATUS) & 0x80) != 0) &&
	    (inp(port + SB_READ_DATA) == 0x0aa))
	{
		SbBase = port;
		return 1;
	}

	return 0;
}

void InitializeSoundBlaster(void)
{
	InitializeIrq();
	AssignDmaBuffer();
	WriteDsp(SB_ENABLE_SPEAKER);
}

void InitializeIrq(void)
{
	unsigned int index;

	// NOTE: Sets globals IrqTableIndex, PicBitMask.
	PicBitMask = 0;

	for (index = 0; index < (sizeof(SpecialSbIrqNumbers) / sizeof(byte)); ++index)
	{
		if (SpecialSbIrqNumbers[index] == SbIrq)
		{
			IrqTableIndex = SpecialSbIrqTableIndexes[index];
			PicBitMask = SpecialSbIrqPicBitMasks[index];
			break;
		}
	}

	if (PicBitMask == 0)
	{
		IrqTableIndex = SbIrq + 8;
	}

	// Save the old IRQ vector.
	OldIsr = _dos_getvect(IrqTableIndex);
	
	// Set audio IRQ vector.
	_dos_setvect(IrqTableIndex, AudioInterrupt);

	// Enable IRQ with the mainboard's PIC.
	if (PicBitMask != 0)
	{
		outp(0xa1, inp(0xa1) & !PicBitMask);
		outp(0x21, inp(0x21) & !0x04);
	}
	else
	{
		outp(0x21, inp(0x21) & !(1 << SbIrq));
	}
}

void ShutdownIrq(void)
{
	// Restore the old IRQ vector.
	_dos_setvect(IrqTableIndex, OldIsr);

	// Disable IRQ with the mainboard's PIC.
	if (PicBitMask != 0)
	{
		outp(0xa1, inp(0xa1) | PicBitMask);
		outp(0x21, inp(0x21) | 0x04);
	}
	else
	{
		outp(0x21, inp(0x21) | (1 << SbIrq));
	}
}

void AssignDmaBuffer(void)
{
	byte* tempBuffer;
	long LinearAddress;
	word Page1, Page2;

	tempBuffer = (byte*)malloc(32768);

	LinearAddress = FP_SEG(tempBuffer);
	LinearAddress = (LinearAddress << 4) + FP_OFF(tempBuffer);

	Page1 = LinearAddress >> 16;
	Page2 = (LinearAddress + 32768 - 1) >> 16;
	if (Page1 != Page2)
	{
		DmaBuffer = (byte*)malloc(32768);
		free(tempBuffer);
	}
	{
		DmaBuffer = tempBuffer;
	}

	LinearAddress = FP_SEG(DmaBuffer);
	LinearAddress = (LinearAddress << 4) + FP_OFF(DmaBuffer);

	Page = LinearAddress >> 16;
	Offset = LinearAddress & 0xffff;
}

void ShutdownSoundBlaster(void)
{
	WriteDsp(SB_DISABLE_SPEAKER);
	free(DmaBuffer);
	ShutdownIrq();
}

void WriteDsp(byte command)
{
	while ((inp(SbBase + SB_WRITE_DATA) & 0x80) != 0);

	outp(SbBase + SB_WRITE_DATA, command);
}

// sox Communicator.wav -b 8 -r 11000 -e unsigned -L sound.raw
// mplayer -rawaudio samplesize=1:channels=1:rate=11000 -demuxer rawaudio sound.raw
void PlaySoundFile(const char* fileName)
{
	FILE* rawFile;
	int fileSize;

	memset(DmaBuffer, 0, 32768);

	rawFile = fopen(fileName, "rb");
	if (rawFile == NULL)
	{
		//IsPlaying = 0;
		return;
	}
	
	fseek(rawFile, 0l, SEEK_END);
	fileSize = ftell(rawFile);
	
	fseek(rawFile, 0l, SEEK_SET);
	fread(DmaBuffer, 1, fileSize, rawFile);
	WriteDsp(SB_SET_PLAYBACK_FREQUENCY);
	WriteDsp((256 - 1000000) / 11000);
	AmountToBePlayed = fileSize;

	SingleCyclePlayback();
}

void SingleCyclePlayback(void)
{
	IsPlaying = 1;

	// Program the DMA controller.
	outp(MASK_REGISTER, 0x04 | SbDma);
	outp(MSB_LSB_FLIP_FLOP, 0);
	outp(MODE_REGISTER, 0x48 | SbDma);
	outp(SbDma << 1, Offset & 0xff);
	outp(SbDma << 1, (Offset >> 8) & 0xff);
	
	switch (SbDma)
	{
		case 0:
		{
			outp(DMA_CHANNEL_0, Page);
			break;
		}

		case 1:
		{
			outp(DMA_CHANNEL_1, Page);
			break;
		}

		case 3:
		{
			outp(DMA_CHANNEL_3, Page);
			break;
		}
	}

	outp((SbDma << 1) + 1, AmountToBePlayed & 0xff);
	outp((SbDma << 1) + 1, (AmountToBePlayed >> 8) & 0xff);
	outp(MASK_REGISTER, SbDma);
	WriteDsp(SB_SINGLE_CYCLE_PLAYBACK);
	WriteDsp(AmountToBePlayed & 0xff);
	WriteDsp((AmountToBePlayed >> 8) & 0xff);
	AmountToBePlayed = 0;
}

void _WCINTERRUPT FAR AudioInterrupt(void)
{
	inp(SbBase + SB_READ_DATA_STATUS);
	outp(0x20, 0x20);

	if (PicBitMask != 0)
	{
		outp(0xa0, 0x20);
	}

	IsPlaying = 0;
}

void SoundKillAll(void)
{
	
}

void InitializeFmSynthesis(void)
{
	unsigned int voiceIndex;

	for (voiceIndex = 0; voiceIndex < NUMBER_OF_ADLIB_CHANNELS; ++voiceIndex)
	{
		// Waveform.
		outp(PORT_ADLIB_STATUS, 0x20 + voiceIndex);
		outp(PORT_ADLIB_DATA, 0x01);

		// Amplitude.
		outp(PORT_ADLIB_STATUS, 0x40 + voiceIndex);
		outp(PORT_ADLIB_DATA, 0x10);

		// Attack / Decay.
		outp(PORT_ADLIB_STATUS, 0x60 + voiceIndex);
		outp(PORT_ADLIB_DATA, 0xf0);

		// Sustain / Release.
		outp(PORT_ADLIB_STATUS, 0x80 + voiceIndex);
		outp(PORT_ADLIB_DATA, 0x77);

		// Frequency (LSB).
		outp(PORT_ADLIB_STATUS, 0xa0 + voiceIndex);
		outp(PORT_ADLIB_DATA, 0x00);

		// Key On (5th bit).
		// Octave (bits 2-4).
		// Frequency (Most significant 2 bits, bits 0-1).
		// Bits 6-7 not used.
		outp(PORT_ADLIB_STATUS, 0xb0 + voiceIndex);
		outp(PORT_ADLIB_DATA, 0x00);
	}
}
