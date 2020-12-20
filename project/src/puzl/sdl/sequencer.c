#include <sdl/sequencer.h>

#include <SDL2/SDL.h>

#include <puzl.h>

const byte* SequenceStart[NUMBER_OF_SEQUENCES];
byte SequenceLooping[NUMBER_OF_SEQUENCES];

byte SequenceStatus[NUMBER_OF_SEQUENCES];
unsigned int SequencePosition[NUMBER_OF_SEQUENCES];
byte SequenceSegmentDuration[NUMBER_OF_SEQUENCES];
byte SequenceSegmentDurationCounter[NUMBER_OF_SEQUENCES];

void ProcessSequence(unsigned int sequenceIndex);
void ProcessSequenceData(unsigned int sequenceIndex);

// ---------------------------------------
void InitializeSequencer(void)
{
	unsigned int sequenceIndex;

	for (sequenceIndex = 0; sequenceIndex < NUMBER_OF_SEQUENCES; ++sequenceIndex)
	{
		StopSequence(sequenceIndex);
	}
}

// ---------------------------------------
void PlaySequence(byte sequenceIndex, const byte* sequenceStart, byte looping)
{
	SequenceStart[sequenceIndex] = sequenceStart;

	// TODO: These zeroing could be consolidated with reset that happens during ProcessSequences.
	SequencePosition[sequenceIndex] = 0;

	// Disable all sequence processing.
	SequenceSegmentDurationCounter[sequenceIndex] = 0;

	SequenceStatus[sequenceIndex] = -1; // 0xff

	SequenceLooping[sequenceIndex] = looping;
}

// ---------------------------------------
void StopSequence(byte sequenceIndex)
{
	// Disable all sequence processing.
	for (sequenceIndex = 0; sequenceIndex < NUMBER_OF_SEQUENCES; ++sequenceIndex)
	{
		SequenceSegmentDurationCounter[sequenceIndex] = 0;
		SequenceStatus[sequenceIndex] = 0;
	}
}

// Start of sequence processing.
void ProcessSequences(void)
{
	unsigned int sequenceIndex;

	// Start processing.
	for (sequenceIndex = 0; sequenceIndex < NUMBER_OF_SEQUENCES; ++sequenceIndex)
	{
		if (SequenceStatus[sequenceIndex] != 0)
		{
			ProcessSequence(sequenceIndex);
		}
	}
}

void ProcessSequence(unsigned int sequenceIndex)
{
	if (SequenceSegmentDurationCounter[sequenceIndex] == 0)
	{
		ProcessSequenceData(sequenceIndex);
	}
	
	if (SequenceSegmentDurationCounter[sequenceIndex] != 1)
	{
		--SequenceSegmentDurationCounter[sequenceIndex];
	}
	else
	{
		// TODO: Abstract this hook, as it pretty much only applies to audio.
		DisableVoice(sequenceIndex);

		SequenceSegmentDurationCounter[sequenceIndex] = 0;
	}
}

void ProcessSequenceData(unsigned int sequenceIndex)
{
	// Fetch current byte and cache for later analysis.
	byte sequenceFetchDatum = SequenceStart[sequenceIndex][SequencePosition[sequenceIndex]];
	if (sequenceFetchDatum == 0)
	{
		if (SequenceLooping[sequenceIndex] == 0)
		{
			// No looping, so do no fetching.
			return;
		}
		else
		{
			// Reset sequencer to start position.
			SequencePosition[sequenceIndex] = 0;
			SequenceSegmentDurationCounter[sequenceIndex] = 0;

			// Fetch first datum.
			sequenceFetchDatum = SequenceStart[sequenceIndex][SequencePosition[sequenceIndex]];
		}
	}

	ProcessSequenceDatum(sequenceIndex, sequenceFetchDatum);

	++SequencePosition[sequenceIndex];
}
