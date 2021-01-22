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

;.importzp mathOperandLo1, mathOperandHi1
;.importzp mathOperandLo2, mathOperandHi2
;.importzp mathTmp1, mathTmp2
;.import AddSignedByteToSignedWord

.import GetTileMapCellCollisionCode

;.include "c64.asm"

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

_SpriteCollisionMasks:
  .res NUMBER_OF_SPRITES

_SpriteCollisions:
  .res NUMBER_OF_SPRITES

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to move.
_MoveSprite:
  tax ; Cache spriteIndex.
  stx tmp1

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
  ldx tmp1
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
  ldx tmp1
  lda _SpriteCollisionMasks,x
  beq @afterCollisionChecking

  jsr CheckSpriteCollision
@afterCollisionChecking:

@updateSpritePosition:
  ldx tmp1
  lda TempSpritePositionX
  sta SpritePositionsXLo,x
  lda TempSpritePositionX+1
  sta SpritePositionsXHi,x

  lda TempSpritePositionY
  sta SpritePositionsYLo,x
  lda TempSpritePositionY+1
  sta SpritePositionsYHi,x

  lda tmp1
  pha
  jsr UpdateSpritePositionX
  pla
  jsr UpdateSpritePositionY

  rts

;------------------------------------------------------------------
; Checks for collision map overlap with temporary sprite position.
; Adjusts this position to original X or Y position, depending on velocity.
; Assumes sprite dimensions of 16x16.
;
; inputs:
;  - spriteIndex: tmp1, which sprite to move.
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
  ldx tmp1
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

  ldx tmp1
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
  ldx tmp1
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

  ldx tmp1
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
  ldx tmp1
  lda tmp3
  sta _SpriteCollisions,x

@done:
  rts
