; nes sprites.asm

.export InitializeSprites
.export _EnableSprite
.export _SetSpriteColor
.export _SetSpriteSeconaryColor
.export _SetSpriteTertiaryColor

.importzp _UpdatePaletteFlag
.import _SpritePalette

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _NthBitFlags
.import _InverseNthBitFlags

.import __SPRAM_START__

.include "nes.asm"

SPRITE_WIDTH = 16
SPRITE_HEIGHT = 16

; TODO: Need to figure how to get this number from puzl/video.h.
NUMBER_OF_SPRITES = 8

SpriteY          = __SPRAM_START__+0
SpriteTileId     = __SPRAM_START__+1
SpriteAttributes = __SPRAM_START__+2
SpriteX          = __SPRAM_START__+3

OAM_ENTRY_SIZE = 4

.include "../6502/sprites.asm"
.include "../6502/sprites_physics.asm"
.include "../6502/sprites_animation.asm"

.segment "CODE"

InitializeSprites:
  rts

;------------------------------------------------------------------
; inputs:
;  - enable: a, enable or disable sprite.
_EnableSprite:
  ; TODO: Implement some scheme where an enabled sprite
  ; has its UpdateSpritePositionY routine is affected
  ; to where it is shown on screen.
  rts

;------------------------------------------------------------------
; Update nes sprite registers with the value in SpritePositionsXLo/Hi.
;
; inputs:
;  - spriteIndex: a, which sprite to update position x.
; notes:
;  - Squashes a, x, y.
UpdateSpritePositionX:
  tay

  asl
  asl
  asl
  tax

  lda SpritePositionsXLo,y
  sta SpriteX,x
  sta SpriteX+(OAM_ENTRY_SIZE*NUMBER_OF_SPRITES)+OAM_ENTRY_SIZE,x
  adc #8
  sta SpriteX+OAM_ENTRY_SIZE,x
  sta SpriteX+(OAM_ENTRY_SIZE*NUMBER_OF_SPRITES),x

  rts

;------------------------------------------------------------------
; Update nes sprite registers with the value in SpritePositionsYLo.
;
; inputs:
;  - spriteIndex: a, which sprite to update position y.
UpdateSpritePositionY:
  tay

  asl
  asl
  asl
  tax

  lda SpritePositionsYLo,y
  
  ; Offset by 8 to account for row above screen (in NTSC).
  ; Minus 1 because sprites present 1 line lower than they should.
  clc
  adc #(8-1)

  sta SpriteY,x
  sta SpriteY+OAM_ENTRY_SIZE,x
  adc #8
  sta SpriteY+(OAM_ENTRY_SIZE*NUMBER_OF_SPRITES),x
  sta SpriteY+(OAM_ENTRY_SIZE*NUMBER_OF_SPRITES)+OAM_ENTRY_SIZE,x

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: x, which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from assembly.
SetSpriteFrameIndex:
  stx tmp1

  asl
  asl
  asl
  tax

  lda #219 ; Tile ID
  sta SpriteTileId,x
  sta SpriteTileId+OAM_ENTRY_SIZE,x
  sta SpriteTileId+(OAM_ENTRY_SIZE*NUMBER_OF_SPRITES),x
  sta SpriteTileId+(OAM_ENTRY_SIZE*NUMBER_OF_SPRITES)+OAM_ENTRY_SIZE,x

  ldx tmp1

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
;  - spriteIndex: _CurrentSpriteIndex, which sprite to assign color.
_SetSpriteColor:
  ldx #2
  
  sta _SpritePalette+(4*0),x
  sta _SpritePalette+(4*1),x
  sta _SpritePalette+(4*2),x
  sta _SpritePalette+(4*3),x

  stx _UpdatePaletteFlag ; NOTE: Just needs to be nonzero.

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
_SetSpriteSeconaryColor:
  ldx #1
  
  sta _SpritePalette+(4*0),x
  sta _SpritePalette+(4*1),x
  sta _SpritePalette+(4*2),x
  sta _SpritePalette+(4*3),x

  stx _UpdatePaletteFlag

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
_SetSpriteTertiaryColor:
  ldx #3
  
  sta _SpritePalette+(4*0),x
  sta _SpritePalette+(4*1),x
  sta _SpritePalette+(4*2),x
  sta _SpritePalette+(4*3),x

  stx _UpdatePaletteFlag ; NOTE: Just needs to be nonzero.
  
  rts
