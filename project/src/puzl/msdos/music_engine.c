#include <msdos/audio.h>

#include <conio.h>

#include <puzl.h>

#include <c/music_engine.c>

const word MusicEngineNoteFrequencyTable[] =
{
	0x157, //       C
	0x16b, // 277.2	C#
	0x181, // 293.7	D
	0x198, // 311.1	D#
	0x1b0, // 329.6	E
	0x1ca, // 349.2	F
	0x1e5, // 370.0	F#
	0x202, // 392.0	G
	0x220, // 415.3	G#
	0x241, // 440.0	A
	0x263, // 466.2	A#
	0x287, // 493.9	B
	//0x2ae, // 523.3	C 
};

byte VoiceA0ShadowRegisters[NUMBER_OF_VOICES];
byte VoiceB0ShadowRegisters[NUMBER_OF_VOICES];

// MSDOS Specific.
void InitializeMusicEngine(void)
{
	unsigned int voiceIndex;

	// NOTE: Not really too MSDOS specific at this point.
	ProcessSequenceDatum[SEQUENCE_TYPE_MUSIC] = &ProcessAudioDatum;
	OnSequenceSegmentEnd[SEQUENCE_TYPE_MUSIC] = &DisableVoice;

	for (voiceIndex = 0; voiceIndex < NUMBER_OF_VOICES; ++voiceIndex)
	{	
		VoiceA0ShadowRegisters[voiceIndex] = 0;
		VoiceB0ShadowRegisters[voiceIndex] = 0;
	}
}

void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex)
{
	word frequency = MusicEngineNoteFrequencyTable[frequencyIndex % 12];
	byte octaveIndex = (frequencyIndex / 12) + 1;

	VoiceA0ShadowRegisters[voiceIndex] = frequency & 0xff;
	VoiceB0ShadowRegisters[voiceIndex] = (VoiceB0ShadowRegisters[voiceIndex] & 0x20) | (octaveIndex << 2) | ((frequency >> 8) & 0x03);

	// Frequency (LSB).
	outp(PORT_ADLIB_STATUS, 0xa0 + voiceIndex);
	outp(PORT_ADLIB_DATA, VoiceA0ShadowRegisters[voiceIndex]);

	// Octave (bits 2-4).
	// Frequency (Most significant 2 bits, bits 0-1).
	outp(PORT_ADLIB_STATUS, 0xb0 + voiceIndex);
	outp(PORT_ADLIB_DATA, VoiceB0ShadowRegisters[voiceIndex]);
}

void EnableVoice(unsigned int voiceIndex)
{
	VoiceB0ShadowRegisters[voiceIndex] |= 0x20;

	// Key On (5th bit).
	outp(PORT_ADLIB_STATUS, 0xb0 + voiceIndex);
	outp(PORT_ADLIB_DATA, VoiceB0ShadowRegisters[voiceIndex]);
}

void DisableVoice(unsigned int voiceIndex)
{
	VoiceB0ShadowRegisters[voiceIndex] &= ~0x20;

	// Key On (5th bit).
	outp(PORT_ADLIB_STATUS, 0xb0 + voiceIndex);
	outp(PORT_ADLIB_DATA, VoiceB0ShadowRegisters[voiceIndex]);
}
