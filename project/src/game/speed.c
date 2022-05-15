#include <puzl.h>

// TODO: Hide this pragma from common code.
#pragma bss-name (push,"ZEROPAGE")
byte SpeedBitPatternByteTimer;
byte SpeedBitFlag;
#pragma bss-name (pop)

// NOTE: End bytes are reversed to get the same pattern as shown in comments.
const byte SpeedBitPatterns[9][NUMBER_OF_SPEED_BIT_PATTERN_BYTES] =
{
	{0x00, 0x00, 0x00}, // 00000000 00000000 00000000 = 0/8 = 0
	{0x01, 0x01, 0x01}, // 00000001 00000001 00000001 = 1/8
	{0x11, 0x11, 0x11}, // 00010001 00010001 00010001 = 2/8 = 1/4
	{0x45, 0x92, 0x24}, // 00100100 10010010 01001001 = 3/8
	{0x55, 0x55, 0x55}, // 01010101 01010101 01010101 = 4/8 = 1/2
	{0xdb, 0xb6, 0x6d}, // 01101101 10110110 11011011 = 5/8
	{0x77, 0x77, 0x77}, // 01110111 01110111 01110111 = 6/8 = 3/4
	{0xef, 0xef, 0xef}, // 01111111 01111111 01111111 = 7/8
	{0xff, 0xff, 0xff}  // 11111111 11111111 11111111 = 8/8 = 1
};

void FASTCALL InitalizeSpeed(void)
{
	SpeedBitPatternByteTimer = NUMBER_OF_SPEED_BIT_PATTERN_BYTES - 1;
	SpeedBitFlag = 0x80;
}

void FASTCALL CycleSpeedBit(void)
{
		SpeedBitFlag >>= 1;
		if (SpeedBitFlag == 0x00)
		{
			if (--SpeedBitPatternByteTimer == 0)
			{
				InitalizeSpeed();
			}
			else
			{
				SpeedBitFlag = 0x80;
			}
		}
}

byte FASTCALL CanMove(byte speedPatternIndex)
{
	return ((SpeedBitPatterns[speedPatternIndex][SpeedBitPatternByteTimer] & SpeedBitFlag) == 0) ? 0 : 1;
}
