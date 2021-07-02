; c64 tilemap.asm

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

.include "c64.asm"

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

; _PrintText
; Prints text string to an X,Y coordinate on the screen.
;
; inputs:
;  - text: (x,a), Pointer to text string.
;  - x: PrintX, X position to write string.
;  - y: PrintY, Y position to write string.
_PrintText:
@setTextPointer:
  ; (ptr1+1,ptr1)=&text
  sta ptr1
  stx ptr1+1

@setTilemapPointer:
  ldx _PrintY ; y

  lda ScreenLineOffsetTableHi,x
  sta ptr2+1
  lda ScreenLineOffsetTableLo,x
  sta ptr2
  
  ; A = ptr2
  clc
  adc _PrintX
  sta ptr2
  bcc @afterIncrementPtr2Hi
  lda #0
  adc ptr2+1
  sta ptr2+1
@afterIncrementPtr2Hi:
  
  ldy #$ff
@printTextLoop:
  iny
  lda (ptr1),y ; (ptr2+1,ptr2)=a=text[++y]
  beq @endPrintTextLoop
  sta (ptr2),y
  bne @printTextLoop
@endPrintTextLoop:

  ; Switch ptr2 to point to color RAM.
  clc
  lda ptr2
  adc #<(SCREEN_COLOR-SCREEN_CHAR)
  sta ptr2
  lda ptr2+1
  adc #>(SCREEN_COLOR-SCREEN_CHAR)
  sta ptr2+1
  
  lda _PrintColor
@printTextColorLoop:
  dey
  sta (ptr2),y
  bne @printTextColorLoop

  rts

; _DrawImage
; Prints text string to an X,Y coordinate on the screen.
;
; inputs:
;  - image: (sp[2],sp[1]), Pointer to an image map.
;  - x: sp[0], X position to draw image (uppler left corner).
;  - y: a, Y position to draw image (uppler left corner)
_DrawImage:
  sta tmp2
  
  clc ; (ptr+1,ptr)=&image
  lda #0
  ldy #1
  adc (sp),y
  sta ptr1
  lda #0
  iny
  adc (sp),y
  sta ptr1+1
  
  ; Retrieve image dimensions.
  ldy #0
  lda (ptr1),y
  sta ImageWidth
  iny
  lda (ptr1),y
  sta ImageHeight
  
  ; Move ptr1 beyond header.
  clc
  lda ptr1
  adc #2
  sta ptr1
  lda #0
  adc ptr1+1
  sta ptr1+1
  
  ; Set up base target VRAM address.
  ldx tmp2 ; y

  lda ScreenLineOffsetTableHi,x
  sta ptr2+1
  lda ScreenLineOffsetTableLo,x
  sta ptr2
  
  ; Add x offset to screen start address.
  clc
  ldy #0 ; x
  adc (sp),y
  sta ptr2
  lda #0
  adc ptr2+1
  sta ptr2+1
  
  ; Calculate color RAM.
  clc
  lda ptr2
  adc #<(SCREEN_COLOR-SCREEN_CHAR)
  sta ptr3
  lda ptr2+1
  adc #>(SCREEN_COLOR-SCREEN_CHAR)
  sta ptr3+1

  ldx #0
@drawImageYLoop:
  
  ldy #$ff
@drawImageXLoop:
  iny
  cpy ImageWidth
  beq @drawImageXLoopEnd
  
  lda (ptr1),y
  sta (ptr2),y
  lda _PrintColor
  sta (ptr3),y
  
  jmp @drawImageXLoop
@drawImageXLoopEnd:
  
  inx
  cpx ImageHeight
  bcs @endDrawImage
  
  ; Move ptr1 to start of next line.
  clc
  lda ptr1
  adc ImageWidth
  sta ptr1
  lda #0
  adc ptr1+1
  sta ptr1+1
  
  ; Move ptr2 and ptr3 to next line.
  clc
  lda ptr2
  adc #SCREEN_CHAR_WIDTH
  sta ptr2
  lda #0
  adc ptr2+1
  sta ptr2+1
  
  clc
  lda ptr3
  adc #SCREEN_CHAR_WIDTH
  sta ptr3
  lda #0
  adc ptr3+1
  sta ptr3+1
  
  jmp @drawImageYLoop

@endDrawImage:
  jmp incsp3

;------------------------------------------------------------------
FillScreen:
  pha

  ; Fill character memory.
  ; Algorithm assumes even pages,
  ; thus -1 to high byte and zeroing of low byte of address.
  lda #>(SCREEN_CHAR+SCREEN_CHAR_SIZE-1)
  sta ptr1+1
  tax ; Set up X to be part of loop as a secondary decrementer.
  ldy #0 ; Also, setting Y's starting value in loop.
  sty ptr1
  
  pla
@fillScreenCharacterLoop:
  dey
  sta (ptr1),y
  bne @fillScreenCharacterLoop
  dex
  cpx #>SCREEN_CHAR-1
  beq @endFillScreenCharacter
  stx ptr1+1
  bne @fillScreenCharacterLoop
@endFillScreenCharacter:

  ; Fill color memory.
  lda #>(SCREEN_COLOR+SCREEN_CHAR_SIZE-1)
  sta ptr1+1
  tax
  
  lda #0
  sta ptr1
  tay
  
  lda _PrintColor
@fillScreenColorLoop:
  dey
  sta (ptr1),y
  bne @fillScreenColorLoop
  dex
  cpx #>SCREEN_COLOR-1
  beq @endFillScreenColor
  stx ptr1+1
  bne @fillScreenColorLoop
@endFillScreenColor:
  
  rts
  
;------------------------------------------------------------------
_ClearScreen:
  lda #' '
  jsr FillScreen
  
  rts
