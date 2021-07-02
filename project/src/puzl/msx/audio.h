#ifndef PUZL_MSX_AUDIO_H
#define PUZL_MSX_AUDIO_H

#include <puzl.h>

#include <audio.h>

#define NUMBER_OF_VOICES 3

void InitializeMusicEngine(void);

void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex);
void EnableVoice(unsigned int voiceIndex);
void DisableVoice(unsigned int voiceIndex);

#endif
