#include <sdl/audio.h>

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include <puzl.h>

SDL_AudioSpec AudioSpec;

SDL_AudioDeviceID AudioDeviceId;

byte Voice1Enabled;
word Voice1Pitch;
unsigned int Voice1WaveCounter;

void AudioCallback(void* userData, Uint8* audioData, int length);

void InitializeAudio(void)
{
    SDL_AudioSpec requestedAudioSpec;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        fprintf(stderr, "Failed to initialize audio: %s\n", SDL_GetError());

        return;
    }

    SDL_zero(requestedAudioSpec);
    requestedAudioSpec.freq = 44100; // 44100 samples per second is 44.1 hKz.
    requestedAudioSpec.format = AUDIO_F32SYS; // Sample format: 32-bit float (system-defined byte order).
    requestedAudioSpec.channels = 1; // 1 (mono).
    requestedAudioSpec.samples = 4096 / 4; // Buffer size in samples.
    requestedAudioSpec.userdata = NULL;
    requestedAudioSpec.callback = &AudioCallback;

    AudioDeviceId = SDL_OpenAudioDevice(NULL, 0, &requestedAudioSpec, &AudioSpec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    if (AudioDeviceId == 0)
    {
        fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());

        return;
    }

    if (AudioSpec.format != requestedAudioSpec.format)
    {
        fprintf(stderr, "Failed to acquire audio format AUDIO_F32SYS: %s\n", SDL_GetError());

        return;
    }

    Voice1Pitch = 0;
    Voice1WaveCounter = 0;

    SDL_PauseAudioDevice(AudioDeviceId, 0);
}

void ShutdownAudio(void)
{
    SDL_CloseAudioDevice(AudioDeviceId);

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void UpdateAudio(void)
{
    ProcessMusic();
}

void SoundKillAll(void)
{
	Voice1Enabled = 0;
    Voice1Pitch = 0;
}

void AudioCallback(void* userData, Uint8* audioData, int length)
{
    float* floatStream = (float*)audioData;
    const unsigned int numberOfSamples = (length / sizeof(float));
    unsigned int sampleIndex;

    // Generate samples.
    for (sampleIndex = 0; sampleIndex < numberOfSamples; ++sampleIndex)
    {
        // Generate one sample and put it into the buffer.

        if (Voice1Enabled == 0)
        {
            floatStream[sampleIndex] = AudioSpec.silence;
        }
        else
        {
            // Generate samples of a square-wave that alternates between signal levels -0.5 and +0.5.
            floatStream[sampleIndex] = sinf(Voice1WaveCounter * 2 * M_PI / AudioSpec.freq);
        }
        
        Voice1WaveCounter += Voice1Pitch;
    }
}
