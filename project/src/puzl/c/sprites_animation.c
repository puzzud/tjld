#include <puzl.h>

#include <c/sequencer.h>

void BaseInitializeSpritesAnimation(void);
void ProcessSpriteAnimationDatum(unsigned int sequenceIndex, byte sequenceFetchDatum);

void BaseInitializeSpritesAnimation(void)
{
	unsigned int index;

	// Initialize animation.
	// NOTE: This could just be referenced in an array at compile time.
	ProcessSequenceDatum[SEQUENCE_TYPE_ANIMATION] = &ProcessSpriteAnimationDatum;
	OnSequenceSegmentEnd[SEQUENCE_TYPE_ANIMATION] = NULL;

	for (index = 0; index < NUMBER_OF_SPRITES; ++index)
	{
		// Set all sprite animation IDs to -1.
		// NOTE: Hacky way to indicate initial unset
		// values for animation IDs, but it's fairly efficient
		// for 8 bit platform implementations.
		Sprites[index].animationId = -1;
	}
}

void SetSpriteAnimationSet(const byte** animationSet)
{
	Sprites[CurrentSpriteIndex].animationSet = animationSet;
}

void PlaySpriteAnimation(byte animationId, byte looping)
{
	const byte* animationStart;

	if (Sprites[CurrentSpriteIndex].animationId == animationId)
	{
		return;
	}

	Sprites[CurrentSpriteIndex].animationId = animationId;

	animationStart = Sprites[CurrentSpriteIndex].animationSet[animationId];

	// TODO: Properly determine sequence from sprite index.
	PlaySequence(CurrentSpriteIndex + 3, animationStart, looping);
}

void StopSpriteAnimation(void)
{
	// TODO: Properly determine sequence from sprite index.
	StopSequence(CurrentSpriteIndex + 3);
}

void ProcessSpriteAnimationDatum(unsigned int sequenceIndex, byte sequenceFetchDatum)
{
	byte currentSpriteIndexBackup = CurrentSpriteIndex;
	
	CurrentSpriteIndex = sequenceIndex - 3; // TODO: Properly determine sprite index from sequence index.

	// Cutoff bit 7.
	// The first seven bits of this byte are the animation frame index.
	SetSpriteFrameIndex(sequenceFetchDatum & 0x7f); // %01111111

	// Now check bit 7.
	if ((sequenceFetchDatum & 0x80) != 0) // %10000000
	{
		// Fetch and store next byte.
		// Increase music pointer.
		SequenceSegmentDuration[sequenceIndex] = SequenceStart[sequenceIndex][++SequencePosition[sequenceIndex]];
	}

	SequenceSegmentDurationCounter[sequenceIndex] = SequenceSegmentDuration[sequenceIndex];

	CurrentSpriteIndex = currentSpriteIndexBackup;
}
