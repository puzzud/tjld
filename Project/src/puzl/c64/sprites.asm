; c64 sprites.asm

.export _EnableSprite
.export _GetSpritePositionX
.export _GetSpritePositionY
.export _SetSpritePositionX
.export _SetSpritePositionY
.export _SetSpriteVelocity
.export _MoveSprite
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
.import AddSignedByteToSignedWord

.import _NthBitFlags
.import _InverseNthBitFlags
.import _SpritePositionsX
.import _SpritePositionsY
.import _SpriteVelocitiesX
.import _SpriteVelocitiesY

.include "c64.asm"

.segment "BSS"

.segment "ZEROPAGE"

TempSpritePositionX:
  .res 2

TempSpritePositionY:
  .res 2

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

  ; spriteIndex * 2, as y register offset.
  ldy #0
  lda (sp),y

  asl
  tay

  pla
  sta _SpritePositionsX,y ; Cache low byte first.
  txa
  sta _SpritePositionsX+1,y ; Cache high byte. 
  
  tya
  lsr
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
  ; spriteIndex * 2, as y register offset.
  tax ; Cache spriteIndex.
  asl
  tay

  lda _SpritePositionsX,y

  clc
  adc #SCREEN_BORDER_THICKNESS_X
  sta SP0X,y
  lda #0
  adc _SpritePositionsX+1,y
  cmp #0
  beq @before256
  bcc @before256

@after255:
  lda _NthBitFlags,x
  ora MSIGX

  jmp @done

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

  ; spriteIndex * 2, as y register offset.
  ldy #0
  lda (sp),y
  asl
  tay

  pla
  sta _SpritePositionsY,y ; Cache low byte first.
  txa
  sta _SpritePositionsY+1,y ; Cache high byte. 
  
  tya
  lsr
  jsr UpdateSpritePositionY

  jmp incsp1

;------------------------------------------------------------------
; Update C64 sprite register with the value in _SpritePositionsY.
;
; inputs:
;  - spriteIndex: a, which sprite to update position y.
UpdateSpritePositionY:
  ; spriteIndex * 2, as y register offset.
  asl
  tay

  lda _SpritePositionsY,y

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
  asl
  tay
  
  ; x, high byte.
  lda _SpritePositionsX+1,y
  tax

  ; a, low byte.
  lda _SpritePositionsX,y

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to get position y.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionY:
  asl
  tay
  
  ; x, high byte.
  lda _SpritePositionsY+1,y
  tax

  ; a, low byte.
  lda _SpritePositionsY,y

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[1], which sprite to set velocity.
;  - x: sp[0], x velocity (signed).
;  - y: a, y velocity (signed).
_SetSpriteVelocity:
  ; Cache a register on CPU stack.
  pha

  ; Cache x velocity in x register.
  ldy #0
  lda (sp),y
  tax

  ; spriteIndex, as y register offset.
  iny
  lda (sp),y
  tay

  txa
  sta _SpriteVelocitiesX,y

  pla
  sta _SpriteVelocitiesY,y

  jmp incsp2

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to move.
_MoveSprite:
  tax ; Cache spriteIndex.
  stx tmp1

  asl
  tay
  sta tmp2 ; Cache spriteIndex word offset.

@setX:
  lda _SpriteVelocitiesX,x
  ;beq @afterSetX

  sta mathOperandLo2
  lda _SpritePositionsX,y
  sta mathOperandLo1
  lda _SpritePositionsX+1,y
  sta mathOperandHi1
  jsr AddSignedByteToSignedWord

  sta TempSpritePositionX
  stx TempSpritePositionX+1

@afterSetX:
@setY:
  ldx tmp1
  lda _SpriteVelocitiesY,x
  ;beq @afterSetY

  sta mathOperandLo2
  lda _SpritePositionsY,y
  sta mathOperandLo1
  lda _SpritePositionsY+1,y
  sta mathOperandHi1
  jsr AddSignedByteToSignedWord

  sta TempSpritePositionY
  stx TempSpritePositionY+1

@afterSetY:
  
  ; TODO: Make collision map optional.
  lda #0
  beq @afterCollisionChecking

  jsr CheckSpriteCollision
@afterCollisionChecking:

@updateSpritePosition:
  ldy tmp2

  lda TempSpritePositionX
  sta _SpritePositionsX,y
  lda TempSpritePositionX+1
  sta _SpritePositionsX+1,y

  lda TempSpritePositionY
  sta _SpritePositionsY,y
  lda TempSpritePositionY+1
  sta _SpritePositionsY+1,y

  lda tmp1
  pha
  jsr UpdateSpritePositionX
  pla
  jsr UpdateSpritePositionY

  rts

;------------------------------------------------------------------
; Checks for collision map overlap with temporary sprite position.
; Adjusts this position to original X or Y position, depending on velocity.
; Assumes sprite dimensions of 16x16.
;
; inputs:
;  - spriteIndex: a, tmp1, which sprite to move.
;  - y, word offset of spriteIndex.
; outputs:
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
CheckSpriteCollision:
  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
_SetSpriteFrameIndex:
  tax

  ldy #0
  lda (sp),y
  tay

  txa
  clc
  adc #64 ; TODO: Need to investigate why 64 is a base.
  sta SPRITE_POINTER_BASE,y

  jmp incsp1

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
