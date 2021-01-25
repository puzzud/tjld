; nes tilemap.asm

.export _GetTileMapShapeCode
.export _GetTileMapColorCode
.export _SetTileMapCellShape
.export _SetTileMapCellColor

.export _ClearScreen
.export _PrintText
.export _DrawImage

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import ScreenLineOffsetTableLo
.import ScreenLineOffsetTableHi
.import ScreenColorLineOffsetTableLo
.import ScreenColorLineOffsetTableHi

.include "nes.asm"

.include "../6502/tilemap.asm"

.segment "BSS"

ImageWidth:
  .res 1
  
ImageHeight:
  .res 1

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, shape code at this coordinate.
_GetTileMapShapeCode:
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, color code at this coordinate.
_GetTileMapColorCode:
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - shapeCode: a, code indicating what graphic shape to put at this coordinate.
_SetTileMapCellShape:
  jmp incsp2

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - colorCode: a, code indicating what color to put at this coordinate.
_SetTileMapCellColor:
  jmp incsp2

; _PrintText
; Prints text string to an X,Y coordinate on the screen.
;
; inputs:
;  - text: (sp[2],sp[1]), Pointer to text string.
;  - x: sp[0], X position to write string.
;  - y: a, Y position to write string.

;  - text: (x,a), Pointer to text string.
;  - x: PrintX, X position to write string.
;  - y: PrintY, Y position to write string.
_PrintText:
@setTextPointer:
  rts

; _DrawImage
; Prints text string to an X,Y coordinate on the screen.
;
; inputs:
;  - image: (sp[2],sp[1]), Pointer to an image map.
;  - x: sp[0], X position to draw image (uppler left corner).
;  - y: a, Y position to draw image (uppler left corner)
_DrawImage:
  jmp incsp3

;------------------------------------------------------------------
FillScreen:
  rts
  
;------------------------------------------------------------------
_ClearScreen:
  lda #' '
  jsr FillScreen
  
  rts
