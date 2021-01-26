; nes video2.asm

.export InitializeVideo

.export _SetBackgroundColor

.export TileMapCollisionOffsetTableLo
.export TileMapCollisionOffsetTableHi
.export _TileMapCollisionCodes

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import InitializeSprites

.importzp _UpdatePaletteFlag
.import _CharacterPalette
.import _SpritePalette

.import _ClearTileMapCollisionCodes

.import InitializeSpritesAnimation

.include "nes.asm"

.segment "BSS"

; TODO: This should probably be moved elsewhere,
; particularly if it is made of 2x2 character blocks.
_TileMapCollisionCodes:
  .res SCREEN_CHAR_SIZE

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
InitializeVideo:

@waitSync1:
  bit PPU_STATUS
@waitSync1Loop:
  bit PPU_STATUS
  bpl @waitSync1Loop

@clearPalette:
  ldx #0
  ldy #$3f
  sty PPU_VRAM_ADDR2
  stx PPU_VRAM_ADDR2
  
  lda #$0f
  ldx #32
@clearPaletteLoop:
  sta PPU_VRAM_IO
  dex
  bne @clearPaletteLoop

@clearVram:
  txa
  ldy #$20
  sty PPU_VRAM_ADDR2
  sta PPU_VRAM_ADDR2
  
  ldy #16
@clearVramLoop:
  sta PPU_VRAM_IO
  inx
  bne @clearVramLoop
  dey
  bne @clearVramLoop

@waitSync2:
  bit PPU_STATUS
@waitSync2Loop:
  bit PPU_STATUS
  bpl @waitSync2Loop

  lda #0
  sta PPU_VRAM_ADDR1
  sta PPU_VRAM_ADDR1

  jsr _ClearTileMapCollisionCodes
  jsr InitializeSprites

  ;lda #%00010000
  ;sta PPU_CTRL1

  ;lda #%00011110
  ;sta PPU_CTRL2

  rts

;------------------------------------------------------------------
_SetBackgroundColor:
  ; First entry of first sprite palette is apparently good enough to
  ; set background color.
  sta _SpritePalette

  lda #1
  sta _UpdatePaletteFlag

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
; Preprocessed table of collision map addresses for each start of a line.
TileMapCollisionOffsetTableLo:
  sloTable _TileMapCollisionCodes, 0

TileMapCollisionOffsetTableHi:
  sloTable _TileMapCollisionCodes, 1
