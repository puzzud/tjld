#include <sdl/audio.h>

#include <SDL2/SDL.h>
#include <math.h>

#include <puzl.h>
#include <c/sequencer.h>

// MULE music engine.
// 
// It provides the following major routines:
// StartAudioPattern

// The encoding for the music is as follows:
// - The first byte fetched will always contain an index to a note table, based on the lower 6 bits.
// - The first byte's 7th bit indicates a duration change (how much music time this byte takes up).
//   - If a duration change is indicated, a second byte is fetched.
//   - The second byte's value acts as a delay (duration) counter, which is decremented every music engine frame.
//   - While this duration counter is positive, no more bytes from the corresponding voice's music data are processed.
//   - Once the duration is zero (non-positive), music data fetching commences, restarting the cycle with the first byte fetch.
// - The first byte's 6th bit indicates a voice gate change.
//   - High will start the silencing or release of the voice.
//   - Low will either start the voice attack or will maintain the voice's active status.
// - Unless a duration change is made, all byte fetches are considered the 'first' byte.

// - Durations:
//   - $0c: 1/8 note.
//   - $c0: 8 beats rest.
#define NOTE_LENGTH_1_96TH 1
#define NOTE_LENGTH_1_32ND NOTE_LENGTH_1_96TH * 3 // 3
#define NOTE_LENGTH_1_16TH NOTE_LENGTH_1_32ND * 2 // 6
#define NOTE_LENGTH_1_8TH  NOTE_LENGTH_1_16TH * 2 // 12
#define NOTE_LENGTH_1_4TH  NOTE_LENGTH_1_8TH  * 2 // 24
#define NOTE_LENGTH_1_HALF NOTE_LENGTH_1_4TH  * 2 // 48
#define NOTE_LENGTH_1      NOTE_LENGTH_1_HALF * 2 // 96

#define NOTE_CONTROL_LENGTH 0x80 // %10000000
#define NOTE_CONTROL_GATE   0x40 // %01000000

// Note tables:
#define NOTE_C  0
#define NOTE_CS 1
#define NOTE_D  2
#define NOTE_DS 3
#define NOTE_E  4
#define NOTE_F  5
#define NOTE_FS 6
#define NOTE_G  7
#define NOTE_GS 8
#define NOTE_A  9
#define NOTE_AS 10
#define NOTE_B  11

#define NUMBER_OF_NOTES_IN_OCTAVE 12
#define OCTAVE NUMBER_OF_NOTES_IN_OCTAVE

void ProcessAudioDatum(unsigned int sequenceIndex, byte sequenceFetchDatum);

void InitializeMusicEngine(void)
{
	ProcessSequenceDatum[SEQUENCE_TYPE_MUSIC] = &ProcessAudioDatum;
	OnSequenceSegmentEnd[SEQUENCE_TYPE_MUSIC] = &DisableVoice;
}

void PlayAudioPattern(byte voiceIndex, const byte* voiceStart, byte looping)
{
	PlaySequence(voiceIndex, voiceStart, looping);
}

void StopAudioPattern(byte voiceIndex)
{
	StopSequence(voiceIndex);
}

void ProcessAudioDatum(unsigned int sequenceIndex, byte sequenceFetchDatum)
{
	unsigned int voiceIndex = SequenceChannelIds[sequenceIndex];

	// Cutoff bits 6 & 7.
	// The first six bits of this byte are the music note index.
	// Load Voice Frequency.
	SetVoiceFrequency(voiceIndex, sequenceFetchDatum & 0x3f); // %00111111

	// Now check bit 7.
	if ((sequenceFetchDatum & 0x80) != 0) // %10000000
	{
		// Fetch and store next byte.
		// Increase music pointer.
		SequenceSegmentDuration[sequenceIndex] = SequenceStart[sequenceIndex][++SequencePosition[sequenceIndex]];
	}

	// Now check bit 6.
	if ((sequenceFetchDatum & 0x40) != 0) // %01000000
	{
		// Disable Voice.
		DisableVoice(voiceIndex);
	}
	else
	{
		// Gate Voice.
		EnableVoice(voiceIndex);
	}

	SequenceSegmentDurationCounter[sequenceIndex] = SequenceSegmentDuration[sequenceIndex];
}

// SDL Specific.
void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex)
{
	VoicePitch[voiceIndex] = (word)(pow(1.059463, (double)frequencyIndex - 45) * 440.0);
}

void EnableVoice(unsigned int voiceIndex)
{
	VoiceEnabled[voiceIndex] = 0xff;
}

void DisableVoice(unsigned int voiceIndex)
{
	VoiceEnabled[voiceIndex] = 0;
}
