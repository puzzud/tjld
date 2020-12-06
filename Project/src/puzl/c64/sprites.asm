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

.import _NthBitFlags
.import _InverseNthBitFlags
.import _SpritePositionsX
.import _SpritePositionsY
.import _SpriteVelocitiesX
.import _SpriteVelocitiesY

.include "c64.asm"

.segment "BSS"

.segment "ZEROPAGE"

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
  tay
  jsr UpdateSpritePositionX

  jmp incsp1

;------------------------------------------------------------------
; Update C64 sprite register with the value in _SpritePositionsX.
;
; inputs:
;  - spriteIndex: y, which sprite to update position x.
UpdateSpritePositionX:
  ; spriteIndex * 2, as y register offset.
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
  lda _NthBitFlags,y
  ora MSIGX

  jmp @done

@before256:
  lda _InverseNthBitFlags,y
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
  tay
  jsr UpdateSpritePositionY

  jmp incsp1

;------------------------------------------------------------------
; Update C64 sprite register with the value in _SpritePositionsY.
;
; inputs:
;  - spriteIndex: y, which sprite to update position y.
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

  ; spriteIndex * 2, as y register offset.
  iny
  lda (sp),y

  asl
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
  sta tmp1 ; Cache spriteIndex.

  asl
  tay
  sty tmp2 ; Cache spriteIndex * 2 word offset.

@setX:
  lda #0
  sta tmp3 ; Initial velocity high byte.
  clc
  lda _SpriteVelocitiesX,y
  beq @afterSetX
  bpl @afterNegativeX
  dec tmp3 ; Make velocity high byte negative.
@afterNegativeX:
  adc _SpritePositionsX,y
  sta _SpritePositionsX,y
  lda tmp3
  adc _SpritePositionsX+1,y
  sta _SpritePositionsX+1,y

@afterSetHiX:
  lda tmp1
  jsr UpdateSpritePositionX

  ldy tmp2
@afterSetX: ; NOTE: Saves on the previous 1 statement.

@setY:
  lda #0 ; NOTE: It appears as though this might not be necessary for Y, as it only resolves to 1 byte?
  sta tmp3 ; Initial velocity high byte.
  clc
  lda _SpriteVelocitiesY,y
  beq @afterSetY
  bpl @afterNegativeY
  dec tmp3 ; Make velocity high byte negative.
@afterNegativeY:
  adc _SpritePositionsY,y
  sta _SpritePositionsY,y
  lda tmp3
  adc _SpritePositionsY+1,y
  sta _SpritePositionsY+1,y

@afterSetHiY:
  lda tmp1
  jsr UpdateSpritePositionY

@afterSetY:
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
