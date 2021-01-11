#ifndef PUZL_MSDOS_AUDIO_H
#define PUZL_MSDOS_AUDIO_H

#include <puzl.h>

#include <audio.h>

#define PORT_ADLIB_STATUS 0x388
#define PORT_ADLIB_DATA   0x389

#define NUMBER_OF_ADLIB_CHANNELS 16

#define NUMBER_OF_VOICES 3

void InitializeMusicEngine(void);

void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex);
void EnableVoice(unsigned int voiceIndex);
void DisableVoice(unsigned int voiceIndex);

#endif
