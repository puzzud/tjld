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

byte Volume;

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

	Volume = 15;

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
	float volumeMultiplier = (((float)Volume / 15.0f) / 16.0f);
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
			// Generate samples of a sine-wave that alternates between signal levels -0.1 and +0.1.
			floatStream[sampleIndex] = sinf(Voice1WaveCounter * 2 * M_PI / AudioSpec.freq);

			// Generate samples of a square-wave that alternates between signal levels -0.1 and +0.1.
			//floatStream[sampleIndex] = sinf(Voice1WaveCounter * 2 * M_PI / AudioSpec.freq) > 0.0 ? 1.0 : -1.0;

			// Generate samples of a triangle-wave that alternates between signal levels -0.1 and +0.1.
			//floatStream[sampleIndex] = asinf(sinf(Voice1WaveCounter * 2 * M_PI / AudioSpec.freq)) * (2.0f / M_PI);

			// Generate samples of a saw-wave that alternates between signal levels -0.1 and +0.1.
			//floatStream[sampleIndex] = ((float)Voice1Pitch * M_PI * fmodf(Voice1WaveCounter, 1.0f / (float)Voice1Pitch) - (M_PI / 2.0f)) * (2.0f / M_PI);
			/*
			floatStream[sampleIndex] = 0.0f;
			for (float n = 1.0f; n < 40.0f; ++n)
			{
			floatStream[sampleIndex] += (sinf(n * (Voice1WaveCounter * 2 * M_PI / AudioSpec.freq))) / n;
			}
			floatStream[sampleIndex] *= (2.0 / M_PI);
			*/

			// Generate samples of a noise-wave that alternates between signal levels -0.1 and +0.1.
			// TODO: Figure out how to tie in pitch.
			//floatStream[sampleIndex] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;

			floatStream[sampleIndex] *= volumeMultiplier;
		}

		Voice1WaveCounter += Voice1Pitch;
	}
}
