; c64 sprites.asm

.export _EnableSprite
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
;  - enable: A, enable or disable sprite.
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
;  - spriteIndex: sp[0], which sprite to assign frame index.
;  - frameIndex: A, index of graphic frame.
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
;  - colorCode: A, color code.
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
;  - colorCode: A, color code.
_SetSpriteSeconaryColor:
  sta SPMC0

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: A, color code.
_SetSpriteTertiaryColor:
  sta SPMC1

  rts
