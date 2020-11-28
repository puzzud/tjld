; c64 sprites.asm

;.export _GetTileMapShapeCode
;.export _GetTileMapColorCode
.export _SetTileMapCellShape
.export _SetTileMapCellColor

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import ScreenLineOffsetTableLo
.import ScreenLineOffsetTableHi
.import ScreenColorLineOffsetTableLo
.import ScreenColorLineOffsetTableHi

.include "c64.asm"

.segment "BSS"

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: A, y coordinate.
; returns: shape code at this coordinate.
;  - 
;_GetTileMapShapeCode:

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: A, y coordinate.
; returns: color code at this coordinate.
;  - 
; _GetTileMapColorCode:

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - shapeCode: A, code indicating what graphic shape to put at this coordinate.
_SetTileMapCellShape:
  sta tmp1

  ; y coordinate.  
  ldy #0
  lda (sp),y
  tax

  ; Get memory screen row start.
  lda ScreenLineOffsetTableLo,x
  sta ptr1
  lda ScreenLineOffsetTableHi,x
  sta ptr1+1

  ; x coordinate.
  iny
  lda (sp),y
  tay

  lda tmp1
  sta (ptr1),y

  jmp incsp2

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - colorCode: A, code indicating what color to put at this coordinate.
_SetTileMapCellColor:
  sta tmp1

  ; y coordinate.  
  ldy #0
  lda (sp),y
  tax

  ; Get memory screen row start.
  lda ScreenColorLineOffsetTableLo,x
  sta ptr1
  lda ScreenColorLineOffsetTableHi,x
  sta ptr1+1

  ; x coordinate.
  iny
  lda (sp),y
  tay

  lda tmp1
  sta (ptr1),y

  jmp incsp2
