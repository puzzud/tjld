; c64 video.asm

.export _InitializeVideo

.export ScreenLineOffsetTableLo
.export ScreenLineOffsetTableHi
.export ScreenColorLineOffsetTableLo
.export ScreenColorLineOffsetTableHi
.export TileMapCollisionOffsetTableLo
.export TileMapCollisionOffsetTableHi
.export _TileMapCollisionCodes

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _CharacterSet
.import _SpriteSet

.import _ClearTileMapCollisionCodes

.import InitializeSpritesAnimation

.include "c64.asm"

.segment "BSS"
  
; TODO: This should probably be moved elsewhere,
; particularly if it is made of 2x2 character blocks.
_TileMapCollisionCodes:
  .res SCREEN_CHAR_SIZE

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
_InitializeVideo:
  ; Disable video.
  lda SCROLX
  ora #%00100000
  sta SCROLX
  
  lda #COLOR_BLACK
  sta EXTCOL
  sta BGCOL0
  
  sta _PrintColor
  jsr _ClearScreen

  jsr InitializeCharacterGraphics
  jsr _ClearTileMapCollisionCodes
  jsr InitializeSprites

  ; Enable video.
  lda SCROLX
  and #%11011111
  sta SCROLX

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

  jsr InitializeSpritesAnimation

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

;------------------------------------------------------------------
; Preprocessed table of collision map addresses for each start of a line.
TileMapCollisionOffsetTableLo:
  sloTable _TileMapCollisionCodes, 0

TileMapCollisionOffsetTableHi:
  sloTable _TileMapCollisionCodes, 1
