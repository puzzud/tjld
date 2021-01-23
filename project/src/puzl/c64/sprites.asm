; c64 sprites.asm

.export _EnableSprite
.export _SetSpriteColor
.export _SetSpriteSeconaryColor
.export _SetSpriteTertiaryColor

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _NthBitFlags
.import _InverseNthBitFlags

.include "c64.asm"

SPRITE_WIDTH = 16
SPRITE_HEIGHT = 16

; TODO: Need to figure how to get this number from puzl/video.h.
NUMBER_OF_SPRITES = NUMBER_OF_HARDWARE_SPRITES

.include "../6502/sprites.asm"
.include "../6502/sprites_physics.asm"
.include "../6502/sprites_animation.asm"

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - enable: a, enable or disable sprite.
_EnableSprite:
  ldy _CurrentSpriteIndex

  cmp #0
  bne @enable

@disable:
  lda _InverseNthBitFlags,y
  and SPENA

  jmp @set

@enable:
  lda _NthBitFlags,y
  ora SPENA

@set:
  sta SPENA

@done:
  rts

;------------------------------------------------------------------
; Update C64 sprite register with the value in SpritePositionsXLo/Hi.
;
; inputs:
;  - spriteIndex: a, which sprite to update position x.
; notes:
;  - Squashes a, x, y.
UpdateSpritePositionX:
  tax ; Cache spriteIndex.
  ; spriteIndex * 2, as y register offset.
  asl
  tay

  lda SpritePositionsXLo,x

  clc
  adc #SCREEN_BORDER_THICKNESS_X
  sta SP0X,y
  lda #0
  adc SpritePositionsXHi,x
  cmp #0
  beq @before256
  bcc @before256

@after255:
  lda _NthBitFlags,x
  ora MSIGX

  ;jmp @done
  bne @done ; NOTE: Assumption is that A has at least one bit set.

@before256:
  lda _InverseNthBitFlags,x
  and MSIGX

@done:
  sta MSIGX
  
  rts

;------------------------------------------------------------------
; Update C64 sprite register with the value in SpritePositionsYLo.
;
; inputs:
;  - spriteIndex: a, which sprite to update position y.
UpdateSpritePositionY:
  tax ; Cache spriteIndex.
  ; spriteIndex * 2, as y register offset.
  asl
  tay

  lda SpritePositionsYLo,x

  ; NOTE: Only low byte is necessary (no x, no 16 bit addition).
  clc
  adc #SCREEN_BORDER_THICKNESS_Y
  sta SP0Y,y
  
  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: x, which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from assembly.
SetSpriteFrameIndex:
  clc
  adc #64 ; TODO: Need to investigate why 64 is a base.
  sta SPRITE_POINTER_BASE,x

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
;  - spriteIndex: _CurrentSpriteIndex, which sprite to assign color.
_SetSpriteColor:
  ldy _CurrentSpriteIndex
  sta SP0COL,y

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
_SetSpriteSeconaryColor:
  sta SPMC0

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
_SetSpriteTertiaryColor:
  sta SPMC1

  rts
