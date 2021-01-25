; nes video.asm

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

.import _ClearTileMapCollisionCodes

.import InitializeSpritesAnimation

.include "nes.asm"

; TODO: Figure out what to do with these.
SCREEN_CHAR = $2000
SCREEN_COLOR = $2000

.segment "BSS"
  
; TODO: This should probably be moved elsewhere,
; particularly if it is made of 2x2 character blocks.
_TileMapCollisionCodes:
; TODO: Figure out what should go here.
;  .res (256 * 224 / 8)
;  .res SCREEN_CHAR_SIZE
  .res 1

.segment "ZEROPAGE"

.segment "CODE"

;------------------------------------------------------------------
_InitializeVideo:
  ;jsr _ClearTileMapCollisionCodes
  ;jsr InitializeSprites

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
