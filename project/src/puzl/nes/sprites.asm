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

SPRITE_OAM_ENTRY_OFFSET_TL = OAM_ENTRY_SIZE*0
SPRITE_OAM_ENTRY_OFFSET_TR = OAM_ENTRY_SIZE*1
SPRITE_OAM_ENTRY_OFFSET_BL = OAM_ENTRY_SIZE*2
SPRITE_OAM_ENTRY_OFFSET_BR = OAM_ENTRY_SIZE*3

.include "../6502/sprites.asm"
.include "../6502/sprites_physics.asm"
.include "../6502/sprites_animation.asm"

.segment "CODE"

InitializeSprites:
  jsr InitializeSpritesAnimation
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
  sta SpriteX+SPRITE_OAM_ENTRY_OFFSET_TL,x
  sta SpriteX+SPRITE_OAM_ENTRY_OFFSET_BL,x
  clc ; NOTE: Technically, clc not required because of asl above assumed on value <= 8.
  adc #8
  sta SpriteX+SPRITE_OAM_ENTRY_OFFSET_TR,x
  sta SpriteX+SPRITE_OAM_ENTRY_OFFSET_BR,x

  rts

;------------------------------------------------------------------
; Update nes sprite registers with the value in SpritePositionsYLo.
;
; inputs:
;  - spriteIndex: a, which sprite to update position y.
; notes:
;  - Squashes a, x, y.
UpdateSpritePositionY:
  tay

  asl
  asl
  asl
  tax

  lda SpritePositionsYLo,y
  
  ; Offset by 8 to account for row above screen (in NTSC).
  ; Offset by another 8 to account for offset made to tilemap offset table to account for row above screen.
  ; Minus 1 because sprites present 1 line lower than they should.
  clc ; NOTE: Technically, clc not required because of asl above assumed on value <= 8.
  adc #((8*2)-1)

  sta SpriteY+SPRITE_OAM_ENTRY_OFFSET_TL,x
  sta SpriteY+SPRITE_OAM_ENTRY_OFFSET_TR,x
  adc #8
  sta SpriteY+SPRITE_OAM_ENTRY_OFFSET_BL,x
  sta SpriteY+SPRITE_OAM_ENTRY_OFFSET_BR,x

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: x, which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from assembly.
SetSpriteFrameIndex:
  pha

  stx tmp1

  txa
  asl
  asl
  asl
  tax

  pla
  asl
  asl
  sta SpriteTileId+SPRITE_OAM_ENTRY_OFFSET_TL,x
  
  clc
  adc #1
  sta SpriteTileId+SPRITE_OAM_ENTRY_OFFSET_TR,x

  adc #1
  sta SpriteTileId+SPRITE_OAM_ENTRY_OFFSET_BL,x

  adc #1
  sta SpriteTileId+SPRITE_OAM_ENTRY_OFFSET_BR,x

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
