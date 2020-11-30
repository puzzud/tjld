; c64 sprites.asm

.export _EnableSprite
.export _GetSpritePositionX
.export _GetSpritePositionY
.export _SetSpritePositionY
.export _SetSpriteFrameIndex
.export _SetSpriteColor
.export _SetSpriteSeconaryColor
.export _SetSpriteTertiaryColor

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _NthBitFlag

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

  ldy #<_NthBitFlag
  sty ptr1
  ldy #>_NthBitFlag
  sty ptr1+1

  ldy #0
  lda (sp),y
  tay

  txa
  bne @enable

@disable:
  lda (ptr1),y
  eor #$ff
  and SPENA

  jmp @set

@enable:
  lda (ptr1),y
  ora SPENA

@set:
  sta SPENA

@done:
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to set position y.
;  - y: x/a, y position (signed).
_SetSpritePositionY:
  tax

  ; spriteIndex * 2, as y register offset.
  ldy #0
  lda (sp),y

  asl
  tay

  ; NOTE: Only low byte is necessary (no x, no 16 bit addition).
  txa
  clc
  adc #SCREEN_BORDER_THICKNESS_Y
  sta SP0Y,y
  
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to get position x.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionX:
  tay

  ; Check 9th bit.
  lda #<_NthBitFlag
  sta ptr2
  lda #>_NthBitFlag
  sta ptr2+1

  lda (ptr2),y
  and MSIGX
  bne @above

@below:
  sta tmp2 ; a is 0.
  beq @readLo

@above:
  lda #$01
  sta tmp2

@readLo:
  tya
  asl
  ; NOTE: No clc needed, assuming spriteIndex is in the range 0-7.
  adc #<SP0X
  sta ptr1
  ; NOTE: No need to transfer carry to high byte,
  ; because there wouldn't be such on this C64 memory address.
  lda #>SP0X
  sta ptr1+1

  ldy #0
  lda (ptr1),y

  sec
  sbc #SCREEN_BORDER_THICKNESS_X
  sta tmp1
  lda tmp2
  sbc #0
  
  ; Set return values.
  tax
  lda tmp1
  
  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to get position y.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionY:
  asl
  ; NOTE: No clc needed, provided spriteIndex is in the range 0-7.
  adc #<SP0Y
  sta ptr1
  ; NOTE: No need to transfer carry to high byte,
  ; because there wouldn't be such on this C64 memory address.
  lda #>SP0Y
  sta ptr1+1

  sec
  ldy #0
  lda (ptr1),y
  sbc #SCREEN_BORDER_THICKNESS_Y
  sta tmp1
  lda #0
  sbc #0
  
  tax ; Set return values.
  lda tmp1
  
  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
_SetSpriteFrameIndex:
  tax

  ldy #0
  lda (sp),y

  clc
  adc #<SPRITE_POINTER_BASE
  sta ptr1
  lda #0
  adc #>SPRITE_POINTER_BASE
  sta ptr1+1

  txa
  clc
  adc #64 ; TODO: Need to investigate why 64 is a base.
  sta (ptr1),y

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to assign color.
;  - colorCode: a, color code.
_SetSpriteColor:
  tax

  ldy #0
  lda (sp),y

  clc
  adc #<SP0COL
  sta ptr1
  ; NOTE: No need to transfer carry to high byte,
  ; because there wouldn't be such on this C64 memory address.
  lda #>SP0COL
  sta ptr1+1

  txa
  sta (ptr1),y

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
