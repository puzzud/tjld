; 6502 sprites.asm

.export _CurrentSpriteIndex
.export _GetSpritePositionX
.export _GetSpritePositionY
.export _SetSpritePositionX
.export _SetSpritePositionY
.export _SetSpriteFrameIndex

;.autoimport on
  
;.importzp sp, sreg, regsave, regbank
;.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
;.macpack longbranch

.importzp mathOperandLo1, mathOperandHi1
.importzp mathOperandLo2, mathOperandHi2
.importzp mathTmp1, mathTmp2
.import AddSignedByteToSignedWord

.segment "ZEROPAGE"

; TODO: Should probably clear these values.
_CurrentSpriteIndex:
  .res 1

SpritePositionsXLo:
  .res NUMBER_OF_SPRITES

SpritePositionsXHi:
  .res NUMBER_OF_SPRITES

SpritePositionsYLo:
  .res NUMBER_OF_SPRITES

SpritePositionsYHi:
  .res NUMBER_OF_SPRITES

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - y: x/a, x position (signed).
;  - spriteIndex: _CurrentSpriteIndex, which sprite to set position x.
_SetSpritePositionX:
  ; spriteIndex, as y register offset.
  ldy _CurrentSpriteIndex

  sta SpritePositionsXLo,y ; Cache low byte first.
  txa
  sta SpritePositionsXHi,y ; Cache high byte. 
  
  tya
  jsr UpdateSpritePositionX

  rts

;------------------------------------------------------------------
; inputs:
;  - y: x/a, y position (signed).
;  - spriteIndex: _CurrentSpriteIndex, which sprite to set position y.
_SetSpritePositionY:
  ; spriteIndex, as y register offset.
  ldy _CurrentSpriteIndex

  sta SpritePositionsYLo,y ; Cache low byte first.
  txa
  sta SpritePositionsYHi,y ; Cache high byte. 
  
  tya
  jsr UpdateSpritePositionY

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: _CurrentSpriteIndex, which sprite to get position x.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionX:
  ldy _CurrentSpriteIndex
  
  ; x, high byte.
  lda SpritePositionsXHi,y
  tax

  ; a, low byte.
  lda SpritePositionsXLo,y

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: _CurrentSpriteIndex, which sprite to get position y.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionY:
  ldy _CurrentSpriteIndex
  
  ; x, high byte.
  lda SpritePositionsYHi,y
  tax

  ; a, low byte.
  lda SpritePositionsYLo,y

  rts

;------------------------------------------------------------------
; inputs:
;  - frameIndex: a, index of graphic frame.
;  - spriteIndex: _CurrentSpriteIndex, which sprite to assign frame index.
; notes:
;  - Intended call from C.
_SetSpriteFrameIndex:
  ldx _CurrentSpriteIndex
  jsr SetSpriteFrameIndex

  rts
