#ifndef AUDIO_H
#define AUDIO_H

#include <puzl.h>

extern void InitializeAudio(void);
extern void ShutdownAudio(void);
extern void UpdateAudio(void);

extern void SoundKillAll(void);

extern void PlayAudioPattern(byte voiceIndex, const byte* voiceStart);
extern void StopAudioPattern(byte voiceIndex);

#endif
