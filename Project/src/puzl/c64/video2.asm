; c64 video.asm

.export _PrintColor

.export _InitializeVideo
.export _ClearScreen
.export _PrintText
.export _DrawImage

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _CharacterSet
.import _SpriteSet

.include "c64.asm"

.segment "BSS"

_PrintColor:
  .res 1
  
ImageWidth:
  .res 1
  
ImageHeight:
  .res 1

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
_InitializeVideo:
  lda #COLOR_BLACK
  sta EXTCOL ; TODO: Temporary border black.
  sta _PrintColor
  
  jsr _ClearScreen

  jsr InitializeCharacterGraphics
  jsr InitializeSprites

  rts

;------------------------------------------------------------------
; Assumes interrupts are disabled.
InitializeCharacterGraphics:
  ;VIC bank
  lda CI2PRA
  ;and #%11111101  ; bank 2  base vic mem = $8000
  and #%11111100  ; bank 3  base vic mem = $c000
  sta CI2PRA
  
  ;set charset
  ;lda #%00111110  ; screen mem = $8000 + $0c00 = $8c00
  ;                ; char mem   = $8000 + $3800 = $b800
  
  lda #%00111100  ; screen mem = $c000 + $0c00 = $cc00
                  ; char mem   = $c000 + $3000 = $f000
  
  ; screen mem = (base VIC memory address) + (1024 * high nibble)
  ; char mem   = (base VIC memory address) + (1024 * low nibble)
  sta VMCSB

  ;save old configuration
  lda R6510
  pha

  ;only RAM
  ;to copy under the IO rom
  lda #%00110000
  sta R6510

  ;take source address from CHARSET
  lda #<_CharacterSet
  sta ptr1
  lda #>_CharacterSet
  sta ptr1+1
  
  ;now copy
  jsr CopyCharacterSet
  
  ;restore ROMs
  pla
  sta R6510
  
  ;enable multi color charset
  lda SCROLX
  ora #%00010000
  sta SCROLX
  
  rts

;------------------------------------------------------------------
; Assumes interrupts are disabled.
InitializeSprites:
  ; Init sprite registers.
  ; No visible sprites.
  lda #$00
  sta SPENA
  
  ; All sprites normal scale.
  sta XXPAND
  sta YXPAND

  ; All sprites multicolor.
  lda #$ff
	sta SPMC
  
  ; Take source address from SPRITES.
  lda #<_SpriteSet
  sta ptr1
  lda #>_SpriteSet
  sta ptr1+1
  
  ; Save old configuration.
  lda R6510
  pha

  ; Only RAM to copy under the IO ROM.
  lda #%00110000
  sta R6510
  
  jsr CopySprites

  ; Restore ROMs.
  pla
  sta R6510

  rts

;------------------------------------------------------------------
CopyCharacterSet:
  ; Set target address ($f000).
  lda #<CHARACTER_GRAPHICS_TARGET
  sta ptr2
  lda #>CHARACTER_GRAPHICS_TARGET
  sta ptr2+1

  ldx #$00
  ldy #$00
  lda #0
  sta tmp1

@nextLine:
  lda (ptr1),Y
  sta (ptr2),Y
  inx
  iny
  cpx #$08
  bne @nextLine
  cpy #$00
  bne @pageBoundaryNotReached
  
  ; The next 256 bytes have been reached, increase high byte.
  inc ptr1+1
  inc ptr2+1

@pageBoundaryNotReached:

  ; Only copy 254 chars to keep IRQ vectors intact.
  inc tmp1
  lda tmp1
  cmp #254
  beq @copyCharsetDone
  ldx #$00
  jmp @nextLine

@copyCharsetDone:
  rts

;------------------------------------------------------------------
CopySprites:
  ldy #$00
  ldx #$00

  lda #<SPRITE_GRAPHICS_TARGET
  sta ptr2
  lda #>SPRITE_GRAPHICS_TARGET
  sta ptr2+1
    
  ; 4 sprites per loop.
@spriteLoop:
  lda (ptr1),y
  sta (ptr2),y
  iny
  bne @spriteLoop
  inx
  inc ptr1+1
  inc ptr2+1
  cpx #NUMBER_OF_SPRITES_DIV_4
  bne @spriteLoop

  rts

; _PrintText
; Prints text string to an X,Y coordinate on the screen.
;
; inputs:
;  - text: (sp[2],sp[1]), Pointer to text string.
;  - x: sp[0], X position to write string.
;  - y: a, Y position to write string.
_PrintText:
  tax ; y
  
  lda ScreenLineOffsetTableHi,x
  sta ptr2+1
  lda ScreenLineOffsetTableLo,x
  sta ptr2
  
  ; A = ptr2
  clc
  ldy #0 ; x
  adc (sp),y
  sta ptr2
  bcc @setTextPointer
  lda #0
  adc ptr2+1
  sta ptr2+1

@setTextPointer:
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

  jmp incsp3

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
  
;------------------------------------------------------------------
.macro sloTable baseAddress, loOrHi, n
  .local lineNumber
  .if .paramcount > 2
    lineNumber = n
  .else
    lineNumber = 0
  .endif
  
  .local address
  address = (baseAddress + (SCREEN_CHAR_WIDTH * lineNumber))

  .if loOrHi = 0
    .byte <(address)
  .endif
  
  .if loOrHi = 1
    .byte >(address)
  .endif
  
  .if lineNumber < SCREEN_CHAR_HEIGHT
    sloTable baseAddress, loOrHi, (lineNumber+1) ; NOTE: Wrapping parameter in parentheses is critical (bug?).
  .endif
.endmacro

;------------------------------------------------------------------
; Preprocessed table of video RAM addresses for each start of a line.
ScreenLineOffsetTableLo:
  sloTable SCREEN_CHAR, 0

ScreenLineOffsetTableHi:
  sloTable SCREEN_CHAR, 1

;------------------------------------------------------------------
; Preprocessed table of video color RAM addresses for each start of a line.
ScreenColorLineOffsetTableLo:
  sloTable SCREEN_COLOR, 0

ScreenColorLineOffsetTableHi:
  sloTable SCREEN_COLOR, 1
