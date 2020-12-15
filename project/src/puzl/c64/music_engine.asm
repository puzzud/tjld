; c64 music_engine.asm

.autoimport on

.importzp VoiceControlCache
.import VoiceRegisterSidOffset

.include "c64.asm"

; NOTE: Platform specific (C64).
NOTE_FREQ_1_C  =   536 ; C-1
NOTE_FREQ_1_CS =   568 ; C#-1
NOTE_FREQ_1_D  =   602 ; D-1
NOTE_FREQ_1_DS =   637 ; D#-1
NOTE_FREQ_1_E  =   675 ; E-1
NOTE_FREQ_1_F  =   716 ; F-1
NOTE_FREQ_1_FS =   758 ; F#-1
NOTE_FREQ_1_G  =   803 ; G-1
NOTE_FREQ_1_GS =   851 ; G#-1
NOTE_FREQ_1_A  =   902 ; A-1
NOTE_FREQ_1_AS =   955 ; A#-1
NOTE_FREQ_1_B  =  1012 ; B-1
NOTE_FREQ_2_C  =  1072 ; C-2
NOTE_FREQ_2_CS =  1136 ; C#-2
NOTE_FREQ_2_D  =  1204 ; D-2
NOTE_FREQ_2_DS =  1275 ; D#-2
NOTE_FREQ_2_E  =  1351 ; E-2
NOTE_FREQ_2_F  =  1432 ; F-2
NOTE_FREQ_2_FS =  1517 ; F#-2
NOTE_FREQ_2_G  =  1607 ; G-2
NOTE_FREQ_2_GS =  1703 ; G#-2
NOTE_FREQ_2_A  =  1804 ; A-2
NOTE_FREQ_2_AS =  1911 ; A#-2
NOTE_FREQ_2_B  =  2025 ; B-2
NOTE_FREQ_3_C  =  2145 ; C-3
NOTE_FREQ_3_CS =  2273 ; C#-3
NOTE_FREQ_3_D  =  2408 ; D-3
NOTE_FREQ_3_DS =  2551 ; D#-3
NOTE_FREQ_3_E  =  2703 ; E-3
NOTE_FREQ_3_F  =  2864 ; F-3
NOTE_FREQ_3_FS =  3034 ; F#-3
NOTE_FREQ_3_G  =  3215 ; G-3
NOTE_FREQ_3_GS =  3406 ; G#-3
NOTE_FREQ_3_A  =  3608 ; A-3
NOTE_FREQ_3_AS =  3823 ; A#-3
NOTE_FREQ_3_B  =  4050 ; B-3
NOTE_FREQ_4_C  =  4291 ; C-4
NOTE_FREQ_4_CS =  4547 ; C#-4
NOTE_FREQ_4_D  =  4817 ; D-4
NOTE_FREQ_4_DS =  5103 ; D#-4
NOTE_FREQ_4_E  =  5407 ; E-4
NOTE_FREQ_4_F  =  5728 ; F-4
NOTE_FREQ_4_FS =  6069 ; F#-4
NOTE_FREQ_4_G  =  6430 ; G-4
NOTE_FREQ_4_GS =  6812 ; G#-4
NOTE_FREQ_4_A  =  7217 ; A-4
NOTE_FREQ_4_AS =  7647 ; A#-4
NOTE_FREQ_4_B  =  8101 ; B-4
NOTE_FREQ_5_C  =  8583 ; C-5
NOTE_FREQ_5_CS =  9094 ; C#-5
NOTE_FREQ_5_D  =  9634 ; D-5
NOTE_FREQ_5_DS = 10207 ; D#-5
NOTE_FREQ_5_E  = 10814 ; E-5
NOTE_FREQ_5_F  = 11457 ; F-5
NOTE_FREQ_5_FS = 12139 ; F#-5
NOTE_FREQ_5_G  = 12860 ; G-5
NOTE_FREQ_5_GS = 13625 ; G#-5
NOTE_FREQ_5_A  = 14435 ; A-5
NOTE_FREQ_5_AS = 15294 ; A#-5
NOTE_FREQ_5_B  = 16203 ; B-5
NOTE_FREQ_6_C  = 17167 ; C-6
NOTE_FREQ_6_CS = 18188 ; C#-6
NOTE_FREQ_6_D  = 19269 ; D-6
NOTE_FREQ_6_DS = 20415 ; D#-6
NOTE_FREQ_6_E  = 21629 ; E-6
NOTE_FREQ_6_F  = 22915 ; F-6
NOTE_FREQ_6_FS = 24278 ; F#-6
NOTE_FREQ_6_G  = 25721 ; G-6
NOTE_FREQ_6_GS = 27251 ; G#-6
NOTE_FREQ_6_A  = 28871 ; A-6
NOTE_FREQ_6_AS = 30588 ; A#-6
NOTE_FREQ_6_B  = 32407 ; B-6

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - noteIndex: a, indicates an entry from the note frequency table.
;  - voiceIndex: x, which voice to change note frequency (preserved).
; notes:
;  - Squashes a, y.
.macro SetVoiceFrequency
  ; Macro for voice frequency load.
  tay
  
  txa ; Cache x on stack.
  pha

  lda VoiceRegisterSidOffset,x
  tax

  lda MusicEngineNoteFreqTableLo1C,y
  sta FRELO1,x
  lda MusicEngineNoteFreqTableHi1C,y
  sta FREHI1,x

  pla ; Restore x.
  tax
.endmacro

;------------------------------------------------------------------
; inputs:
;  - voiceIndex: x, which voice to change note frequency (preserved).
; notes:
;  - squashes a, y.
.macro DisableVoice
  ; Disable Voice.
  lda VoiceRegisterSidOffset,x
  tay

  lda VoiceControlCache,x
  and #%11111110
  sta VoiceControlCache,x
  sta VCREG1,y
.endmacro

;------------------------------------------------------------------
; inputs:
;  - voiceIndex: x, which voice to change note frequency (preserved).
; notes:
;  - squashes a, y.
.macro EnableVoice
  ; Gate Voice.
  lda VoiceRegisterSidOffset,x
  tay

  lda VoiceControlCache,x
  ora #%00000001
  sta VoiceControlCache,x
  sta VCREG1,y
.endmacro

.include "../6502/music_engine.asm"
