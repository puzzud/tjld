#ifndef AUDIO_H
#define AUDIO_H

#include <puzl.h>

extern void SoundKillAll(void);

extern void SetMusicVoice(byte voiceIndex, const byte* voiceStart);
extern void StartMusic(void);

#endif
