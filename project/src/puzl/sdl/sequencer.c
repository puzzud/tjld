#include <sdl/sequencer.h>

#include <SDL2/SDL.h>

#include <puzl.h>

const byte* SequenceStart[NUMBER_OF_SEQUENCES];
byte SequenceLooping[NUMBER_OF_SEQUENCES];

byte SequenceStatus[NUMBER_OF_SEQUENCES];
unsigned int SequencePosition[NUMBER_OF_SEQUENCES];
byte SequenceSegmentDuration[NUMBER_OF_SEQUENCES];
byte SequenceSegmentDurationCounter[NUMBER_OF_SEQUENCES];

SequenceType SequenceTypes[NUMBER_OF_SEQUENCES] =
{
	SEQUENCE_TYPE_MUSIC,
	SEQUENCE_TYPE_MUSIC,
	SEQUENCE_TYPE_MUSIC,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION,
	SEQUENCE_TYPE_ANIMATION
};

unsigned int SequenceChannelIds[NUMBER_OF_SEQUENCES] =
{
	0,
	1,
	2,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7
};

void (*ProcessSequenceDatum[NUMBER_OF_SEQUENCE_TYPES])(unsigned int channelId, byte sequenceFetchDatum) =
{
	NULL,
	NULL
};

void (*OnSequenceSegmentEnd[NUMBER_OF_SEQUENCE_TYPES])(unsigned int channelId) =
{
	NULL,
	NULL
};

void ProcessSequence(unsigned int sequenceIndex);
void ProcessSequenceData(unsigned int sequenceIndex);

// ---------------------------------------
void InitializeSequencer(void)
{
	unsigned int index;

	for (index = 0; index < NUMBER_OF_SEQUENCES; ++index)
	{
		StopSequence(index);
	}

	for (index = 0; index < NUMBER_OF_SEQUENCE_TYPES; ++index)
	{
		ProcessSequenceDatum[index] = NULL;
		OnSequenceSegmentEnd[index] = NULL;
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
	SequenceSegmentDurationCounter[sequenceIndex] = 0;
	SequenceStatus[sequenceIndex] = 0;
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
	void (*onSequenceSegmentEnd)(unsigned int channelId);

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
		onSequenceSegmentEnd = OnSequenceSegmentEnd[SequenceTypes[sequenceIndex]];
		if (onSequenceSegmentEnd != NULL)
		{
			onSequenceSegmentEnd(SequenceChannelIds[sequenceIndex]);
		}

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

	ProcessSequenceDatum[SequenceTypes[sequenceIndex]](sequenceIndex, sequenceFetchDatum);

	++SequencePosition[sequenceIndex];
}
