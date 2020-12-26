; c64 sprites.asm

.export InitializeSpritesAnimation
.export _EnableSprite
.export _GetSpritePositionX
.export _GetSpritePositionY
.export _SetSpritePositionX
.export _SetSpritePositionY
.export _SetSpriteVelocity
.export _MoveSprite
.export _SetSpriteFrameIndex
.export _SetSpriteColor
.export _SetSpriteSeconaryColor
.export _SetSpriteTertiaryColor
.export _SetSpriteAnimationSet
.export _PlaySpriteAnimation
.export _StopSpriteAnimation

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.importzp mathOperandLo1, mathOperandHi1
.importzp mathOperandLo2, mathOperandHi2
.importzp mathTmp1, mathTmp2
.import AddSignedByteToSignedWord

.import GetTileMapCellCollisionCode

.importzp sPtr1
.importzp SequencePositionLo
.importzp SequencePositionHi
.importzp SequenceSegmentDuration
.importzp SequenceSegmentDurationCounter
.importzp SequenceTempFetch

.import _NthBitFlags
.import _InverseNthBitFlags
.import _SpritePositionsX
.import _SpritePositionsY
.import _SpriteVelocitiesX
.import _SpriteVelocitiesY

.include "c64.asm"

; TODO: Need to figure how to get this number from puzl/video.h.
NUMBER_OF_SPRITES = NUMBER_OF_HARDWARE_SPRITES

.segment "BSS"

SpriteAnimationSetsLo:
  .res NUMBER_OF_SPRITES

SpriteAnimationSetsHi:
  .res NUMBER_OF_SPRITES

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

SpriteVelocitiesX:
  .res NUMBER_OF_SPRITES

SpriteVelocitiesY:
  .res NUMBER_OF_SPRITES

SpriteAnimationIds:
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

.segment "CODE"

;------------------------------------------------------------------
InitializeSpritesAnimation:
  ; NOTE: This could just be assembled in a sequence of data.
  ; TODO: SEQUENCE_TYPE_ANIMATION = 1, need to source this value and in theory use indexing.
  lda #<ProcessSpriteAnimationDatum
  sta ProcessSequenceDatum+(1*2)
  lda #>ProcessSpriteAnimationDatum
  sta ProcessSequenceDatum+(1*2)+1

  ; NOTE: OnSequenceSegmentEnd should already be null (saving space here).
  ;lda #0
  ;sta OnSequenceSegmentEnd+(1*2)
  ;sta OnSequenceSegmentEnd+(1*2)+1

  ; Set all sprite animation IDs to -1.
  ldx #NUMBER_OF_SPRITES-1
@resetSpriteAnimationsLoop:
  lda #$ff
  sta SpriteAnimationIds,x
  lda #$00
  sta SpriteAnimationSetsLo,x
  sta SpriteAnimationSetsHi,x

  dex
  bpl @resetSpriteAnimationsLoop

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to enable / disable.
;  - enable: a, enable or disable sprite.
_EnableSprite:
  tax

  ldy #0
  lda (sp),y
  tay

  txa
  bne @enable

@disable:
  lda _InverseNthBitFlags,y
  and SPENA

  jmp @set

@enable:
  lda _NthBitFlags,y
  ora SPENA

@set:
  sta SPENA

@done:
  jmp incsp1

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
; Update C64 sprite register with the value in _SpritePositionsX.
;
; inputs:
;  - spriteIndex: a, which sprite to update position x.
; notes:
;  - Squashes a, x, y.
UpdateSpritePositionX:
  tax ; Cache spriteIndex.
  ; spriteIndex * 2, as y register offset.
  asl
  tay

  lda SpritePositionsXLo,x

  clc
  adc #SCREEN_BORDER_THICKNESS_X
  sta SP0X,y
  lda #0
  adc SpritePositionsXHi,x
  cmp #0
  beq @before256
  bcc @before256

@after255:
  lda _NthBitFlags,x
  ora MSIGX

  ;jmp @done
  bne @done ; NOTE: Assumption is that A has at least one bit set.

@before256:
  lda _InverseNthBitFlags,x
  and MSIGX

@done:
  sta MSIGX
  
  rts

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
; Update C64 sprite register with the value in _SpritePositionsY.
;
; inputs:
;  - spriteIndex: a, which sprite to update position y.
UpdateSpritePositionY:
  tax ; Cache spriteIndex.
  ; spriteIndex * 2, as y register offset.
  asl
  tay

  lda SpritePositionsYLo,x

  ; NOTE: Only low byte is necessary (no x, no 16 bit addition).
  clc
  adc #SCREEN_BORDER_THICKNESS_Y
  sta SP0Y,y
  
  rts

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
; inputs:
;  - spriteIndex: sp[1], which sprite to set velocity.
;  - x: sp[0], x velocity (signed).
;  - y: a, y velocity (signed).
_SetSpriteVelocity:
  ; Cache a register on CPU stack.
  pha

  ; Cache x velocity in x register.
  ldy #0
  lda (sp),y
  tax

  ; spriteIndex, as y register offset.
  iny
  lda (sp),y
  tay

  txa
  sta SpriteVelocitiesX,y

  pla
  sta SpriteVelocitiesY,y

  jmp incsp2

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: a, which sprite to move.
_MoveSprite:
  tax ; Cache spriteIndex.
  stx tmp1

@setX:
  lda SpriteVelocitiesX,x
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
  lda SpriteVelocitiesY,x
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
  
  ; TODO: Make collision map optional.
  ;lda #1
  ;beq @afterCollisionChecking

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
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
; outputs:
;  - TempSpritePositionX+1/TempSpritePositionX
;  - TempSpritePositionY+1/TempSpritePositionY
CheckSpriteCollision:
  jsr GetCornerTiles

@checkX:
  ldx tmp1
  lda SpriteVelocitiesX,x
  beq @afterCheckX
  bpl @xVelocityPositive

@xVelocityNegative:
  ldy UpperLeftSpriteTileX
  ldx UpperLeftSpriteTileY
  jsr GetTileMapCellCollisionCode
  beq @afterCheckX
  ldx LowerRightSpriteTileY
  jsr GetTileMapCellCollisionCode
  beq @afterCheckX

@resetPositionX:
  ldx tmp1
  lda SpritePositionsXLo,x
  sta TempSpritePositionX
  lda SpritePositionsXHi,x
  sta TempSpritePositionX+1

  jmp @afterCheckX

@xVelocityPositive:
  ldy LowerRightSpriteTileX
  ldx UpperLeftSpriteTileY
  jsr GetTileMapCellCollisionCode
  beq @afterCheckX
  ldx LowerRightSpriteTileY
  jsr GetTileMapCellCollisionCode
  bne @resetPositionX ; NOTE: Not beq.
@afterCheckX:

@checkY:
  ldx tmp1
  lda SpriteVelocitiesY,x
  beq @afterCheckY
  bpl @yVelocityPositive

@yVelocityNegative:
  ldy UpperLeftSpriteTileX
  ldx UpperLeftSpriteTileY
  jsr GetTileMapCellCollisionCode
  beq @afterCheckY
  ldy LowerRightSpriteTileX
  jsr GetTileMapCellCollisionCode
  beq @afterCheckY

@resetPositionY:
  ldx tmp1
  lda SpritePositionsYLo,x
  sta TempSpritePositionY
  lda SpritePositionsYHi,x
  sta TempSpritePositionY+1

  jmp @afterCheckY

@yVelocityPositive:
  ldy UpperLeftSpriteTileX
  ldx LowerRightSpriteTileY
  jsr GetTileMapCellCollisionCode
  beq @afterCheckY
  ldy LowerRightSpriteTileX
  jsr GetTileMapCellCollisionCode
  bne @resetPositionY ; NOTE: Not beq.
@afterCheckY:

@done:
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
GetCornerTiles:
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
  lda #(16-1)
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
  lda #(16-1)
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

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: x, which sprite to assign frame index.
;  - frameIndex: a, index of graphic frame.
; notes:
;  - Intended call from assembly.
SetSpriteFrameIndex:
  clc
  adc #64 ; TODO: Need to investigate why 64 is a base.
  sta SPRITE_POINTER_BASE,x

  rts

;------------------------------------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to assign color.
;  - colorCode: a, color code.
_SetSpriteColor:
  tax

  ldy #0
  lda (sp),y
  tay

  txa
  sta SP0COL,y

  jmp incsp1

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
_SetSpriteSeconaryColor:
  sta SPMC0

  rts

;------------------------------------------------------------------
; inputs:
;  - colorCode: a, color code.
_SetSpriteTertiaryColor:
  sta SPMC1

  rts

;---------------------------------------
; inputs:
;  - spriteIndex: sp[0], which sprite to play this animation with.
;  - animationSet: x/a, address of animation data to play.
_SetSpriteAnimationSet:
  pha
  txa
  pha

  ; spriteIndex
  ldy #0
  lda (sp),y
  tax

  pla
  sta SpriteAnimationSetsHi,x

  pla
  sta SpriteAnimationSetsLo,x

  jmp incsp1

;---------------------------------------
; inputs:
;  - spriteIndex: sp[1], which sprite to play this animation with.
;  - animationId: sp[0], id entry in animation set for animation data to play.
;  - looping: a, indicate whether animation should loop.
_PlaySpriteAnimation:
  sta tmp1
  
  ldy #1
  lda (sp),y
  tax

  dey ; y=0
  lda (sp),y
  cmp SpriteAnimationIds,x
  beq @done

  sta SpriteAnimationIds,x
  asl
  tay

  lda SpriteAnimationSetsLo,x
  sta ptr2
  lda SpriteAnimationSetsHi,x
  sta ptr2+1

  lda (ptr2),y
  sta ptr1
  iny
  lda (ptr2),y
  sta ptr1+1

  ; TODO: Properly determine sequence from sprite index.
  txa
  clc
  adc #3
  tax ; Cache normal offset for call to PlaySequence.

  lda tmp1
  jsr PlaySequence

@done:
  jmp incsp2

;---------------------------------------
; inputs:
;  - spriteIndex: a, index of which voice to stop.
_StopSpriteAnimation:
  ; TODO: Properly determine sequence from sprite index.
  clc
  adc #3
  tax
  jmp StopSequence
  ;rts

;---------------------------------------
; inputs:
;  - sequenceIndex: x, which sequence to fetch and process data from.
;  - sequenceTempFetch: SequenceTempFetch, current datum just read.
;  - sequencerPosition: sPtr1+1/sPtr1, where sequencer is looking at in sequence.
; notes:
;  - Preserves x.
ProcessSpriteAnimationDatum:
  ; Cutoff bits 7.
  ; The first seven bits of this byte are the animation frame index.
  txa
  pha ; Cache x.

  ; For SetSpriteFrameIndex, set x temporarily to
  ; voiceIndex from sequenceIndex.
  sec
  sbc #3
  tax

  lda SequenceTempFetch
  ; TODO: Make this literal use flag constants.
  and #%01111111
  
  ; TODO: Translate to sequence channel ID.
  jsr SetSpriteFrameIndex
  
  ; Restore x.
  pla
  tax

  ; Now check bit 7.
  bit SequenceTempFetch
  bpl @updateDuration

  ; Increase sequence pointer.
  inc sPtr1
  bne @skipIncrementCarry
  inc sPtr1+1
@skipIncrementCarry:
  lda sPtr1
  sta SequencePositionLo,x
  lda sPtr1+1
  sta SequencePositionHi,x

  ; Fetch and store next byte.
  ldy #0
  lda (sPtr1),y
  sta SequenceSegmentDuration,x

@updateDuration:
  lda SequenceSegmentDuration,x
  sta SequenceSegmentDurationCounter,x

  rts
