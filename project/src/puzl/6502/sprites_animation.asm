; 6502 sprites_animation.asm

.export InitializeSpritesAnimation
.export _SetSpriteAnimationSet
.export _PlaySpriteAnimation
.export _StopSpriteAnimation

;.autoimport on
  
;.importzp sp, sreg, regsave, regbank
;.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
;.macpack longbranch

.importzp sPtr1
.importzp SequencePositionLo
.importzp SequencePositionHi
.importzp SequenceSegmentDuration
.importzp SequenceSegmentDurationCounter
.importzp SequenceTempFetch

.segment "BSS"

SpriteAnimationSetsLo:
  .res NUMBER_OF_SPRITES

SpriteAnimationSetsHi:
  .res NUMBER_OF_SPRITES

.segment "ZEROPAGE"

SpriteAnimationIds:
  .res NUMBER_OF_SPRITES

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
  sta _SpriteCollisionMasks,x
  sta _SpriteCollisions,x

  dex
  bpl @resetSpriteAnimationsLoop

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
