; c64 audio.asm

.import InitializeMusicEngine
.import ProcessMusic

.export InitializeAudio
.export UpdateAudio

.export _SoundKillAll

.exportzp Voice1ControlCache
.exportzp Voice2ControlCache
.exportzp Voice3ControlCache

.autoimport on
  
; .importzp sp, sreg, regsave, regbank
; .importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.include "c64.asm"

.segment "BSS"

.segment "ZEROPAGE"

; Caching of gating / waveform type SID registers for each voice,
; because reading from them is apparently not possible.
Voice1ControlCache:
  .res 1
  
Voice2ControlCache:
  .res 1
  
Voice3ControlCache:
  .res 1

.segment "CODE"

;------------------------------------------------------------------
InitializeAudio:
  jsr InitializeVoices
  jsr InitializeMusicEngine

  rts

;------------------------------------------------------------------
UpdateAudio:
  jsr ProcessMusic

;------------------------------------------------------------------
InitializeVoices:
  ; Set Voice1/2/3 Waveform (high nibble) to 8.
  lda #$08
  sta PWHI1
  sta PWHI2
  sta PWHI3
  
  ; Set Voice1/2/3 decay 2ms / attack 250ms.
  lda #$09
  sta ATDCY1
  sta ATDCY2
  sta ATDCY3
  
  ; Set Voice1/2/3 sustain 114ms / release 6ms.
  lda #$40
  sta SUREL1
  sta SUREL2
  sta SUREL3
  
  ; Set Voice1/2/3 Waveform (low byte) to 0.
  lda #0
  sta PWLO1
  sta PWLO2
  sta PWLO3

  ; Gate all Voices silent.
  ; Also, set their waveforms.
  lda #%01000000 ; pulse
  
  sta VCREG1
  sta Voice1ControlCache
  
  sta VCREG2
  sta Voice2ControlCache
  
  lda #%00010000 ; triangle
  sta VCREG3
  sta Voice3ControlCache

  ; Set volume 15 (max).
  lda #15
  sta SIGVOL

  rts
  
;------------------------------------------------------------------
_SoundKillAll:
  lda Voice1ControlCache
  and #%11111110
  sta VCREG1
  sta Voice1ControlCache
  
  lda Voice2ControlCache
  and #%11111110
  sta VCREG2
  sta Voice2ControlCache
  
  lda Voice3ControlCache
  and #%11111110
  sta VCREG3
  sta Voice3ControlCache
  
  rts
