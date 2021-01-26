; c64 audio.asm

.import InitializeMusicEngine

.export InitializeAudio

.export _SoundKillAll

.exportzp VoiceControlCaches

.export VoiceApuControlRegisterOffset
.export VoiceApuLoFrequencyRegisterOffset

.autoimport on
  
; .importzp sp, sreg, regsave, regbank
; .importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.include "nes.asm"

.segment "BSS"

.segment "ZEROPAGE"

; NES specific APU register caches
; (since they cannot be read?).
VoiceControlCaches:
  .res 3 ; TODO: Number of engine voices (for now).

.segment "RODATA"

VoiceApuControlRegisterOffset:
  .byte (APU_PULSE1CTRL-APU_PULSE1CTRL)
  .byte (APU_PULSE2CTRL-APU_PULSE1CTRL)

VoiceApuLoFrequencyRegisterOffset:
  .byte (APU_PULSE1FTUNE-APU_PULSE1CTRL)
  .byte (APU_PULSE2FTUNE-APU_PULSE1CTRL)
  .byte (APU_TRIFREQ1-APU_PULSE1CTRL)

.segment "CODE"

;------------------------------------------------------------------
InitializeAudio:
  lda #%01111111
  sta APU_CHANCTRL

  jsr InitializeVoices
  jsr InitializeMusicEngine

  rts

;------------------------------------------------------------------
InitializeVoices:
  ; Enable all voices.
  lda #%00111111
  sta APU_CHANCTRL

  ; Disable sweep.
  lda #%00110000
  sta APU_PULSE1RAMP
  sta APU_PULSE2RAMP
  
  ; 50% duty, envelope and length counter off. No volume.
  lda #%11110000
  sta APU_PULSE1CTRL
  sta APU_PULSE2CTRL
  sta VoiceControlCaches+0
  sta VoiceControlCaches+1
  
  ; Length counter and linear control off. No sound.
  lda #%10000000
  sta APU_TRICTRL1
  sta VoiceControlCaches+2

  rts
  
;------------------------------------------------------------------
_SoundKillAll:
  lda VoiceControlCaches+0
  and #%00001111
  sta APU_PULSE1CTRL
  sta VoiceControlCaches+0
  
  lda VoiceControlCaches+1
  and #%00001111
  sta APU_PULSE2CTRL
  sta VoiceControlCaches+1
  
  lda VoiceControlCaches+2
  and #%00000000
  sta APU_TRICTRL1
  sta VoiceControlCaches+2

  rts
