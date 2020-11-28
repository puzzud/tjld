; c64 sprites.asm

.export _EnableSprite

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _NthBitFlag

.include "c64.asm"

.segment "BSS"
  
ImageHeight:
  .res 1

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
