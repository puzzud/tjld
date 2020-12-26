; c64 dwarf.asm

.export _DwarfAnimationSet

.import _DwarfSpriteAnimationLeftIdle
.import _DwarfSpriteAnimationFrontIdle
.import _DwarfSpriteAnimationRightIdle
.import _DwarfSpriteAnimationLeftWalk
.import _DwarfSpriteAnimationRightWalk
.import _DwarfSpriteAnimationBackClimb
.import _DwarfSpriteAnimationLeftJump
.import _DwarfSpriteAnimationFrontJump
.import _DwarfSpriteAnimationRightJump

; NOTE: cc65 seems to have a bug where it's not populating
; this array of pointers correctly.
; Therefore, this array needs to be defined in cc65 / 6502 implementation.
_DwarfAnimationSet:
	.word _DwarfSpriteAnimationLeftIdle  ; DWARF_ANIMATION_ID_LEFT_IDLE
	.word _DwarfSpriteAnimationFrontIdle ; DWARF_ANIMATION_ID_FRONT_IDLE
	.word _DwarfSpriteAnimationRightIdle ; DWARF_ANIMATION_ID_RIGHT_IDLE
	.word _DwarfSpriteAnimationLeftWalk  ; DWARF_ANIMATION_ID_LEFT_WALK
	.word _DwarfSpriteAnimationRightWalk ; DWARF_ANIMATION_ID_RIGHT_WALK
	.word _DwarfSpriteAnimationBackClimb ; DWARF_ANIMATION_ID_BACK_CLIMB
	.word _DwarfSpriteAnimationLeftJump  ; DWARF_ANIMATION_ID_LEFT_JUMP
	.word _DwarfSpriteAnimationFrontJump ; DWARF_ANIMATION_ID_FRONT_JUMP
	.word _DwarfSpriteAnimationRightJump ; DWARF_ANIMATION_ID_RIGHT_JUMP
