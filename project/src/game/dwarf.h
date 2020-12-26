#ifndef DWARF_H
#define DWARF_H

#include <puzl.h>

typedef enum
{
	DWARF_FRAME_ID_FRONT_IDLE = 1,
	DWARF_FRAME_ID_FRONT_JUMP = 2,

	DWARF_FRAME_ID_BACK_IDLE = 7,
	DWARF_FRAME_ID_BACK_CLIMB_0 = 8,
	DWARF_FRAME_ID_BACK_CLIMB_1 = 9,
	DWARF_FRAME_ID_BACK_DUCK = 10,

	DWARF_FRAME_ID_RIGHT_IDLE = 14,
	DWARF_FRAME_ID_RIGHT_WALK_0 = 15,
	DWARF_FRAME_ID_RIGHT_WALK_1 = 16,
	DWARF_FRAME_ID_RIGHT_WALK_2 = 17,

	DWARF_FRAME_ID_LEFT_IDLE = 21,
	DWARF_FRAME_ID_LEFT_WALK_0 = 22,
	DWARF_FRAME_ID_LEFT_WALK_1 = 23,
	DWARF_FRAME_ID_LEFT_WALK_2 = 24,

	DWARF_FRAME_ID_RIGHT_JUMP_TROUGH = 28,
	DWARF_FRAME_ID_RIGHT_JUMP_PEAK = 29,

	DWARF_FRAME_ID_LEFT_JUMP_TROUGH = 35,
	DWARF_FRAME_ID_LEFT_JUMP_PEAK = 36
} DwarfSpriteFrameId;

typedef enum
{
	DWARF_ANIMATION_ID_LEFT_IDLE,
	DWARF_ANIMATION_ID_FRONT_IDLE,
	DWARF_ANIMATION_ID_RIGHT_IDLE,
	DWARF_ANIMATION_ID_LEFT_WALK,
	DWARF_ANIMATION_ID_RIGHT_WALK,
	DWARF_ANIMATION_ID_BACK_CLIMB,
	DWARF_ANIMATION_ID_LEFT_JUMP,
	DWARF_ANIMATION_ID_FRONT_JUMP,
	DWARF_ANIMATION_ID_RIGHT_JUMP
} DwarfSpriteAnimationId;

extern const byte DwarfSpriteAnimationLeftIdle[];
extern const byte DwarfSpriteAnimationFrontIdle[];
extern const byte DwarfSpriteAnimationRightIdle[];
extern const byte DwarfSpriteAnimationLeftWalk[];
extern const byte DwarfSpriteAnimationRightWalk[];
extern const byte DwarfSpriteAnimationBackClimb[];
extern const byte DwarfSpriteAnimationLeftJump[];
extern const byte DwarfSpriteAnimationFrontJump[];
extern const byte DwarfSpriteAnimationRightJump[];

extern const byte* DwarfAnimationSet[];

#endif
