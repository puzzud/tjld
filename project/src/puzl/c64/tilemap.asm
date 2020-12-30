; c64 sprites.asm

.export _GetTileMapShapeCode
.export _GetTileMapColorCode
.export _GetTileMapCellCollisionCode
.export GetTileMapCellCollisionCode
.export _SetTileMapCellShape
.export _SetTileMapCellColor
.export _SetTileMapCellCollisionCode

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import ScreenLineOffsetTableLo
.import ScreenLineOffsetTableHi
.import ScreenColorLineOffsetTableLo
.import ScreenColorLineOffsetTableHi
.import TileMapCollisionOffsetTableLo
.import TileMapCollisionOffsetTableHi

.include "c64.asm"

.segment "BSS"

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, shape code at this coordinate.
_GetTileMapShapeCode:
  ; y coordinate.
  tax

  ; Get memory screen row start.
  lda ScreenLineOffsetTableLo,x
  sta ptr1
  lda ScreenLineOffsetTableHi,x
  sta ptr1+1

  ; x coordinate.
  ldy #0
  lda (sp),y
  tay

  ; Set up return values.
  ldx #0 ; Returning a byte requires zeroing x.
  lda (ptr1),y

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, color code at this coordinate.
_GetTileMapColorCode:
  ; y coordinate.
  tax

  ; Get memory screen row start.
  lda ScreenColorLineOffsetTableLo,x
  sta ptr1
  lda ScreenColorLineOffsetTableHi,x
  sta ptr1+1

  ; x coordinate.
  ldy #0
  lda (sp),y
  tay

  ; Set up return values.
  ldx #0 ; Returning a byte requires zeroing x.
  lda (ptr1),y
  and #%00001111 ; Masked because upper nibble can be random.

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, collision code at this coordinate.
; notes:
;  - Squashes a, x, y.
;  - Squashes ptr1, ptr+1 (by proxy).
_GetTileMapCellCollisionCode:
  ; y coordinate.
  tax

  ; x coordinate.
  ldy #0
  lda (sp),y
  tay

  jsr GetTileMapCellCollisionCode

  ; Set up return values.
  ; a is already set from routine.
  ldx #0 ; Returning a byte requires zeroing x.

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: y, x coordinate.
;  - y: x, y coordinate.
; outputs:
;  - return: a, collision code at this coordinate.
;  - flags: n, z.
; notes:
;  - Squashes a, ptr1, ptr+1.
;  - Intended for calls from assembly.
GetTileMapCellCollisionCode:
  ; Get memory collision row start.
  lda TileMapCollisionOffsetTableLo,x
  sta ptr1
  lda TileMapCollisionOffsetTableHi,x
  sta ptr1+1

  lda (ptr1),y

  rts

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - shapeCode: a, code indicating what graphic shape to put at this coordinate.
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
;  - colorCode: a, code indicating what color to put at this coordinate.
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

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - collisionCode: a, code indicating the set of collisions.
_SetTileMapCellCollisionCode:
  sta tmp1

  ; y coordinate.  
  ldy #0
  lda (sp),y
  tax

  ; Get memory screen row start.
  lda TileMapCollisionOffsetTableLo,x
  sta ptr1
  lda TileMapCollisionOffsetTableHi,x
  sta ptr1+1

  ; x coordinate.
  iny
  lda (sp),y
  tay

  lda tmp1
  sta (ptr1),y

  jmp incsp2
