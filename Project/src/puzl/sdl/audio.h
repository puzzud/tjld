#ifndef PUZL_SDL_AUDIO_H
#define PUZL_SDL_AUDIO_H

#include <puzl.h>

#include <audio.h>

extern byte Voice1Enabled;
extern word Voice1Pitch;

extern void UpdateAudio(void);
extern void ProcessMusic(void);

#endif
