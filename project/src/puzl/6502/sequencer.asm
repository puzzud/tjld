; 6502 sequencer.asm
; base implementation.

.export InitializeSequencer
.export PlaySequence
.export StopSequence
.export ProcessSequences

.exportzp sPtr1
.exportzp SequencePositionLo
.exportzp SequencePositionHi
.exportzp SequenceSegmentDuration
.exportzp SequenceSegmentDurationCounter
.exportzp SequenceTempFetch

.export ProcessSequenceDatum
.export OnSequenceSegmentEnd

.autoimport on

.importzp sp, sreg, regsave, regbank  
.importzp ptr1
.macpack longbranch

; TODO: Properly source these constants.
NUMBER_OF_VOICES = 3
NUMBER_OF_SEQUENCES = (NUMBER_OF_VOICES + 8)
SEQUENCE_TYPE_MUSIC = 0
SEQUENCE_TYPE_ANIMATION = 1
NUMBER_OF_SEQUENCE_TYPES = 2

.segment "RODATA"

SequenceTypes:
  .byte SEQUENCE_TYPE_MUSIC
  .byte SEQUENCE_TYPE_MUSIC
  .byte SEQUENCE_TYPE_MUSIC
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION
  .byte SEQUENCE_TYPE_ANIMATION

SequenceChannelIds:
  .byte 0
  .byte 1
  .byte 2
  .byte 0
  .byte 1
  .byte 2
  .byte 3
  .byte 4
  .byte 5
  .byte 6
  .byte 7

.segment "BSS"

SequenceStartLo:
  .res 1 * NUMBER_OF_SEQUENCES

SequenceStartHi:
  .res 1 * NUMBER_OF_SEQUENCES

SequenceLooping:
  .res 1 * NUMBER_OF_SEQUENCES

ProcessSequenceDatum:
  .res 2 * NUMBER_OF_SEQUENCES

OnSequenceSegmentEnd:
  .res 2 * NUMBER_OF_SEQUENCES

.segment "ZEROPAGE"

sPtr1: ; NOTE: Not using ptr1, because this is expected to run in an interrupt.
  .res 2

sPtr2: ; NOTE: Not using ptr2, same as sPtr1.
  .res 2

SequenceStatus:
  .res 1 * NUMBER_OF_SEQUENCES

SequencePositionLo:
  .res 1 * NUMBER_OF_SEQUENCES

SequencePositionHi:
  .res 1 * NUMBER_OF_SEQUENCES

SequenceSegmentDuration:
  .res 1 * NUMBER_OF_SEQUENCES

SequenceSegmentDurationCounter:
  .res 1 * NUMBER_OF_SEQUENCES

SequenceTempFetch:
  .res 1

.segment "CODE"

;---------------------------------------
; InitializeSequencer
;
; inputs:
InitializeSequencer:
  ldx #NUMBER_OF_SEQUENCES-1
@loop:
  jsr StopSequence

  dex
  bpl @loop

  ; Clear sequence type callbacks.
  lda #0
  ldx #((2*2)-1) ; Word / pointer size minus 1.
@clearCallbacksLoop:
  sta ProcessSequenceDatum,x
  sta OnSequenceSegmentEnd,x

  dex
  bpl @clearCallbacksLoop

  rts

;---------------------------------------
; inputs:
;  - sequenceIndex: x, which voice to play this pattern with.
;  - voiceStart: ptr1+1/ptr1, address of audio pattern data to play.
;  - looping: a, indicate whether pattern should loop.
PlaySequence:
  pha

  lda ptr1+1
  sta SequenceStartHi,x
  lda ptr1
  sta SequenceStartLo,x
  
  pla
  jsr StartSequence

  rts

;---------------------------------------
; inputs:
;  - sequenceIndex: x, which voice to start.
;  - looping: a, indicate whether pattern should loop.
StartSequence:
  sta SequenceLooping,x
  
  ; Load music vectors into music engine voice music data vector (counters).
  lda SequenceStartLo,x
  sta SequencePositionLo,x
  lda SequenceStartHi,x
  sta SequencePositionHi,x

  ; Disable all voice music processing.
  lda #0
  sta SequenceSegmentDurationCounter,x

  tay
  dey
  tya
  sta SequenceStatus,x

  rts

;---------------------------------------
; inputs:
;  - sequenceIndex: x, index of which voice to stop.
StopSequence:
  lda #0
  sta SequenceSegmentDurationCounter,x
  sta SequenceStatus,x

  rts

;---------------------------------------
; Start of all voice/music processing.
ProcessSequences:
  ldx #NUMBER_OF_SEQUENCES-1
@loop:
  lda SequenceStatus,x
  beq @afterProcessSequence

  jsr ProcessSequence

@afterProcessSequence:
  dex
  bpl @loop

@done:
  rts

;---------------------------------------
; inputs:
;  - sequenceIndex: x, which voice to process music.
ProcessSequence:
  lda SequenceSegmentDurationCounter,x
  bne @ProcessSequencesDurAndRel

  jsr ProcessSequenceData

; Process music engine voice time to release and duration.
@ProcessSequencesDurAndRel:
  lda SequenceSegmentDurationCounter,x
  cmp #1
  bne @decrementDurationCounter

  ; Check and JSR OnSequenceSegmentEnd hook.
  lda SequenceTypes,x
  asl
  tay
  lda OnSequenceSegmentEnd,y
  sta sPtr2
  lda OnSequenceSegmentEnd+1,y
  sta sPtr2+1
  ora sPtr2
  beq @afterOnSequenceSegmentEnd

  lda #>(@afterOnSequenceSegmentEnd-1)
  pha
  lda #<(@afterOnSequenceSegmentEnd-1)
  pha
  jmp (sPtr2)
@afterOnSequenceSegmentEnd:

@decrementDurationCounter:
  dec SequenceSegmentDurationCounter,x
  
  rts

;---------------------------------------
; inputs:
;  - sequenceIndex: x, which voice to fetch and process music from.
ProcessSequenceData:
  ; Fetch current byte and cache for later analysis.
  lda SequencePositionLo,x
  sta sPtr1
  lda SequencePositionHi,x
  sta sPtr1+1
  
  ldy #0
  lda (sPtr1),y
  sta SequenceTempFetch

  ; Check if at end of pattern.
  bne @processPatternDatum

  lda SequenceLooping,x
  bne @resetSequence

  ; No looping, so do no fetching.
  rts

; Reset music engine vectors to currently set base music data vectors.
@resetSequence:
  lda SequenceStartLo,x
  sta SequencePositionLo,x
  sta sPtr1
  lda SequenceStartHi,x
  sta SequencePositionHi,x
  sta sPtr1+1

  lda #0
  sta SequenceSegmentDurationCounter,x
  
  ; Fetch first byte after reset (NOTE: sPtr1 reset above).
  lda (sPtr1),y
  sta SequenceTempFetch

; Start sequence data processing.
@processPatternDatum:
  ; Check and JSR ProcessSequenceDatum hook.
  lda SequenceTypes,x
  asl
  tay
  lda ProcessSequenceDatum,y
  sta sPtr2
  lda ProcessSequenceDatum+1,y
  sta sPtr2+1
  ora sPtr2
  beq @afterProcessSequenceDatum

  lda #>(@afterProcessSequenceDatum-1)
  pha
  lda #<(@afterProcessSequenceDatum-1)
  pha
  jmp (sPtr2) ; 
@afterProcessSequenceDatum:

  ; Increase sequence pointer.
  inc SequencePositionLo,x
  bne @skipIncrementCarry
  inc SequencePositionHi,x
@skipIncrementCarry:

  rts
