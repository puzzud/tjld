; nes sprites.asm

.export InitializeSprites
.export _EnableSprite
.export _SetSpriteColor
.export _SetSpriteSeconaryColor
.export _SetSpriteTertiaryColor

.importzp _UpdatePaletteFlag
.import _SpritePalette
.import _CharacterPalette

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.import _NthBitFlags
.import _InverseNthBitFlags

.include "nes.asm"

SPRITE_WIDTH = 16
SPRITE_HEIGHT = 16

; TODO: Need to figure how to get this number from puzl/video.h.
NUMBER_OF_SPRITES = 8

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
  rts

;------------------------------------------------------------------
; Update C64 sprite register with the value in SpritePositionsXLo/Hi.
;
; inputs:
;  - spriteIndex: a, which sprite to update position x.
; notes:
;  - Squashes a, x, y.
UpdateSpritePositionX:
  rts

;------------------------------------------------------------------
; Update C64 sprite register with the value in SpritePositionsYLo.
;
; inputs:
;  - spriteIndex: a, which sprite to update position y.
UpdateSpritePositionY:
  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: x, which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from assembly.
SetSpriteFrameIndex:
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
