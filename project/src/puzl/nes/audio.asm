; c64 audio.asm

.import InitializeMusicEngine

.export InitializeAudio

.export _SoundKillAll

;.exportzp VoiceControlCache
;.export VoiceRegisterSidOffset

.autoimport on
  
; .importzp sp, sreg, regsave, regbank
; .importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.include "nes.asm"

.segment "BSS"

.segment "ZEROPAGE"

.segment "RODATA"

.segment "CODE"

;------------------------------------------------------------------
InitializeAudio:
  jsr InitializeVoices
  jsr InitializeMusicEngine

  rts

;------------------------------------------------------------------
InitializeVoices:
  rts
  
;------------------------------------------------------------------
_SoundKillAll:
  rts
