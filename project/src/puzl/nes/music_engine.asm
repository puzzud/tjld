; nes music_engine.asm

.autoimport on

.importzp VoiceControlCaches

.import VoiceApuControlRegisterOffset
.import VoiceApuLoFrequencyRegisterOffset

.include "nes.asm"

; NOTE: Platform specific (NES).
NOTE_FREQ_1_C  = 3419 ; C-1
NOTE_FREQ_1_CS = 3227 ; C#-1
NOTE_FREQ_1_D  = 3046 ; D-1
NOTE_FREQ_1_DS = 2875 ; D#-1
NOTE_FREQ_1_E  = 2714 ; E-1
NOTE_FREQ_1_F  = 2561 ; F-1
NOTE_FREQ_1_FS = 2417 ; F#-1
NOTE_FREQ_1_G  = 2281 ; G-1
NOTE_FREQ_1_GS = 2153 ; G#-1
NOTE_FREQ_1_A  = 2032 ; A-1
NOTE_FREQ_1_AS = 1918 ; A#-1
NOTE_FREQ_1_B  = 1810 ; B-1
NOTE_FREQ_2_C  = 1709 ; C-2
NOTE_FREQ_2_CS = 1613 ; C#-2
NOTE_FREQ_2_D  = 1522 ; D-2
NOTE_FREQ_2_DS = 1437 ; D#-2
NOTE_FREQ_2_E  = 1356 ; E-2
NOTE_FREQ_2_F  = 1280 ; F-2
NOTE_FREQ_2_FS = 1208 ; F#-2
NOTE_FREQ_2_G  = 1140 ; G-2
NOTE_FREQ_2_GS = 1076 ; G#-2
NOTE_FREQ_2_A  = 1015 ; A-2
NOTE_FREQ_2_AS = 958  ; A#-2
NOTE_FREQ_2_B  = 904  ; B-2
NOTE_FREQ_3_C  = 854  ; C-3
NOTE_FREQ_3_CS = 806  ; C#-3
NOTE_FREQ_3_D  = 760  ; D-3
NOTE_FREQ_3_DS = 718  ; D#-3
NOTE_FREQ_3_E  = 677  ; E-3
NOTE_FREQ_3_F  = 639  ; F-3
NOTE_FREQ_3_FS = 603  ; F#-3
NOTE_FREQ_3_G  = 569  ; G-3
NOTE_FREQ_3_GS = 537  ; G#-3
NOTE_FREQ_3_A  = 507  ; A-3
NOTE_FREQ_3_AS = 478  ; A#-3
NOTE_FREQ_3_B  = 451  ; B-3
NOTE_FREQ_4_C  = 426  ; C-4
NOTE_FREQ_4_CS = 402  ; C#-4
NOTE_FREQ_4_D  = 379  ; D-4
NOTE_FREQ_4_DS = 358  ; D#-4
NOTE_FREQ_4_E  = 338  ; E-4
NOTE_FREQ_4_F  = 319  ; F-4
NOTE_FREQ_4_FS = 301  ; F#-4
NOTE_FREQ_4_G  = 284  ; G-4
NOTE_FREQ_4_GS = 268  ; G#-4
NOTE_FREQ_4_A  = 253  ; A-4
NOTE_FREQ_4_AS = 238  ; A#-4
NOTE_FREQ_4_B  = 225  ; B-4
NOTE_FREQ_5_C  = 212  ; C-5
NOTE_FREQ_5_CS = 200  ; C#-5
NOTE_FREQ_5_D  = 189  ; D-5
NOTE_FREQ_5_DS = 178  ; D#-5
NOTE_FREQ_5_E  = 168  ; E-5
NOTE_FREQ_5_F  = 159  ; F-5
NOTE_FREQ_5_FS = 150  ; F#-5
NOTE_FREQ_5_G  = 141  ; G-5
NOTE_FREQ_5_GS = 133  ; G#-5
NOTE_FREQ_5_A  = 126  ; A-5
NOTE_FREQ_5_AS = 118  ; A#-5
NOTE_FREQ_5_B  = 112  ; B-5
NOTE_FREQ_6_C  = 105  ; C-6
NOTE_FREQ_6_CS = 99   ; C#-6
NOTE_FREQ_6_D  = 94   ; D-6
NOTE_FREQ_6_DS = 88   ; D#-6
NOTE_FREQ_6_E  = 83   ; E-6
NOTE_FREQ_6_F  = 79   ; F-6
NOTE_FREQ_6_FS = 74   ; F#-6
NOTE_FREQ_6_G  = 70   ; G-6
NOTE_FREQ_6_GS = 66   ; G#-6
NOTE_FREQ_6_A  = 62   ; A-6
NOTE_FREQ_6_AS = 58   ; A#-6
NOTE_FREQ_6_B  = 55   ; B-6

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - noteIndex: a, indicates an entry from the note frequency table.
;  - voiceIndex: x, which voice to change note frequency (preserved).
; notes:
;  - Squashes a, y.
.macro SetVoiceFrequency
	.local @afterOctaveShift
	cpx #2 ; Is voice the triangle voice?
	bne @afterOctaveShift

	; Triangle voice needs to have its octave shifted up one.
	clc
	adc #12
@afterOctaveShift:

	tay

  txa ; Cache x on stack.
  pha

	lda VoiceApuLoFrequencyRegisterOffset,x
	tax
	lda MusicEngineNoteFreqTableLo1C,y
	sta APU_PULSE1CTRL,x ; NOTE: Using offsets from base APU address.
	lda MusicEngineNoteFreqTableHi1C,y
	sta APU_PULSE1CTRL+1,x ; NOTE: Hi register is one after Lo.

	pla ; Restore x.
  tax
.endmacro

;------------------------------------------------------------------
; inputs:
;  - voiceIndex: x, which voice to change note frequency (preserved).
; notes:
;  - squashes a, y.
.macro DisableVoice
	.local @disablePulseVoice
	.local @disableTriangleVoice
	.local @done
	cpx #2 ; Is voice the triangle voice?
	bne @disablePulseVoice

@disableTriangleVoice:
	lda VoiceControlCaches,x
  and #%10000000
  sta APU_TRICTRL1

	jmp @done

@disablePulseVoice:
	lda VoiceApuControlRegisterOffset,x
	tay

	lda VoiceControlCaches,x
  and #%11110000
  sta APU_PULSE1CTRL,y ; NOTE: Using offsets from base APU address.

@done:
	sta VoiceControlCaches,x
.endmacro

;------------------------------------------------------------------
; inputs:
;  - voiceIndex: x, which voice to change note frequency (preserved).
; notes:
;  - squashes a, y.
.macro EnableVoice
	.local @enablePulseVoice
	.local @enableTriangleVoice
	.local @done
	cpx #2 ; Is voice the triangle voice?
	bne @enablePulseVoice

@enableTriangleVoice:
	lda VoiceControlCaches,x
  ora #%10000001
  sta APU_TRICTRL1

	bne @done ; NOTE: Assume not zero from ora above.

@enablePulseVoice:
	lda VoiceApuControlRegisterOffset,x
	tay

	lda VoiceControlCaches,x
  ora #%00001111
  sta APU_PULSE1CTRL,y ; NOTE: Using offsets from base APU address.

@done:
	sta VoiceControlCaches,x
.endmacro

.include "../6502/music_engine.asm"
