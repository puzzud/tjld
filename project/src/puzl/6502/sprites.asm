; 6502 sprites.asm

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
SpritePositionsXLo:
  .res NUMBER_OF_SPRITES

SpritePositionsXHi:
  .res NUMBER_OF_SPRITES

SpritePositionsYLo:
  .res NUMBER_OF_SPRITES

SpritePositionsYHi:
  .res NUMBER_OF_SPRITES

UpperLeftSpriteTileX:
  .res 1

UpperLeftSpriteTileY:
  .res 1

LowerRightSpriteTileX:
  .res 1

LowerRightSpriteTileY:
  .res 1

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to set position x.
;  - y: x/a, x position (signed).
_SetSpritePositionX:
  pha

  ; spriteIndex, as y register offset.
  ldy #0
  lda (sp),y
  tay

  pla
  sta SpritePositionsXLo,y ; Cache low byte first.
  txa
  sta SpritePositionsXHi,y ; Cache high byte. 
  
  tya
  jsr UpdateSpritePositionX

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to set position y.
;  - y: x/a, y position (signed).
_SetSpritePositionY:
  pha

  ; spriteIndex, as y register offset.
  ldy #0
  lda (sp),y
  tay

  pla
  sta SpritePositionsYLo,y ; Cache low byte first.
  txa
  sta SpritePositionsYHi,y ; Cache high byte. 
  
  tya
  jsr UpdateSpritePositionY

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to get position x.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionX:
  tay
  
  ; x, high byte.
  lda SpritePositionsXHi,y
  tax

  ; a, low byte.
  lda SpritePositionsXLo,y

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to get position y.
; outputs:
;  - return: x/a, sprite position y.
_GetSpritePositionY:
  tay
  
  ; x, high byte.
  lda SpritePositionsYHi,y
  tax

  ; a, low byte.
  lda SpritePositionsYLo,y

  rts

;------------------------------------------------------------------
; Gets collision map cell coordinates for the top left and bottom right
; points of a sprite through TempSpritePositionX/Y.
; Assumes sprite dimensions of 16x16.
;
; inputs:
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
; outputs:
;  - UpperLeftSpriteTileX
;  - UpperLeftSpriteTileY
;  - LowerRightSpriteTileX
;  - LowerRightSpriteTileY
; notes:
;  - Squashes a, mathOperandLo2, mathOperandLo1, mathOperandHi1.
CalculateSpriteTileCorners:
  ; Upper left X.
  lda #0
  sta mathOperandLo2
  lda TempSpritePositionX
  sta mathOperandLo1
  lda TempSpritePositionX+1
  sta mathOperandHi1
  jsr GetTileIndexFromPosition
  sta UpperLeftSpriteTileX

  ; Lower right X.
  lda #(SPRITE_WIDTH-1)
  sta mathOperandLo2
  jsr GetTileIndexFromPosition
  sta LowerRightSpriteTileX

  ; Upper left Y.
  lda #0
  sta mathOperandLo2
  lda TempSpritePositionY
  sta mathOperandLo1
  lda TempSpritePositionY+1
  sta mathOperandHi1
  jsr GetTileIndexFromPosition
  sta UpperLeftSpriteTileY

  ; Lower right Y.
  lda #(SPRITE_HEIGHT-1)
  sta mathOperandLo2
  jsr GetTileIndexFromPosition
  sta LowerRightSpriteTileY

  rts

;------------------------------------------------------------------
; inputs:
;  - offset: mathOperandLo2, offset applied to position.
;  - mathOperandHi1/mathOperandLo1, position to convert.
; outputs:
;  - a, tile offset (X or Y).
GetTileIndexFromPosition:
  jsr AddSignedByteToSignedWord

  sta mathTmp1 ; Low
  stx mathTmp2 ; High

  lsr mathTmp2
  ror mathTmp1
  lsr mathTmp2
  ror mathTmp1
  lsr mathTmp2
  lda mathTmp1
  ror

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from C.
_SetSpriteFrameIndex:
  pha

  ldy #0
  lda (sp),y
  tax

  pla
  jsr SetSpriteFrameIndex

  jmp incsp1
