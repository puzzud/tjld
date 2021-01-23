; 6502 sprites_physics.asm

.export _MoveSprite

.export _SpriteVelocitiesX
.export _SpriteVelocitiesY

.export _SpriteCollisionMasks
.export _SpriteCollisions

;.autoimport on
  
;.importzp sp, sreg, regsave, regbank
;.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
;.macpack longbranch

.import GetTileMapCellCollisionCode

COLLISION_FLAG_OBSTACLE = $80

.segment "ZEROPAGE"

_SpriteVelocitiesX:
  .res NUMBER_OF_SPRITES

_SpriteVelocitiesY:
  .res NUMBER_OF_SPRITES

TempSpritePositionX:
  .res 2

TempSpritePositionY:
  .res 2

UpperLeftSpriteTileX:
  .res 1

UpperLeftSpriteTileY:
  .res 1

LowerRightSpriteTileX:
  .res 1

LowerRightSpriteTileY:
  .res 1

_SpriteCollisionMasks:
  .res NUMBER_OF_SPRITES

_SpriteCollisions:
  .res NUMBER_OF_SPRITES

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: _CurrentSpriteIndex, which sprite to move.
_MoveSprite:
  ; spriteIndex.
  ldx _CurrentSpriteIndex

@setX:
  lda _SpriteVelocitiesX,x
  ;beq @afterSetX

  sta mathOperandLo2
  lda SpritePositionsXLo,x
  sta mathOperandLo1
  lda SpritePositionsXHi,x
  sta mathOperandHi1
  jsr AddSignedByteToSignedWord

  sta TempSpritePositionX
  stx TempSpritePositionX+1

@afterSetX:
@setY:
  ldx _CurrentSpriteIndex
  lda _SpriteVelocitiesY,x
  ;beq @afterSetY

  sta mathOperandLo2
  lda SpritePositionsYLo,x
  sta mathOperandLo1
  lda SpritePositionsYHi,x
  sta mathOperandHi1
  jsr AddSignedByteToSignedWord

  sta TempSpritePositionY
  stx TempSpritePositionY+1

@afterSetY:

  ; Check if should do collision map checking.
  ldx _CurrentSpriteIndex
  lda _SpriteCollisionMasks,x
  beq @afterCollisionChecking

  jsr CheckSpriteCollision
@afterCollisionChecking:

@updateSpritePosition:
  ldx _CurrentSpriteIndex
  lda TempSpritePositionX
  sta SpritePositionsXLo,x
  lda TempSpritePositionX+1
  sta SpritePositionsXHi,x

  lda TempSpritePositionY
  sta SpritePositionsYLo,x
  lda TempSpritePositionY+1
  sta SpritePositionsYHi,x

  lda _CurrentSpriteIndex
  pha
  jsr UpdateSpritePositionX
  pla
  jsr UpdateSpritePositionY

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
; Checks for collision map overlap with temporary sprite position.
; Adjusts this position to original X or Y position, depending on velocity.
; Assumes sprite dimensions of 16x16.
;
; inputs:
;  - spriteIndex: _CurrentSpriteIndex, which sprite to move.
;  - spriteCollisionMask: a, which layers this sprite collides with.
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
; outputs:
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
; notes:
;  - Squashes a, x, y, tmp2, tmp3.
;  - Squashes ptr1, ptr1+1 (by proxy).
CheckSpriteCollision:
  sta tmp2 ; Cache spriteCollisionMask.

  lda #0
  sta tmp3 ; Temporary cache for setting _SpriteCollisions,x.

  lda tmp2
  bmi @calculateSpriteTileCorners; spriteCollisionMask & COLLISION_FLAG_OBSTACLE
  jmp @otherCollisionCheckMaskLoaded

@calculateSpriteTileCorners:
  jsr CalculateSpriteTileCorners

@obstacleCollisionCheck:

@checkY:
  ldx _CurrentSpriteIndex
  lda _SpriteVelocitiesY,x
  beq @afterCheckY
  bpl @yVelocityPositive

@yVelocityNegative:
  ldy UpperLeftSpriteTileX
  ldx UpperLeftSpriteTileY
  jsr GetTileMapCellCollisionCode
  bmi @resetPositionY
  ldy LowerRightSpriteTileX
  jsr GetTileMapCellCollisionCode
  bpl @afterCheckY

@resetPositionY:
  ;lda #COLLISION_FLAG_OBSTACLE
  sta tmp3 ; NOTE: Assumed first set, OR not needed.

  ldx _CurrentSpriteIndex
  lda SpritePositionsYLo,x
  sta TempSpritePositionY
  lda SpritePositionsYHi,x
  sta TempSpritePositionY+1

  jsr CalculateSpriteTileCorners

  jmp @afterCheckY

@yVelocityPositive:
  ldy UpperLeftSpriteTileX
  ldx LowerRightSpriteTileY
  jsr GetTileMapCellCollisionCode
  bmi @resetPositionY
  ldy LowerRightSpriteTileX
  jsr GetTileMapCellCollisionCode
  bmi @resetPositionY ; NOTE: Not bpl.
@afterCheckY:

@checkX:
  ldx _CurrentSpriteIndex
  lda _SpriteVelocitiesX,x
  beq @afterCheckX
  bpl @xVelocityPositive

@xVelocityNegative:
  ldy UpperLeftSpriteTileX
  ldx UpperLeftSpriteTileY
  jsr GetTileMapCellCollisionCode
  bmi @resetPositionX
  ldx LowerRightSpriteTileY
  jsr GetTileMapCellCollisionCode
  bpl @afterCheckX

@resetPositionX:
  ;lda #COLLISION_FLAG_OBSTACLE
  sta tmp3 ; NOTE: Assumed first set, OR not needed.

  ldx _CurrentSpriteIndex
  lda SpritePositionsXLo,x
  sta TempSpritePositionX
  lda SpritePositionsXHi,x
  sta TempSpritePositionX+1

  jsr CalculateSpriteTileCorners

  jmp @afterCheckX

@xVelocityPositive:
  ldy LowerRightSpriteTileX
  ldx UpperLeftSpriteTileY
  jsr GetTileMapCellCollisionCode
  bmi @resetPositionX
  ldx LowerRightSpriteTileY
  jsr GetTileMapCellCollisionCode
  bmi @resetPositionX ; NOTE: Not bpl.
@afterCheckX:

@afterObstacleCollisionCheck:

@otherCollisionCheck:
  lda tmp2
@otherCollisionCheckMaskLoaded:
  and #$7f
  beq @afterOtherCollisionCheck; spriteCollisionMask & ~COLLISION_FLAG_OBSTACLE

  ; Cycle through all overlapped tiles and imprint their collision flags on
	; this sprite's collisions.

  ldx UpperLeftSpriteTileY
@yLoop:

  ldy UpperLeftSpriteTileX
@xLoop:
  tya
  pha
  txa
  pha

  jsr GetTileMapCellCollisionCode
  ora tmp3 ; Update _SpriteCollisions.
  sta tmp3

  pla
  tax
  pla
  tay

  iny
  cpy LowerRightSpriteTileX
  bcc @xLoop
  beq @xLoop ; TODO: This check shouldn't be necessary!

  inx
  cpx LowerRightSpriteTileY
  bcc @yLoop
  beq @yLoop ; TODO: This check shouldn't be necessary!
@afterOtherCollisionCheck:

@setSpriteCollisions:
  ldx _CurrentSpriteIndex
  lda tmp3
  sta _SpriteCollisions,x

@done:
  rts
