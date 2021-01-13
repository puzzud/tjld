#include <sdl/audio.h>

#include <SDL2/SDL.h>
#include <math.h>

#include <puzl.h>

#include <c/music_engine.c>

// SDL Specific.
void InitializeMusicEngine(void)
{
	BaseInitializeMusicEngine();
}

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
