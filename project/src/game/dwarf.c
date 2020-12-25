#include <dwarf.h>

#include <puzl.h>

const byte DwarfSpriteAnimationLeftIdle[] =
{
	0x80 | DWARF_FRAME_ID_LEFT_IDLE, 255,
	0x00
};

const byte DwarfSpriteAnimationFrontIdle[] =
{
	0x80 | DWARF_FRAME_ID_FRONT_IDLE, 255,
	0x00
};

const byte DwarfSpriteAnimationRightIdle[] =
{
	0x80 | DWARF_FRAME_ID_RIGHT_IDLE, 255,
	0x00
};

const byte DwarfSpriteAnimationLeftWalk[] =
{
	0x80 | DWARF_FRAME_ID_LEFT_WALK_0, 4,
	DWARF_FRAME_ID_LEFT_WALK_1,
	DWARF_FRAME_ID_LEFT_WALK_2,
	0x00
};

const byte DwarfSpriteAnimationRightWalk[] =
{
	0x80 | DWARF_FRAME_ID_RIGHT_WALK_0, 4,
	DWARF_FRAME_ID_RIGHT_WALK_1,
	DWARF_FRAME_ID_RIGHT_WALK_2,
	0x00
};

const byte DwarfSpriteAnimationBackClimb[] =
{
	0x80 | DWARF_FRAME_ID_BACK_CLIMB_0, 4,
	DWARF_FRAME_ID_BACK_CLIMB_1,
	0x00
};

const byte DwarfSpriteAnimationLeftJump[] =
{
	0x80 | DWARF_FRAME_ID_LEFT_JUMP_PEAK, 255,
	0x00
};

const byte DwarfSpriteAnimationFrontJump[] =
{
	0x80 | DWARF_FRAME_ID_FRONT_JUMP, 255,
	0x00
};

const byte DwarfSpriteAnimationRightJump[] =
{
	0x80 | DWARF_FRAME_ID_RIGHT_JUMP_PEAK, 255,
	0x00
};

const byte* DwarfAnimationSet[] =
{
	DwarfSpriteAnimationLeftIdle,  // DWARF_ANIMATION_ID_LEFT_IDLE
	DwarfSpriteAnimationFrontIdle, // DWARF_ANIMATION_ID_FRONT_IDLE
	DwarfSpriteAnimationRightIdle, // DWARF_ANIMATION_ID_RIGHT_IDLE
	DwarfSpriteAnimationLeftWalk,  // DWARF_ANIMATION_ID_LEFT_WALK
	DwarfSpriteAnimationRightWalk, // DWARF_ANIMATION_ID_RIGHT_WALK
	DwarfSpriteAnimationBackClimb, // DWARF_ANIMATION_ID_BACK_CLIMB
	DwarfSpriteAnimationLeftJump,  // DWARF_ANIMATION_ID_LEFT_JUMP
	DwarfSpriteAnimationFrontJump, // DWARF_ANIMATION_ID_FRONT_JUMP
	DwarfSpriteAnimationRightJump  // DWARF_ANIMATION_ID_RIGHT_JUMP
};
