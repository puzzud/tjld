#include <nds/audio.h>

#include <string.h>

#include <nds.h>
#include <nds/audio.h>

#include <puzl.h>

#include <c/music_engine.c>

// Runs from C1 to B6.
const word MusicEngineNoteFrequencyTable[] =
{
	33, // C1
	35,
	37,
	39,
	41,
	44,
	46,
	49,
	52,
	55,
	58,
	62,
	65, // C2
	69,
	73,
	78,
	82,
	87,
	93,
	98,
	104,
	110,
	117,
	123,
	131,
	139, // C3
	147,
	156,
	165,
	175,
	185,
	196,
	208,
	220,
	233,
	247,
	262, // C4
	277,
	294,
	311,
	330,
	349,
	370,
	392,
	415,
	440,
	466,
	494,
	523, // C5
	554,
	587,
	622,
	659,
	698,
	740,
	784,
	830,
	880,
	932,
	988,
	1047, // C6
	1109,
	1175,
	1245,
	1319,
	1397,
	1480,
	1568,
	1661,
	1760,
	1865,
	1976 // B6
};

int VoiceChannelIds[NUMBER_OF_VOICES];
word VoiceChannelFrequencies[NUMBER_OF_VOICES];

// NDS Specific.
void InitializeMusicEngine(void)
{
	unsigned int index;

	BaseInitializeMusicEngine();

	for (index = 0; index < NUMBER_OF_VOICES; ++index)
	{
		VoiceChannelIds[index] = -1;
	}

	memset(VoiceChannelFrequencies, 0, sizeof(VoiceChannelFrequencies));
}

void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex)
{
	word frequency = MusicEngineNoteFrequencyTable[frequencyIndex] * 8; // NOTE: Need to multiple by 8 for some reason to get the right pitch.

	int channelId = VoiceChannelIds[voiceIndex];
	if (channelId >= 0)
	{
		soundSetFreq(channelId, frequency);
	}

	VoiceChannelFrequencies[voiceIndex] = frequency;
}

void EnableVoice(unsigned int voiceIndex)
{
	int channelId = VoiceChannelIds[voiceIndex];
	if (channelId < 0)
	{
		VoiceChannelIds[voiceIndex] = soundPlayPSG(DutyCycle_50, VoiceChannelFrequencies[voiceIndex], 32, 64);
	}
}

void DisableVoice(unsigned int voiceIndex)
{
	int channelId = VoiceChannelIds[voiceIndex];
	if (channelId >= 0)
	{
		soundKill(channelId);

		VoiceChannelIds[voiceIndex] = -1;
	}
}
