; c64 sprites.asm

.export InitializeSpritesAnimation
.export _EnableSprite
.export _GetSpritePositionX
.export _GetSpritePositionY
.export _SetSpritePositionX
.export _SetSpritePositionY
.export _SetSpriteFrameIndex
.export _SetSpriteColor
.export _SetSpriteSeconaryColor
.export _SetSpriteTertiaryColor

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.importzp mathOperandLo1, mathOperandHi1
.importzp mathOperandLo2, mathOperandHi2
.importzp mathTmp1, mathTmp2
.import AddSignedByteToSignedWord

.import _NthBitFlags
.import _InverseNthBitFlags
.import _SpritePositionsX
.import _SpritePositionsY

.include "c64.asm"

SPRITE_WIDTH = 16
SPRITE_HEIGHT = 16

; TODO: Need to figure how to get this number from puzl/video.h.
NUMBER_OF_SPRITES = NUMBER_OF_HARDWARE_SPRITES

.include "../6502/sprites_physics.asm"
.include "../6502/sprites_animation.asm"

.segment "ZEROPAGE"

; TODO: Should probably clear these values.
SpritePositionsXLo:
  .res NUMBER_OF_SPRITES

SpritePositionsXHi:
  .res NUMBER_OF_SPRITES

SpritePositionsYLo:
  .res NUMBER_OF_SPRITES

SpritePositionsYHi:
  .res NUMBER_OF_SPRITES

UpperLeftSpriteTileX:
  .res 1

UpperLeftSpriteTileY:
  .res 1

LowerRightSpriteTileX:
  .res 1

LowerRightSpriteTileY:
  .res 1

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to enable / disable.
;  - enable: a, enable or disable sprite.
_EnableSprite:
  tax

  ldy #0
  lda (sp),y
  tay

  txa
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
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to set position x.
;  - y: x/a, x position (signed).
_SetSpritePositionX:
  pha

  ; spriteIndex, as y register offset.
  ldy #0
  lda (sp),y
  tay

  pla
  sta SpritePositionsXLo,y ; Cache low byte first.
  txa
  sta SpritePositionsXHi,y ; Cache high byte. 
  
  tya
  jsr UpdateSpritePositionX

  jmp incsp1

;------------------------------------------------------------------
; Update C64 sprite register with the value in _SpritePositionsX.
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
; inputs:
;  - spriteIndex: sp[0], which sprite to set position y.
;  - y: x/a, y position (signed).
_SetSpritePositionY:
  pha

  ; spriteIndex, as y register offset.
  ldy #0
  lda (sp),y
  tay

  pla
  sta SpritePositionsYLo,y ; Cache low byte first.
  txa
  sta SpritePositionsYHi,y ; Cache high byte. 
  
  tya
  jsr UpdateSpritePositionY

  jmp incsp1

;------------------------------------------------------------------
; Update C64 sprite register with the value in _SpritePositionsY.
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
;  - spriteIndex: a, which sprite to get position x.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionX:
  tay
  
  ; x, high byte.
  lda SpritePositionsXHi,y
  tax

  ; a, low byte.
  lda SpritePositionsXLo,y

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to get position y.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionY:
  tay
  
  ; x, high byte.
  lda SpritePositionsYHi,y
  tax

  ; a, low byte.
  lda SpritePositionsYLo,y

  rts

;------------------------------------------------------------------
; Gets collision map cell coordinates for the top left and bottom right
; points of a sprite through TempSpritePositionX/Y.
; Assumes sprite dimensions of 16x16.
;
; inputs:
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
; outputs:
;  - UpperLeftSpriteTileX
;  - UpperLeftSpriteTileY
;  - LowerRightSpriteTileX
;  - LowerRightSpriteTileY
; notes:
;  - Squashes a, mathOperandLo2, mathOperandLo1, mathOperandHi1.
CalculateSpriteTileCorners:
  ; Upper left X.
  lda #0
  sta mathOperandLo2
  lda TempSpritePositionX
  sta mathOperandLo1
  lda TempSpritePositionX+1
  sta mathOperandHi1
  jsr GetTileIndexFromPosition
  sta UpperLeftSpriteTileX

  ; Lower right X.
  lda #(SPRITE_WIDTH-1)
  sta mathOperandLo2
  jsr GetTileIndexFromPosition
  sta LowerRightSpriteTileX

  ; Upper left Y.
  lda #0
  sta mathOperandLo2
  lda TempSpritePositionY
  sta mathOperandLo1
  lda TempSpritePositionY+1
  sta mathOperandHi1
  jsr GetTileIndexFromPosition
  sta UpperLeftSpriteTileY

  ; Lower right Y.
  lda #(SPRITE_HEIGHT-1)
  sta mathOperandLo2
  jsr GetTileIndexFromPosition
  sta LowerRightSpriteTileY

  rts

;------------------------------------------------------------------
; inputs:
;  - offset: mathOperandLo2, offset applied to position.
;  - mathOperandHi1/mathOperandLo1, position to convert.
; outputs:
;  - a, tile offset (X or Y).
GetTileIndexFromPosition:
  jsr AddSignedByteToSignedWord

  sta mathTmp1 ; Low
  stx mathTmp2 ; High

  lsr mathTmp2
  ror mathTmp1
  lsr mathTmp2
  ror mathTmp1
  lsr mathTmp2
  lda mathTmp1
  ror

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from C.
_SetSpriteFrameIndex:
  pha

  ldy #0
  lda (sp),y
  tax

  pla
  jsr SetSpriteFrameIndex

  jmp incsp1

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
;  - spriteIndex: sp[0], which sprite to assign color.
;  - colorCode: a, color code.
_SetSpriteColor:
  tax

  ldy #0
  lda (sp),y
  tay

  txa
  sta SP0COL,y

  jmp incsp1

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
