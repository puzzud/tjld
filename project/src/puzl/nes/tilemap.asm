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

;.import ScreenLineOffsetTableLo
;.import ScreenLineOffsetTableHi
;.import ScreenColorLineOffsetTableLo
;.import ScreenColorLineOffsetTableHi

.include "nes.asm"

.include "../6502/tilemap.asm"

.segment "BSS"

ImageWidth:
  .res 1
  
ImageHeight:
  .res 1

.segment "ZEROPAGE"

_PrintColorSet:
  .res 1

AttributePointer:
  .res 2

AttributeQuadrantNumber:
  .res 1
  
AttributeTemp:
  .res 1

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, shape code at this coordinate.
_GetTileMapShapeCode:
  ldx #0
  txa
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: sp[0], x coordinate.
;  - y: a, y coordinate.
; outputs:
;  - return: x/a, color code at this coordinate.
_GetTileMapColorCode:
  ldx #0
  txa
  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - x: sp[1], x coordinate.
;  - y: sp[0], y coordinate.
;  - shapeCode: a, code indicating what graphic shape to put at this coordinate.
_SetTileMapCellShape:
  pha

  ; y
  ldy #0
  lda (sp),y
  tax

  ; x
  iny
  lda (sp),y
  sta tmp1

  clc
  lda ScreenLineOffsetTableLo,x
  adc tmp1
  sta ptr2
  lda #0
  adc ScreenLineOffsetTableHi,x
  sta ptr2+1

  lda PPU_STATUS
  lda ptr2+1
  sta PPU_VRAM_ADDR2
  lda ptr2
  sta PPU_VRAM_ADDR2

  pla
  sta PPU_VRAM_IO

  lda #0
  sta PPU_VRAM_ADDR2
  sta PPU_VRAM_ADDR2
  sta PPU_VRAM_ADDR1
  sta PPU_VRAM_ADDR1

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
;  - text: (x,a), Pointer to text string.
;  - x: _PrintX, X position to write string.
;  - y: _PrintY, Y position to write string.
_PrintText:
@setTextPointer:
  ldx _PrintY ; y
  lda ScreenLineOffsetTableHi,x
  sta ptr2+1
  lda ScreenLineOffsetTableLo,x
  sta ptr2
  
  ; A = ptr2
  clc
  adc _PrintX ; x
  sta ptr2
  bcc @endAddXOffsetToCharVram
  inc ptr2+1
@endAddXOffsetToCharVram:
  
  lda PPU_STATUS
  
@setVramAddress:  
  lda ptr2+1
  sta PPU_VRAM_ADDR2
  lda ptr2
  sta PPU_VRAM_ADDR2
  
  clc ; (ptr+1,ptr)=&text
  lda #0
  ldy #1
  adc (sp),y
  sta ptr1
  lda #0
  iny
  adc (sp),y
  sta ptr1+1
  
  ldy #$ff
@printTextLoop:
  iny
  lda (ptr1),y ; PPU_VRAM_IO=a=text[++y]
  beq @endPrintTextLoop
  sta PPU_VRAM_IO
  bne @printTextLoop
@endPrintTextLoop:
  dey
  sty tmp3 ; index for loop.
  
  ldy _PrintX ; x
  ldx _PrintY ; y
@printColorLoop:
  jsr SetCharacterAttribute
  
  iny
  dec tmp3
  bne @printColorLoop
  
  ; A = 0
  sta PPU_VRAM_ADDR2
  sta PPU_VRAM_ADDR2
  sta PPU_VRAM_ADDR1
  sta PPU_VRAM_ADDR1

  jmp incsp2

; SetCharacterAttribute
; Sets the corresponding attribute table half-nibble to _PrintColorSet.
;
; inputs:
;  - x: y, x coordinate.
;  - y: x, y coordinate.
;
; modifies:
;  - AttributePointer: address to attribute byte to modify.
;  - AttributeQuadrantNumber: which of the 4 attribute quadrants to modify.
;  - AttributeTemp: temporary variable for SetCharacterAttribute.
;
; preserves:
;  - a
;  - x
;  - y
SetCharacterAttribute:
  pha
  txa
  pha
  tya
  pha
  
  ; Note that the high value on a standard NES will not change.
  lda #>(PPU_ATTRIBUTE_TABLE_0)
  sta AttributePointer+1
  lda ScreenColorLineOffsetTableLo,x
  sta AttributePointer

  tya ; x /= 4
  lsr
  lsr
  clc
  adc AttributePointer
  sta AttributePointer
@endAddXOffsetToColorVram:
 
@getQuadrantNumber:
  ; Calculate attribute byte quadrant number.
  lda #0
  sta AttributeQuadrantNumber
 
@getAttributeRow:
  inx ; NOTE: First line compensation. Also, X is modified here!
  txa ; y % 4
  and #%11
  cmp #2
  bcc @getAttributeColumn
  inc AttributeQuadrantNumber
  inc AttributeQuadrantNumber
@getAttributeColumn:
  tya ; x % 4
  and #%11
  cmp #2
  bcc @endGetQuadrantNumber
  inc AttributeQuadrantNumber
@endGetQuadrantNumber:

  lda PPU_STATUS

  lda AttributePointer+1
  sta PPU_VRAM_ADDR2
  lda AttributePointer
  sta PPU_VRAM_ADDR2
  
  ; Get existing attribute.
  lda PPU_VRAM_IO
  
  ldy #0
  sty AttributeTemp
  
  ldx AttributeQuadrantNumber
  beq @endShiftDownAttributesLoop
  
  clc
@shiftDownAttributesLoop:
  ror
  ror AttributeTemp
  ror
  ror AttributeTemp
  
  dex
  bne @shiftDownAttributesLoop
@endShiftDownAttributesLoop:
  
  and #%11111100
  ora _PrintColorSet

  ldx AttributeQuadrantNumber
  beq @endShiftUpAttributesLoop
  
  ;clc ; NOTE: Technially, no C clear needed.
@shiftUpAttributesLoop:
  rol AttributeTemp
  rol
  rol AttributeTemp
  rol
  
  dex
  bne @shiftUpAttributesLoop
@endShiftUpAttributesLoop:
  
  ldx PPU_STATUS
  
  ldx AttributePointer+1
  stx PPU_VRAM_ADDR2
  ldx AttributePointer
  stx PPU_VRAM_ADDR2
  
  sta PPU_VRAM_IO
  
  pla
  tay
  pla
  tax
  pla
  
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

;------------------------------------------------------------------
; NOTE: This macro seems to compensate for non-visible top 1 line of name table.
.macro sloTable loOrHi, n
  .local lineNumber
  .if .paramcount > 1
    lineNumber = n
  .else
    lineNumber = 0
  .endif
  
  ; 2 * SCREEN_CHAR_WIDTH to account for hidden row above screen.
  .local address
  address = (PPU_NAME_TABLE_0 + (2*SCREEN_CHAR_WIDTH) + (lineNumber * SCREEN_CHAR_WIDTH))

  .if loOrHi = 0
    .byte <(address)
  .endif
  
  .if loOrHi = 1
    .byte >(address)
  .endif
  
  .if lineNumber < SCREEN_CHAR_HEIGHT - 1
    sloTable loOrHi, (lineNumber+1) ; NOTE: Wrapping parameter in parentheses is critical (bug?).
  .endif
.endmacro

;------------------------------------------------------------------
; NOTE: This macro compensates for non-visible top 1 line of name table.
; TODO: Fix this macro to produce the table below!
.macro scloTable loOrHi, n
  .local lineNumber
  .if .paramcount > 1
    lineNumber = n
  .else
    lineNumber = 0
  .endif
  
  .local attributeLineWidth
  attributeLineWidth = SCREEN_CHAR_WIDTH / 4
  
  .local address
  address = (PPU_ATTRIBUTE_TABLE_0 + (((lineNumber + 1) / 4) * attributeLineWidth))

  .if loOrHi = 0
    .byte <(address)
  .endif
  
  .if loOrHi = 1
    .byte >(address)
  .endif
  
  .if lineNumber < SCREEN_CHAR_HEIGHT - 1
    scloTable loOrHi, (lineNumber+1) ; NOTE: Wrapping parameter in parentheses is critical (bug?).
  .endif
.endmacro


; Preprocessed table of PPU name table addresses for each start of a line.
ScreenLineOffsetTableLo:
  sloTable 0

ScreenLineOffsetTableHi:
  sloTable 1  

; Preprocessed table of PPU attribute table addresses for each start of a line.
ScreenColorLineOffsetTableLo:
  scloTable 0
  
; ScreenColorLineOffsetTableHi:
;   scloTable 1
