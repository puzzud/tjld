; 6502 music_engine.asm
; base implementation.

.export InitializeMusicEngine
.export _PlayAudioPattern
.export _StopAudioPattern
.export ProcessMusic

.exportzp MusicEngineVoiceStatus

.export MusicEngineVoiceMusicStart
.export MusicEngineVoiceMusicEnd

.export MusicEngineVoiceTimeToRelease

.autoimport on
  
;.importzp sp, sreg, regsave, regbank
;.importzp tmp1, tmp2, tmp3, tmp4, mePtr1, ptr2, ptr3, ptr4
;.macpack longbranch

.import _SoundKillAll

.segment "BSS"

MusicEngineVoiceMusicStart:
  .res 2 * NUMBER_OF_VOICES

MusicEngineVoiceMusicEnd:
  .res 2 * NUMBER_OF_VOICES

MusicEngineVoiceTimeToRelease:
  .res 1 * NUMBER_OF_VOICES

MusicEngineVoiceLooping:
  .res 1 * NUMBER_OF_VOICES

.segment "ZEROPAGE"

mePtr1: ; NOTE: Not using ptr1, because this is expected to run in an interrupt.
  .res 2

meTmp1: ; NOTE: Not using tmp1, because this is expected to run in an interrupt.
  .res 1

MusicEngineVoiceStatus:
  .res 1 * NUMBER_OF_VOICES

MusicEngineVoicePosition:
  .res 2 * NUMBER_OF_VOICES

MusicEngineVoiceDuration:
  .res 1 * NUMBER_OF_VOICES

MusicEngineVoiceActive:
  .res 1 * NUMBER_OF_VOICES

MusicEngineVoiceTimeToReleaseCounter:
  .res 1 * NUMBER_OF_VOICES

MusicEngineVoiceDurationCounter:
  .res 1 * NUMBER_OF_VOICES

MusicEngineTempFetch:
  .res 1

; MULE music engine.
;
; It provides the following major routines:
; _InitializeMusic
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
InitializeMusicEngine:
  ldx #NUMBER_OF_VOICES-1
@loop:
  jsr _StopAudioPattern

  dex
  bpl @loop

  rts

;---------------------------------------
; inputs:
;  - voiceIndex: sp[2], which voice to play this pattern with.
;  - voiceStart: sp[1]/sp[0], address of audio pattern data to play.
;  - looping: a, indicate whether pattern should loop.
_PlayAudioPattern:
  pha

  ; voiceIndex
  ldy #2 
  lda (sp),y
  tax ; Cache normal offset for call to StartAudioPattern.

  dey
  lda (sp),y
  pha
  dey
  lda (sp),y
  pha

  ; voiceIndex as word offset.
  txa
  asl
  tay

  pla
  sta MusicEngineVoiceMusicStart,y
  pla
  sta MusicEngineVoiceMusicStart+1,y
  
  pla
  jsr StartAudioPattern

  jmp incsp3

;---------------------------------------
; inputs:
;  - voiceIndex: x, which voice to start.
;  - looping: a, indicate whether pattern should loop.
StartAudioPattern:
  sta MusicEngineVoiceLooping,x
  
  ; Calculate these rather than setting them.
  jsr CalculateMusicVoiceEnd
  
  ; TODO: Perhaps it's possible to integrate
  ; release time better with NES voice timed envelopes?
  lda #$1e
  sta MusicEngineVoiceTimeToRelease,x
  
  ; Load music vectors into music engine voice music data vector (counters).
  txa
  asl
  tay

  lda MusicEngineVoiceMusicStart,y
  sta MusicEngineVoicePosition,y
  lda MusicEngineVoiceMusicStart+1,y
  sta MusicEngineVoicePosition+1,y

  ; Disable all voice music processing.
  lda #0
  sta MusicEngineVoiceActive,x

  tay
  dey
  tya
  sta MusicEngineVoiceStatus,x

  rts

;---------------------------------------
; inputs:
;  - voiceIndex: a, index of which voice to stop.
_StopAudioPattern:
  tax

  lda #0
  sta MusicEngineVoiceActive,x
  sta MusicEngineVoiceStatus,x
  
  ; TODO: Need to translate this function.
  ;jsr _SoundKillAll

  rts

;---------------------------------------
; CalculateMusicVoiceEnd
; Populate the corresponding MusicEngineV*MusicEnd
; pointer from its current starting point.
;
; inputs:
;  - voiceIndex: X, indicates which music voice to calculate.
;  - MusicEngineVoiceMusicStart + (2 * voiceIndex): start of music pointer should be set before calling.
;
; outputs:
;  - MusicEnd: mePtr1, 
CalculateMusicVoiceEnd:
  ; Set the correct music start pointer according to voiceIndex.
  txa
  asl
  tay

  lda MusicEngineVoiceMusicStart,y
  sta mePtr1
  lda MusicEngineVoiceMusicStart+1,y
  sta mePtr1+1

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
  sta MusicEngineVoiceMusicEnd,y
  
  lda mePtr1+1
  sta MusicEngineVoiceMusicEnd+1,y
  
  rts

;---------------------------------------
; Start of all voice/music processing.
; inputs:
;  - voiceIndex: x, which voice to operate on.
ProcessMusic:
  ldx #NUMBER_OF_VOICES-1
@loop:
  lda MusicEngineVoiceStatus,x
  beq @afterProcessVoice

  jsr ProcessVoice

@afterProcessVoice:
  dex
  bpl @loop

@done:
  rts

ProcessVoice:
  stx meTmp1 ; Cache voiceIndex.

  lda MusicEngineVoiceActive,x
  bne @processMusicDurAndRel

  jsr FetchVoiceNotes

; Process music engine voice time to release and duration.
@processMusicDurAndRel:
  lda MusicEngineVoiceTimeToReleaseCounter,x
  bne A_7266

  DisableVoice ; macro, squashes y.
  
  jmp A_7269
  
A_7266:
  dec MusicEngineVoiceTimeToReleaseCounter,x

A_7269:
  lda MusicEngineVoiceDurationCounter,x
  cmp #1
  beq A_7276

  dec MusicEngineVoiceDurationCounter,x

  rts

A_7276:
  DisableVoice ; macro, squashes y.
  
  lda #0
  sta MusicEngineVoiceActive,x
  
  rts

FetchVoiceNotes:
  lda meTmp1
  asl
  tay

  ; Check if at end of pattern.
  lda MusicEngineVoiceMusicEnd,y
  cmp MusicEngineVoicePosition,y
  bne @fetchPatternDatum

  lda MusicEngineVoiceMusicEnd+1,y
  cmp MusicEngineVoicePosition+1,y
  bne @fetchPatternDatum

  lda MusicEngineVoiceLooping,x
  bne @resetMusicVectors

  ; No looping, so do no fetching.
  sta MusicEngineVoiceActive,x ; a should be 0.
  rts

; Reset music engine vectors to currently set base music data vectors.
@resetMusicVectors:
  lda MusicEngineVoiceMusicStart,y
  sta MusicEngineVoicePosition,y
  lda MusicEngineVoiceMusicStart+1,y
  sta MusicEngineVoicePosition+1,y

  lda #0
  sta MusicEngineVoiceActive,x

; Start music data processing (of voice).
@fetchPatternDatum:
  ; Fetch current byte and cache for later analysis.
  lda MusicEngineVoicePosition,y
  sta mePtr1
  lda MusicEngineVoicePosition+1,y
  sta mePtr1+1
  
  ldy #0
  lda (mePtr1),y
  sta MusicEngineTempFetch
  
  ; Cutoff bits 6 & 7.
  ; The first six bits of this byte are the music note index.
  and #%00111111
  
  ; Load Voice Frequency.
  SetVoiceFrequency ; macro
  
  ; Now check bit 7.
  bit MusicEngineTempFetch
  bpl A_7180

  ; Fetch and store next byte.
  ldy #1
  lda (mePtr1),y
  sta MusicEngineVoiceDuration,x

  ; Increase music pointer.
  inc mePtr1
  bne A_7180
  inc mePtr1+1
A_7180:
  inc mePtr1
  bne @skip2ndIncrementCarry
  inc mePtr1+1
@skip2ndIncrementCarry:
  lda meTmp1
  asl
  tay
  lda mePtr1
  sta MusicEngineVoicePosition,y
  lda mePtr1+1
  sta MusicEngineVoicePosition+1,y

  ; Now check bit 6.
A_7186:
  bit MusicEngineTempFetch
  bvc A_7192

  DisableVoice ; macro
  
  jmp A_7197

A_7192:
  ; Gate Voice.
  EnableVoice ; macro, squashes y.
  
A_7197:
  lda MusicEngineVoiceTimeToRelease,x
  sta MusicEngineVoiceTimeToReleaseCounter,x

  lda MusicEngineVoiceDuration,x
  sta MusicEngineVoiceDurationCounter,x

  lda #1
  sta MusicEngineVoiceActive,x

  rts
