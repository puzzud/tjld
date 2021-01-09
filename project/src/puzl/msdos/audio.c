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
#define SB_SINGLE_CYCLE_PLAYBACK 0x14

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

word sb_base; // Default 220h.
byte sb_irq; // Default 7.
byte sb_dma; // Default 1.

byte IrqTableIndex;
byte PicBitMask;

void _WCINTERRUPT (FAR* OldIsr)(void); // Holds old interrupt handler that audio one replaces.

volatile int playing;
volatile long to_be_played;

byte* dma_buffer;
word page;
word offset;

void InitializeSoundBlaster(void);
void ShutdownSoundBlaster(void);

int sb_detect(void);
void sb_init(void);
void sb_deinit(void);

int reset_dsp(word port);

void init_irq(void);
void deinit_irq(void);
void assign_dma_buffer(void);
void write_dsp(byte command);

void sb_single_play(const char* file_name);
void single_cycle_playback(void);

void _WCINTERRUPT FAR AudioInterrupt(void);

void InitializeAudio(void)
{
	InitializeSoundBlaster();

	sb_single_play("tadaa.raw");
}

void ShutdownAudio(void)
{
	ShutdownSoundBlaster();
}

void InitializeSoundBlaster(void)
{
	int sb_detected = sb_detect();
	if (!sb_detected)
	{
		//printf("SB not found.\n");
	}
	else
	{
		//printf("SB found at A%x I%u D%u.\n", sb_base, sb_irq, sb_dma);
	}

	sb_init();
}

void ShutdownSoundBlaster(void)
{
	sb_deinit();
}

int sb_detect(void)
{
	byte temp;
	char* BLASTER;

	sb_base = 0;

	// Possible values: 210, 220, 230, 240, 250, 260, 280.
	for (temp = 1; temp < 9; ++temp)
	{
		if (temp != 7) // Skip 270.
		{
			if (reset_dsp(0x200 + (temp << 4)))
			{
				break;
			}
		}
	}

	if (temp == 9)
	{
		return 0;
	}

	BLASTER = getenv("BLASTER");
	sb_dma = 0;
	for (temp = 0; temp < strlen(BLASTER); ++temp)
	{
		if ((BLASTER[temp] | 32) == 'd')
		{
			sb_dma = BLASTER[temp + 1] - '0';
		}
	}

	for (temp = 0; temp < strlen(BLASTER); ++temp)
	{
		if ((BLASTER[temp] | 32) == 'i')
		{
			sb_irq = BLASTER[temp + 1] - '0';

			if (BLASTER[temp + 2] != ' ')
			{
				sb_irq = (sb_irq * 10) + BLASTER[temp + 2] - '0';
			}
		}
	}

	return sb_base;
}

int reset_dsp(word port)
{
	outp(port + SB_RESET, 1);
	delay(10);
	outp(port + SB_RESET, 0);
	delay(10);

	if (((inp(port + SB_READ_DATA_STATUS) & 0x80) != 0) &&
	    (inp(port + SB_READ_DATA) == 0x0aa))
	{
		sb_base = port;
		return 1;
	}

	return 0;
}

void sb_init(void)
{
	init_irq();
	assign_dma_buffer();
	write_dsp(SB_ENABLE_SPEAKER);
}

void init_irq(void)
{
	unsigned int index;

	// NOTE: Sets globals IrqTableIndex, PicBitMask.
	PicBitMask = 0;

	for (index = 0; index < (sizeof(SpecialSbIrqNumbers) / sizeof(byte)); ++index)
	{
		if (SpecialSbIrqNumbers[index] == sb_irq)
		{
			IrqTableIndex = SpecialSbIrqTableIndexes[index];
			PicBitMask = SpecialSbIrqPicBitMasks[index];
			break;
		}
	}

	if (PicBitMask == 0)
	{
		IrqTableIndex = sb_irq + 8;
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
		outp(0x21, inp(0x21) & !(1 << sb_irq));
	}
}

void deinit_irq(void)
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
		outp(0x21, inp(0x21) | (1 << sb_irq));
	}
}

void assign_dma_buffer(void)
{
	byte* temp_buf;
	long linear_address;
	word page1, page2;

	temp_buf = (byte*)malloc(32768);

	linear_address = FP_SEG(temp_buf);
	linear_address = (linear_address << 4) + FP_OFF(temp_buf);

	page1 = linear_address >> 16;
	page2 = (linear_address + 32768 - 1) >> 16;
	if (page1 != page2)
	{
		dma_buffer = (byte*)malloc(32768);
		free(temp_buf);
	}
	{
		dma_buffer = temp_buf;
	}

	linear_address = FP_SEG(dma_buffer);
	linear_address = (linear_address << 4) + FP_OFF(dma_buffer);

	page = linear_address >> 16;
	offset = linear_address & 0xffff;
}

void sb_deinit(void)
{
	write_dsp(SB_DISABLE_SPEAKER);
	free(dma_buffer);
	deinit_irq();
}

void write_dsp(byte command)
{
	while ((inp(sb_base + SB_WRITE_DATA) & 0x80) != 0);

	outp(sb_base + SB_WRITE_DATA, command);
}

// sox Communicator.wav -b 8 -r 11000 -e unsigned -L sound.raw
// mplayer -rawaudio samplesize=1:channels=1:rate=11000 -demuxer rawaudio sound.raw
void sb_single_play(const char* file_name)
{
	FILE* raw_file;
	int file_size;

	memset(dma_buffer, 0, 32768);

	raw_file = fopen(file_name, "rb");
	if (raw_file == NULL)
	{
		//playing = 0;
		return;
	}
	
	fseek(raw_file, 0l, SEEK_END);
	file_size = ftell(raw_file);
	
	fseek(raw_file, 0l, SEEK_SET);
	fread(dma_buffer, 1, file_size, raw_file);
	write_dsp(SB_SET_PLAYBACK_FREQUENCY);
	write_dsp((256 - 1000000) / 11000);
	to_be_played = file_size;

	single_cycle_playback();
}

void single_cycle_playback(void)
{
	playing = 1;

	// Program the DMA controller.
	outp(MASK_REGISTER, 0x04 | sb_dma);
	outp(MSB_LSB_FLIP_FLOP, 0);
	outp(MODE_REGISTER, 0x48 | sb_dma);
	outp(sb_dma << 1, offset & 0xff);
	outp(sb_dma << 1, (offset >> 8) & 0xff);
	
	switch (sb_dma)
	{
		case 0:
		{
			outp(DMA_CHANNEL_0, page);
			break;
		}

		case 1:
		{
			outp(DMA_CHANNEL_1, page);
			break;
		}

		case 3:
		{
			outp(DMA_CHANNEL_3, page);
			break;
		}
	}

	outp((sb_dma << 1) + 1, to_be_played & 0xff);
	outp((sb_dma << 1) + 1, (to_be_played >> 8) & 0xff);
	outp(MASK_REGISTER, sb_dma);
	write_dsp(SB_SINGLE_CYCLE_PLAYBACK);
	write_dsp(to_be_played & 0xff);
	write_dsp((to_be_played >> 8) & 0xff);
	to_be_played = 0;
}

void _WCINTERRUPT FAR AudioInterrupt(void)
{
	inp(sb_base + SB_READ_DATA_STATUS);
	outp(0x20, 0x20);

	if (PicBitMask != 0)
	{
		outp(0xa0, 0x20);
	}

	playing = 0;
}

void SoundKillAll(void)
{
	
}

void PlayAudioPattern(byte voiceIndex, const byte* voiceStart, byte looping)
{
	
}

void StopAudioPattern(byte voiceIndex)
{

}
