#ifndef PUZL_SDL_SEQUENCER_H
#define PUZL_SDL_SEQUENCER_H

#include <core.h>
#include <sdl/audio.h>

typedef enum
{
	SEQUENCE_TYPE_MUSIC,
	SEQUENCE_TYPE_ANIMATION,

	NUMBER_OF_SEQUENCE_TYPES
} SequenceType;

#define NUMBER_OF_SEQUENCES  (NUMBER_OF_VOICES)

extern const byte* SequenceStart[NUMBER_OF_SEQUENCES];

extern unsigned int SequencePosition[NUMBER_OF_SEQUENCES];
extern byte SequenceSegmentDuration[NUMBER_OF_SEQUENCES];
extern byte SequenceSegmentDurationCounter[NUMBER_OF_SEQUENCES];

extern void (*ProcessSequenceDatum[NUMBER_OF_SEQUENCE_TYPES])(unsigned int channelId, byte sequenceFetchDatum);
extern void (*OnSequenceSegmentEnd[NUMBER_OF_SEQUENCE_TYPES])(unsigned int channelId);

void InitializeSequencer(void);

extern void ProcessSequences(void);
extern void PlaySequence(byte sequenceIndex, const byte* sequenceStart, byte looping);
extern void StopSequence(byte sequenceIndex);

extern void SetVoiceFrequency(unsigned int voiceIndex, unsigned int frequencyIndex);
extern void EnableVoice(unsigned int voiceIndex);
extern void DisableVoice(unsigned int voiceIndex);

#endif
