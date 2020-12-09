#ifndef AUDIO_H
#define AUDIO_H

#include <puzl.h>

extern void InitializeAudio(void);
extern void ShutdownAudio(void);
extern void UpdateAudio(void);

extern void SoundKillAll(void);

extern void SetMusicVoice(byte voiceIndex, const byte* voiceStart);
extern void StartMusic(void);

#endif
