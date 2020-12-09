#include <sdl/audio.h>

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include <puzl.h>

SDL_AudioSpec AudioSpec;

SDL_AudioDeviceID AudioDeviceId;

byte VoiceEnabled[NUMBER_OF_VOICES];
word VoicePitch[NUMBER_OF_VOICES];
unsigned int VoiceWaveCounter[NUMBER_OF_VOICES];

byte Volume;

void AudioCallback(void* userData, Uint8* audioData, int length);

float GenerateVoiceSampleSine(unsigned int voiceIndex);
float GenerateVoiceSampleSquare(unsigned int voiceIndex);
float GenerateVoiceSampleTriangle(unsigned int voiceIndex);
float GenerateVoiceSampleSaw(unsigned int voiceIndex);
float GenerateVoiceSampleNoise(unsigned int voiceIndex);

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

	SoundKillAll();

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
	unsigned int voiceIndex;

	for (voiceIndex = 0; voiceIndex < NUMBER_OF_VOICES; ++voiceIndex)
	{
		VoiceEnabled[voiceIndex] = 0;
		VoicePitch[voiceIndex] = 0;
		VoiceWaveCounter[voiceIndex] = 0;
	}
}

void AudioCallback(void* userData, Uint8* audioData, int length)
{
	float* floatStream = (float*)audioData;
	float volumeMultiplier = (((float)Volume / 15.0f) / 32.0f);
	const unsigned int numberOfSamples = (length / sizeof(float));
	unsigned int sampleIndex;
	unsigned int voiceIndex;

	// Generate samples.
	for (sampleIndex = 0; sampleIndex < numberOfSamples; ++sampleIndex)
	{
		// Generate one sample and put it into the buffer.
		floatStream[sampleIndex] = 0.0f;

		for (voiceIndex = 0; voiceIndex < NUMBER_OF_VOICES; ++voiceIndex)
		{
			floatStream[sampleIndex] += GenerateVoiceSampleSquare(voiceIndex);
		}

		floatStream[sampleIndex] *= volumeMultiplier;
	}
}

float GenerateVoiceSampleSine(unsigned int voiceIndex)
{
	// Generate samples of a sine-wave that alternates between signal levels -0.1 and +0.1.
	float sample = AudioSpec.silence;
	
	if (VoiceEnabled[voiceIndex] != 0)
	{
		sample = sinf(VoiceWaveCounter[voiceIndex] * 2 * M_PI / AudioSpec.freq);
	}
	
	VoiceWaveCounter[voiceIndex] += VoicePitch[voiceIndex];
	
	return sample;
}

float GenerateVoiceSampleSquare(unsigned int voiceIndex)
{
	// Generate samples of a square-wave that alternates between signal levels -0.1 and +0.1.
	float sample = AudioSpec.silence;
	
	if (VoiceEnabled[voiceIndex] != 0)
	{
		sample = sinf(VoiceWaveCounter[voiceIndex] * 2 * M_PI / AudioSpec.freq) > 0.0 ? 1.0 : -1.0;
	}
	
	VoiceWaveCounter[voiceIndex] += VoicePitch[voiceIndex];
	
	return sample;
}

float GenerateVoiceSampleTriangle(unsigned int voiceIndex)
{
	// Generate samples of a triangle-wave that alternates between signal levels -0.1 and +0.1.
	float sample = AudioSpec.silence;
	
	if (VoiceEnabled[voiceIndex] != 0)
	{
		sample = asinf(sinf(VoiceWaveCounter[voiceIndex] * 2 * M_PI / AudioSpec.freq)) * (2.0f / M_PI);
	}

	VoiceWaveCounter[voiceIndex] += VoicePitch[voiceIndex];

	return sample;
}

float GenerateVoiceSampleSaw(unsigned int voiceIndex)
{
	// Generate samples of a saw-wave that alternates between signal levels -0.1 and +0.1.
	float sample = AudioSpec.silence;
	
	if (VoiceEnabled[voiceIndex] != 0)
	{
		//floatStream[sampleIndex] = ((float)VoicePitch[voiceIndex] * M_PI * fmodf(VoiceWaveCounter[voiceIndex], 1.0f / (float)VoicePitch[voiceIndex]) - (M_PI / 2.0f)) * (2.0f / M_PI);
		sample = 0.0f;

		for (float n = 1.0f; n < 40.0f; ++n)
		{
			sample += (sinf(n * (VoiceWaveCounter[voiceIndex] * 2 * M_PI / AudioSpec.freq))) / n;
		}

		sample *= (2.0 / M_PI);
	}
	
	VoiceWaveCounter[voiceIndex] += VoicePitch[voiceIndex];

	return sample;
}

float GenerateVoiceSampleNoise(unsigned int voiceIndex)
{
	// Generate samples of a noise-wave that alternates between signal levels -0.1 and +0.1.
	float sample = AudioSpec.silence;

	if (VoiceEnabled[voiceIndex] != 0)
	{
		// TODO: Figure out how to tie in pitch.
		sample = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	}

	VoiceWaveCounter[voiceIndex] += VoicePitch[voiceIndex];

	return sample;
}
