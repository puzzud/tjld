; 6502 music_engine.asm
; base implementation.

.export InitializeMusicEngine
.export _SetMusicVoice
.export _StartMusic
.export _StopMusic
.export ProcessMusic

.exportzp _MusicStatus

.export _MusicEngineV1MusicStart
.export _MusicEngineV1MusicEnd
.export _MusicEngineV2MusicStart
.export _MusicEngineV2MusicEnd
.export _MusicEngineV3MusicStart
.export _MusicEngineV3MusicEnd

.export _MusicEngineV1TimeToRelease
.export _MusicEngineV2TimeToRelease
.export _MusicEngineV3TimeToRelease

.autoimport on
  
;.importzp sp, sreg, regsave, regbank
;.importzp tmp1, tmp2, tmp3, tmp4, mePtr1, ptr2, ptr3, ptr4
;.macpack longbranch

.import _SoundKillAll

.segment "BSS"

_MusicEngineV1MusicStart:
MusicEngineV1MusicStart:
  .res 2

_MusicEngineV2MusicStart:
MusicEngineV2MusicStart:
  .res 2

_MusicEngineV3MusicStart:
MusicEngineV3MusicStart:
  .res 2

_MusicEngineV1MusicEnd:
MusicEngineV1MusicEnd:
  .res 2

_MusicEngineV2MusicEnd:
MusicEngineV2MusicEnd:
  .res 2

_MusicEngineV3MusicEnd:
MusicEngineV3MusicEnd:
  .res 2

_MusicEngineV1TimeToRelease:
MusicEngineV1TimeToRelease:
  .res 1

_MusicEngineV2TimeToRelease:
MusicEngineV2TimeToRelease:
  .res 1

_MusicEngineV3TimeToRelease:
MusicEngineV3TimeToRelease:
  .res 1

.segment "ZEROPAGE"

mePtr1: ; NOTE: Not using ptr1, because this is expected to run in an interrupt.
  .res 2

_MusicStatus:
MusicStatus:
  .res 1

MusicEngineV1Position:
  .res 2
  
MusicEngineV2Position:
  .res 2
  
MusicEngineV3Position:
  .res 2

MusicEngineV1Duration:
  .res 1

MusicEngineV2Duration:
  .res 1

MusicEngineV3Duration:
  .res 1

MusicEngineV1Active:
  .res 1

MusicEngineV2Active:
  .res 1

MusicEngineV3Active:
  .res 1

MusicEngineV1TimeToReleaseCounter:
  .res 1

MusicEngineV2TimeToReleaseCounter:
  .res 1

MusicEngineV3TimeToReleaseCounter:
  .res 1

MusicEngineV1DurationCounter:
  .res 1

MusicEngineV2DurationCounter:
  .res 1

MusicEngineV3DurationCounter:
  .res 1

MusicEngineTempFetch:
  .res 1

; MULE music engine.
;
; It provides the following major routines:
; _InitializeMusic
; _StartMusic
; _ProcessMusic

; The encoding for the music is as follows:
; - The first byte fetched will always contain an index to a note table, based on the lower 6 bits.
; - The first byte's 7th bit indicates a duration change (how much music time this byte takes up).
;   - If a duration change is indicated, a second byte is fetched.
;   - The second byte's value acts as a delay (duration) counter, which is decremented every music engine frame.
;   - While this duration counter is positive, no more bytes from the corresponding voice's music data are processed.
;   - Once the duration is zero (non-positive), music data fetching commences, restarting the cycle with the first byte fetch.
; - The first byte's 6th bit indicates a voice gate change.
;   - High will start the silencing or release of the voice.
;   - Low will either start the voice attack or will maintain the voice's active status.
; - Unless a duration change is made, all byte fetches are considered the 'first' byte.

; - Durations:
;   - $0c: 1/8 note.
;   - $c0: 8 beats rest.
NOTE_LENGTH_1_96TH = 1
NOTE_LENGTH_1_32ND = NOTE_LENGTH_1_96TH * 3 ; 3
NOTE_LENGTH_1_16TH = NOTE_LENGTH_1_32ND * 2 ; 6
NOTE_LENGTH_1_8TH  = NOTE_LENGTH_1_16TH * 2 ; 12
NOTE_LENGTH_1_4TH  = NOTE_LENGTH_1_8TH  * 2 ; 24
NOTE_LENGTH_1_HALF = NOTE_LENGTH_1_4TH  * 2 ; 48
NOTE_LENGTH_1      = NOTE_LENGTH_1_HALF * 2 ; 96

NOTE_CONTROL_LENGTH = %10000000
NOTE_CONTROL_GATE   = %01000000

; Note tables:
NOTE_C  = 0
NOTE_CS = 1
NOTE_D  = 2
NOTE_DS = 3
NOTE_E  = 4
NOTE_F  = 5
NOTE_FS = 6
NOTE_G  = 7
NOTE_GS = 8
NOTE_A  = 9
NOTE_AS = 10
NOTE_B  = 11

NUMBER_OF_NOTES_IN_OCTAVE = 12
OCTAVE = NUMBER_OF_NOTES_IN_OCTAVE

.segment "CODE"

MusicEngineNoteFreqTableHi1C:
  .byte >NOTE_FREQ_1_C
  .byte >NOTE_FREQ_1_CS
  .byte >NOTE_FREQ_1_D
  .byte >NOTE_FREQ_1_DS
  .byte >NOTE_FREQ_1_E
  .byte >NOTE_FREQ_1_F
  .byte >NOTE_FREQ_1_FS
  .byte >NOTE_FREQ_1_G
  .byte >NOTE_FREQ_1_GS
  .byte >NOTE_FREQ_1_A
  .byte >NOTE_FREQ_1_AS
  .byte >NOTE_FREQ_1_B
  .byte >NOTE_FREQ_2_C
  .byte >NOTE_FREQ_2_CS
  .byte >NOTE_FREQ_2_D
  .byte >NOTE_FREQ_2_DS
  .byte >NOTE_FREQ_2_E
  .byte >NOTE_FREQ_2_F
  .byte >NOTE_FREQ_2_FS
  .byte >NOTE_FREQ_2_G
  .byte >NOTE_FREQ_2_GS
  .byte >NOTE_FREQ_2_A
  .byte >NOTE_FREQ_2_AS
  .byte >NOTE_FREQ_2_B      
  .byte >NOTE_FREQ_3_C
  .byte >NOTE_FREQ_3_CS
  .byte >NOTE_FREQ_3_D
  .byte >NOTE_FREQ_3_DS
  .byte >NOTE_FREQ_3_E
  .byte >NOTE_FREQ_3_F
  .byte >NOTE_FREQ_3_FS
  .byte >NOTE_FREQ_3_G
  .byte >NOTE_FREQ_3_GS
  .byte >NOTE_FREQ_3_A
  .byte >NOTE_FREQ_3_AS
  .byte >NOTE_FREQ_3_B
  .byte >NOTE_FREQ_4_C
  .byte >NOTE_FREQ_4_CS
  .byte >NOTE_FREQ_4_D
  .byte >NOTE_FREQ_4_DS
  .byte >NOTE_FREQ_4_E
  .byte >NOTE_FREQ_4_F
  .byte >NOTE_FREQ_4_FS
  .byte >NOTE_FREQ_4_G
  .byte >NOTE_FREQ_4_GS
  .byte >NOTE_FREQ_4_A
  .byte >NOTE_FREQ_4_AS
  .byte >NOTE_FREQ_4_B
  .byte >NOTE_FREQ_5_C
  .byte >NOTE_FREQ_5_CS
  .byte >NOTE_FREQ_5_D
  .byte >NOTE_FREQ_5_DS
  .byte >NOTE_FREQ_5_E
  .byte >NOTE_FREQ_5_F
  .byte >NOTE_FREQ_5_FS
  .byte >NOTE_FREQ_5_G
  .byte >NOTE_FREQ_5_GS
  .byte >NOTE_FREQ_5_A
  .byte >NOTE_FREQ_5_AS
  .byte >NOTE_FREQ_5_B
  .byte >NOTE_FREQ_6_C
  .byte >NOTE_FREQ_6_CS
  .byte >NOTE_FREQ_6_D
  .byte >NOTE_FREQ_6_DS
  .byte >NOTE_FREQ_6_E
  .byte >NOTE_FREQ_6_F
  .byte >NOTE_FREQ_6_FS
  .byte >NOTE_FREQ_6_G
  .byte >NOTE_FREQ_6_GS
  .byte >NOTE_FREQ_6_A
  .byte >NOTE_FREQ_6_AS
  .byte >NOTE_FREQ_6_B

MusicEngineNoteFreqTableLo1C:
  .byte <NOTE_FREQ_1_C
  .byte <NOTE_FREQ_1_CS
  .byte <NOTE_FREQ_1_D
  .byte <NOTE_FREQ_1_DS
  .byte <NOTE_FREQ_1_E
  .byte <NOTE_FREQ_1_F
  .byte <NOTE_FREQ_1_FS
  .byte <NOTE_FREQ_1_G
  .byte <NOTE_FREQ_1_GS
  .byte <NOTE_FREQ_1_A
  .byte <NOTE_FREQ_1_AS
  .byte <NOTE_FREQ_1_B
  .byte <NOTE_FREQ_2_C
  .byte <NOTE_FREQ_2_CS
  .byte <NOTE_FREQ_2_D
  .byte <NOTE_FREQ_2_DS
  .byte <NOTE_FREQ_2_E
  .byte <NOTE_FREQ_2_F
  .byte <NOTE_FREQ_2_FS
  .byte <NOTE_FREQ_2_G
  .byte <NOTE_FREQ_2_GS
  .byte <NOTE_FREQ_2_A
  .byte <NOTE_FREQ_2_AS
  .byte <NOTE_FREQ_2_B 
  .byte <NOTE_FREQ_3_C
  .byte <NOTE_FREQ_3_CS
  .byte <NOTE_FREQ_3_D
  .byte <NOTE_FREQ_3_DS
  .byte <NOTE_FREQ_3_E
  .byte <NOTE_FREQ_3_F
  .byte <NOTE_FREQ_3_FS
  .byte <NOTE_FREQ_3_G
  .byte <NOTE_FREQ_3_GS
  .byte <NOTE_FREQ_3_A
  .byte <NOTE_FREQ_3_AS
  .byte <NOTE_FREQ_3_B
  .byte <NOTE_FREQ_4_C
  .byte <NOTE_FREQ_4_CS
  .byte <NOTE_FREQ_4_D
  .byte <NOTE_FREQ_4_DS
  .byte <NOTE_FREQ_4_E
  .byte <NOTE_FREQ_4_F
  .byte <NOTE_FREQ_4_FS
  .byte <NOTE_FREQ_4_G
  .byte <NOTE_FREQ_4_GS
  .byte <NOTE_FREQ_4_A
  .byte <NOTE_FREQ_4_AS
  .byte <NOTE_FREQ_4_B
  .byte <NOTE_FREQ_5_C
  .byte <NOTE_FREQ_5_CS
  .byte <NOTE_FREQ_5_D
  .byte <NOTE_FREQ_5_DS
  .byte <NOTE_FREQ_5_E
  .byte <NOTE_FREQ_5_F
  .byte <NOTE_FREQ_5_FS
  .byte <NOTE_FREQ_5_G
  .byte <NOTE_FREQ_5_GS
  .byte <NOTE_FREQ_5_A
  .byte <NOTE_FREQ_5_AS
  .byte <NOTE_FREQ_5_B
  .byte <NOTE_FREQ_6_C
  .byte <NOTE_FREQ_6_CS
  .byte <NOTE_FREQ_6_D
  .byte <NOTE_FREQ_6_DS
  .byte <NOTE_FREQ_6_E
  .byte <NOTE_FREQ_6_F
  .byte <NOTE_FREQ_6_FS
  .byte <NOTE_FREQ_6_G
  .byte <NOTE_FREQ_6_GS
  .byte <NOTE_FREQ_6_A
  .byte <NOTE_FREQ_6_AS
  .byte <NOTE_FREQ_6_B

MusicEngineNoteFreqTableHi2C = MusicEngineNoteFreqTableHi1C + (NUMBER_OF_NOTES_IN_OCTAVE * 1)
MusicEngineNoteFreqTableHi3C = MusicEngineNoteFreqTableHi1C + (NUMBER_OF_NOTES_IN_OCTAVE * 2)
MusicEngineNoteFreqTableHi4C = MusicEngineNoteFreqTableHi1C + (NUMBER_OF_NOTES_IN_OCTAVE * 3)
MusicEngineNoteFreqTableHi5C = MusicEngineNoteFreqTableHi1C + (NUMBER_OF_NOTES_IN_OCTAVE * 4)
MusicEngineNoteFreqTableHi6C = MusicEngineNoteFreqTableHi1C + (NUMBER_OF_NOTES_IN_OCTAVE * 5)

MusicEngineNoteFreqTableLo2C = MusicEngineNoteFreqTableLo1C + (NUMBER_OF_NOTES_IN_OCTAVE * 1)
MusicEngineNoteFreqTableLo3C = MusicEngineNoteFreqTableLo1C + (NUMBER_OF_NOTES_IN_OCTAVE * 2)
MusicEngineNoteFreqTableLo4C = MusicEngineNoteFreqTableLo1C + (NUMBER_OF_NOTES_IN_OCTAVE * 3)
MusicEngineNoteFreqTableLo5C = MusicEngineNoteFreqTableLo1C + (NUMBER_OF_NOTES_IN_OCTAVE * 4)
MusicEngineNoteFreqTableLo6C = MusicEngineNoteFreqTableLo1C + (NUMBER_OF_NOTES_IN_OCTAVE * 5)

;---------------------------------------
; InitializeMusicEngine
;
; inputs:
;  - A: newMusicStatus, what to set MusicStatus to.
InitializeMusicEngine:
  jsr StopMusic

  rts

;---------------------------------------
_SetMusicVoice:
  jsr pushax
  
  jsr ldax0sp
  sta mePtr1
  stx mePtr1+1
  
  ; voiceIndex
  ldy #2
  lda (sp),y
  asl
  tay

  lda mePtr1
  sta MusicEngineV1MusicStart,y
  lda mePtr1+1
  sta MusicEngineV1MusicStart+1,y
  
  jmp incsp3
  
;---------------------------------------
_StopMusic:
StopMusic:
  ; Disable all voice music processing.
  lda #0
  sta MusicEngineV1Active
  sta MusicEngineV2Active
  sta MusicEngineV3Active
  
  sta MusicStatus
  
  jsr _SoundKillAll

  rts
  
;---------------------------------------
_StartMusic:
  ; Calculate these rather than setting them.
  ldx #0
  jsr CalculateMusicVoiceEnd
  inx
  jsr CalculateMusicVoiceEnd
  inx
  jsr CalculateMusicVoiceEnd
  
  ; TODO: Perhaps it's possible to integrate
  ; release time better with NES voice timed envelopes?
  lda #$1e
  sta MusicEngineV1TimeToRelease
  sta MusicEngineV2TimeToRelease
  sta MusicEngineV3TimeToRelease
  
  ; Load music vectors into music engine voice music data vector (counters).
  lda MusicEngineV1MusicStart
  sta MusicEngineV1Position
  lda MusicEngineV1MusicStart+1
  sta MusicEngineV1Position+1

  lda MusicEngineV2MusicStart
  sta MusicEngineV2Position
  lda MusicEngineV2MusicStart+1
  sta MusicEngineV2Position+1

  lda MusicEngineV3MusicStart
  sta MusicEngineV3Position
  lda MusicEngineV3MusicStart+1
  sta MusicEngineV3Position+1

  ; Disable all voice music processing.
  ldx #0
  stx MusicEngineV1Active
  stx MusicEngineV2Active
  stx MusicEngineV3Active
  
  dex ; X=$ff
  stx MusicStatus

  rts

; CalculateMusicVoiceEnd
; Populate the corresponding MusicEngineV*MusicEnd
; pointer from its current starting point.
;
; inputs:
;  - voiceIndex: X, indicates which music voice to calculate.
;  - MusicEngineV1MusicStart + (2 * voiceIndex): start of music pointer should be set before calling.
;
; outputs:
;  - MusicEnd: mePtr1, 
CalculateMusicVoiceEnd:
  ; Set the correct music start pointer according to voiceIndex.
  txa
  asl
  tay

  lda MusicEngineV1MusicStart,y
  sta mePtr1
  lda MusicEngineV1MusicStart+1,y
  sta mePtr1+1
  
  ; Set the correct music start pointer according to voiceIndex.
; @setMusicStartPointer:
;   clc
;   txa
;   adc mePtr1
;   sta mePtr1
;   bcc @endSetMusicStartPointer
;   inc mePtr1+1
; @endSetMusicStartPointer:

@findNullMusicVoiceTerminator:
  ldy #0
@loop:
  lda (mePtr1),y
  beq @foundNullTerminator
  
  iny
  bne @loop
  
  inc mePtr1+1
  jmp @loop

@foundNullTerminator:
  ; Add current Y as offset to mePtr1.
  clc
  tya
  adc mePtr1
  sta mePtr1
  bcc @endYOffset
  inc mePtr1+1
@endYOffset:
  
  txa
  asl
  tay
  
  lda mePtr1
  sta MusicEngineV1MusicEnd,y
  
  lda mePtr1+1
  sta MusicEngineV1MusicEnd+1,y
  
  rts
  
; Start of all voice/music processing.
ProcessMusic:
  lda MusicStatus
  bne @startProcessing
  rts
@startProcessing:

CheckVoice1:
  lda MusicEngineV1Active
  beq A_70EC

  jmp CheckVoice2

A_70EC:
  lda MusicEngineV1MusicEnd
  cmp MusicEngineV1Position
  bne @processVoice1

  lda MusicEngineV1MusicEnd+1
  cmp MusicEngineV1Position+1
  bne @processVoice1

  ; Loop?
  ;jmp @resetMusicVectors
  ;jmp SoundKillAll

; Reset music engine vectors to currently set base music data vectors.
@resetMusicVectors:
  lda MusicEngineV1MusicStart
  sta MusicEngineV1Position
  lda MusicEngineV1MusicStart+1
  sta MusicEngineV1Position+1

  lda MusicEngineV2MusicStart
  sta MusicEngineV2Position
  lda MusicEngineV2MusicStart+1
  sta MusicEngineV2Position+1

  lda MusicEngineV3MusicStart
  sta MusicEngineV3Position
  lda MusicEngineV3MusicStart+1
  sta MusicEngineV3Position+1

  lda #0
  sta MusicEngineV1Active
  sta MusicEngineV2Active
  sta MusicEngineV3Active

; Start music data processing (of voice 1).
@processVoice1:
  ; Fetch current byte and cache for later for later analysis.
  ldy #0
  lda (MusicEngineV1Position),y
  sta MusicEngineTempFetch
  
  ; Cutoff bits 6 & 7.
  ; The first six bits of this byte are the music note index.
  and #%00111111
  
  ; Load Voice 1 Frequency.
  tax
  setVoiceFrequencyV1 ; macro
  
  ; Now check bit 7.
  bit MusicEngineTempFetch
  bpl A_7180

  ; Fetch and store next byte.
  iny
  lda (MusicEngineV1Position),y
  sta MusicEngineV1Duration

  ; Increase music pointer.
  inc MusicEngineV1Position
  bne A_7180
  inc MusicEngineV1Position+1
A_7180:
  inc MusicEngineV1Position
  bne A_7186
  inc MusicEngineV1Position+1

  ; Now check bit 6.
A_7186:
  bit MusicEngineTempFetch
  bvc A_7192

  ; Disable Voice 1.
  disableVoice1 ; macro
  
  jmp A_7197

A_7192:
  ; Gate Voice 1.
  enableVoice1 ; macro
  
A_7197:
  lda MusicEngineV1TimeToRelease
  sta MusicEngineV1TimeToReleaseCounter

  lda MusicEngineV1Duration
  sta MusicEngineV1DurationCounter

  lda #1
  sta MusicEngineV1Active

CheckVoice2:
  lda MusicEngineV2Active
  beq A_71B0

  jmp CheckVoice3

A_71B0:
  lda MusicEngineV2MusicEnd
  cmp MusicEngineV2Position
  bne @processVoice2

  lda MusicEngineV2MusicEnd+1
  cmp MusicEngineV2Position+1
  bne @processVoice2

  lda MusicEngineV2MusicStart          ; // Reset music data address (counter).
  sta MusicEngineV2Position
  lda MusicEngineV2MusicStart+1
  sta MusicEngineV2Position+1

; Start music data processing (of voice 2).
@processVoice2:
  ldy #0
  lda (MusicEngineV2Position),y
  sta MusicEngineTempFetch
  and #%00111111
  
  ; Load Voice 2 Frequency.
  tax
  setVoiceFrequencyV2 ; macro
  
  bit MusicEngineTempFetch
  bpl A_71EF

  iny
  lda (MusicEngineV2Position),y
  sta MusicEngineV2Duration
  inc MusicEngineV2Position
  bne A_71EF

  inc MusicEngineV2Position+1
A_71EF:
  inc MusicEngineV2Position
  bne A_71F5

  inc MusicEngineV2Position+1
A_71F5:
  bit MusicEngineTempFetch
  bvc A_7201

  ; Disable Voice 2.
  disableVoice2 ; macro
  
  jmp A_7206

A_7201:
  ; Gate Voice 2.
  enableVoice2 ; macro
  
A_7206:
  lda MusicEngineV2TimeToRelease
  sta MusicEngineV2TimeToReleaseCounter

  lda MusicEngineV2Duration
  sta MusicEngineV2DurationCounter

  lda #1
  sta MusicEngineV2Active

CheckVoice3:
  lda MusicEngineV3Active
  beq A_721F

  jmp ProcessMusicDurAndRel

A_721F:
  lda MusicEngineV3MusicEnd
  cmp MusicEngineV3Position
  bne @processVoice3
  
  lda MusicEngineV3MusicEnd+1
  cmp MusicEngineV3Position+1
  bne @processVoice3

  lda MusicEngineV3MusicStart
  sta MusicEngineV3Position
  lda MusicEngineV3MusicStart+1
  sta MusicEngineV3Position+1

@processVoice3:
  ldy #0
  lda (MusicEngineV3Position),y
  sta MusicEngineTempFetch
  and #%00111111
  
  ; Load Voice 3 Frequency.
  tax
  setVoiceFrequencyV3 ; macro
  
  bit MusicEngineTempFetch
  bpl V3A_71EF

  iny
  lda (MusicEngineV3Position),y
  sta MusicEngineV3Duration
  inc MusicEngineV3Position
  bne V3A_71EF

  inc MusicEngineV3Position+1
V3A_71EF:
  inc MusicEngineV3Position
  bne V3A_71F5

  inc MusicEngineV3Position+1
V3A_71F5:
  bit MusicEngineTempFetch
  bvc V3A_7201

  ; Disable Voice 3.
  disableVoice3 ; macro
  
  jmp V3A_7206

V3A_7201:
  ; Gate Voice 3.
  enableVoice3 ; macro
  
V3A_7206:
  lda MusicEngineV3TimeToRelease
  sta MusicEngineV3TimeToReleaseCounter

  lda MusicEngineV3Duration
  sta MusicEngineV3DurationCounter

  lda #1
  sta MusicEngineV3Active

; Process music engine voice time to release and duration.
ProcessMusicDurAndRel:
  lda MusicEngineV1TimeToReleaseCounter
  bne A_7266

  ; Disable Voice 1.
  disableVoice1 ; macro
  
  jmp A_7269
  
A_7266:
  dec MusicEngineV1TimeToReleaseCounter                ;   MusicEngineV1TimeToReleaseCounter--

A_7269:
  lda MusicEngineV1DurationCounter                     ; A=MusicEngineV1DurationCounter
  cmp #1
  beq A_7276

  dec MusicEngineV1DurationCounter                     ;   MusicEngineV1DurationCounter--
  jmp J_727E

A_7276:
  ; Disable Voice 1.
  disableVoice1 ; macro
  
  lda #0
  sta MusicEngineV1Active

J_727E:
  lda MusicEngineV2TimeToReleaseCounter
  bne A_728A

  ; Disable Voice 2.
  disableVoice2 ; macro
  
  jmp A_728D
  
A_728A:
  dec MusicEngineV2TimeToReleaseCounter

A_728D:
  lda MusicEngineV2DurationCounter
  cmp #1
  beq A_729A

  dec MusicEngineV2DurationCounter
  jmp J_72A2

A_729A:
  ; Disable Voice 2.
  disableVoice2 ; macro
  
  lda #0
  sta MusicEngineV2Active

J_72A2:
  lda MusicEngineV3TimeToReleaseCounter
  bne A_72AE

  ; Disable Voice 3.
  disableVoice3 ; macro
  
  jmp A_72B1
  
A_72AE:
  dec MusicEngineV3TimeToReleaseCounter

A_72B1:
  lda MusicEngineV3DurationCounter
  cmp #1
  beq A_72BC

  dec MusicEngineV3DurationCounter
  
  rts

A_72BC:
  ; Disable Voice 3.
  disableVoice3 ; macro
  
  lda #0
  sta MusicEngineV3Active
  
  rts
