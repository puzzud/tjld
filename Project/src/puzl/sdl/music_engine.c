#include <sdl/audio.h>

#include <SDL2/SDL.h>
#include <math.h>

#include <puzl.h>

void InitializeMusicEngine(void);
void SetMusicVoice(void);
void StartMusic(void);
void StopMusic(void);
void ProcessMusic(void);
void ProcessVoice(unsigned int voiceIndex);

void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex);
void EnableVoice(unsigned int voiceIndex);
void DisableVoice(unsigned int voiceIndex);

byte MusicStatus;

const byte* MusicEngineV1MusicStart;
const byte* MusicEngineV2MusicStart;
const byte* MusicEngineV3MusicStart;

byte MusicEngineV1TimeToRelease;
byte MusicEngineV2TimeToRelease;
byte MusicEngineV3TimeToRelease;

unsigned int MusicEngineV1Position;
unsigned int MusicEngineV2Position;
unsigned int MusicEngineV3Position;
byte MusicEngineV1Duration;
byte MusicEngineV2Duration;
byte MusicEngineV3Duration;
byte MusicEngineV1Active;
byte MusicEngineV2Active;
byte MusicEngineV3Active;
byte MusicEngineV1TimeToReleaseCounter;
byte MusicEngineV2TimeToReleaseCounter;
byte MusicEngineV3TimeToReleaseCounter;
byte MusicEngineV1DurationCounter;
byte MusicEngineV2DurationCounter;
byte MusicEngineV3DurationCounter;
//byte MusicEngineTempFetch;

// MULE music engine.
// 
// It provides the following major routines:
// InitializeMusic
// StartMusic
// StopMusic
// ProcessMusic

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

// ---------------------------------------
// InitializeMusicEngine
void InitializeMusicEngine(void)
{
	StopMusic();
}

// ---------------------------------------
void SetMusicVoice(byte voiceIndex, const byte* voiceStart)
{
	// TODO: Clean this up into arrays & pointers.
	switch (voiceIndex)
	{
		case 0:
		{
			MusicEngineV1MusicStart = voiceStart;
			break;
		}

		case 1:
		{
			MusicEngineV2MusicStart = voiceStart;
			break;
		}

		case 2:
		{
			MusicEngineV3MusicStart = voiceStart;
			break;
		}
	}
}

// ---------------------------------------
void StopMusic(void)
{
	// Disable all voice music processing.
	MusicEngineV1Active = 0;
	MusicEngineV2Active = 0;
	MusicEngineV3Active = 0;

	MusicStatus = 0;

	SoundKillAll();
}

// ---------------------------------------
void StartMusic(void)
{
	// TODO: Perhaps it's possible to integrate
	// release time better with NES voice timed envelopes?
	const byte musicEngineVoiceTimeToRelease = 0x1e;
	MusicEngineV1TimeToRelease = musicEngineVoiceTimeToRelease;
	MusicEngineV2TimeToRelease = musicEngineVoiceTimeToRelease;
	MusicEngineV3TimeToRelease = musicEngineVoiceTimeToRelease;

	// TODO: These zeroing could be consolidated with reset that happens curing ProcessMusic.
	MusicEngineV1Position = 0;
	MusicEngineV2Position = 0;
	MusicEngineV3Position = 0;

	// Disable all voice music processing.
	MusicEngineV1Active = 0;
	MusicEngineV2Active = 0;
	MusicEngineV3Active = 0;

	MusicStatus = -1; // 0xff
}

// Start of all voice/music processing.
void ProcessMusic(void)
{
	unsigned int voiceIndex;

	if (MusicStatus == 0)
	{
		return;
	}

	// Start processing.
	voiceIndex = 0;
	ProcessVoice(voiceIndex);
}

void ProcessVoice(unsigned int voiceIndex)
{
	byte musicEngineTempFetch;

	// Check voice.
	if (MusicEngineV1Active == 0)
	{
		if (MusicEngineV1MusicStart[MusicEngineV1Position] == 0)
		{
			// Reset music engine vectors to currently set base music data vectors.
			MusicEngineV1Position = 0;
			MusicEngineV1Active = 0;
		}

		// Process Voice.
		// Fetch current byte and cache for later for later analysis.
		musicEngineTempFetch = MusicEngineV1MusicStart[MusicEngineV1Position];

		// Cutoff bits 6 & 7.
  		// The first six bits of this byte are the music note index.
		// Load Voice Frequency.
		SetVoiceFrequency(voiceIndex, musicEngineTempFetch & 0x3f); // %00111111

		// Now check bit 7.
		if ((musicEngineTempFetch & 0x80) != 0) // %10000000
		{
			// Fetch and store next byte.
			// Increase music pointer.
			MusicEngineV1Duration = MusicEngineV1MusicStart[++MusicEngineV1Position];
		}

		++MusicEngineV1Position;

		// Now check bit 6.
		if ((musicEngineTempFetch & 0x40) != 0) // %01000000
		{
			// Disable Voice.
			DisableVoice(voiceIndex);
		}
		else
		{
			// Gate Voice.
			EnableVoice(voiceIndex);
		}

		MusicEngineV1TimeToReleaseCounter = MusicEngineV1TimeToRelease;
		MusicEngineV1DurationCounter = MusicEngineV1Duration;

		MusicEngineV1Active = 1;	
	}

	// Process music engine voice time to release and duration.
	if (MusicEngineV1TimeToReleaseCounter == 0)
	{
		// Disable Voice.
		DisableVoice(voiceIndex);
	}
	else
	{
		--MusicEngineV1TimeToReleaseCounter;
	}
	
	if (MusicEngineV1DurationCounter != 1)
	{
		--MusicEngineV1DurationCounter;
	}
	else
	{
		// Disable Voice.
		DisableVoice(voiceIndex);

		MusicEngineV1Active = 0;
	}
}

// SDL Specific.
void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex)
{
	Voice1Pitch = (word)(pow(1.059463, (double)frequencyIndex - 45) * 440.0);
}

void EnableVoice(unsigned int voiceIndex)
{
	Voice1Enabled = 0xff;
}

void DisableVoice(unsigned int voiceIndex)
{
	Voice1Enabled = 0;
}
