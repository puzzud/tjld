#ifndef PUZL_SDL_AUDIO_H
#define PUZL_SDL_AUDIO_H

#include <puzl.h>

#include <audio.h>

#define NUMBER_OF_VOICES 3

extern byte VoiceEnabled[NUMBER_OF_VOICES];
extern word VoicePitch[NUMBER_OF_VOICES];

extern void UpdateAudio(void);

extern void PlayAudioPattern(byte voiceIndex, const byte* voiceStart, byte looping);
extern void StopAudioPattern(byte voiceIndex);

void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex);
void EnableVoice(unsigned int voiceIndex);
void DisableVoice(unsigned int voiceIndex);

#endif
