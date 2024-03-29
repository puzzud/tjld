.import InitializeVideo
.import InitializeInput
.import InitializeAudio

.import UpdateInput

.import _UpdatePalette

.import ProcessSequences

.import _InitializeNodeTree
.import _Process

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.importzp sp
.import __BSS_START__, __BSS_LAST__, __BSS_SIZE__
.import __STACKSIZE__
.import __RAM_START__,__RAM_SIZE__
.import __WRAM_START__,__WRAM_SIZE__

.export Reset

.exportzp _UpdatePaletteFlag
.exportzp _Running

.export _NthBitFlags
.export _InverseNthBitFlags

.include "nes.asm"

.segment "ZEROPAGE"

NmiStatus:
  .res 1

_Running:
  .res 1

_UpdatePaletteFlag:
  .res 1

.segment "BSS"

.segment "RODATA"

; TODO: Probably don't need.
_NthBitFlags:
  .byte $01
  .byte $02
  .byte $04
  .byte $08
  .byte $10
  .byte $20
  .byte $40
  .byte $80

_InverseNthBitFlags:
  .byte !$01
  .byte !$02
  .byte !$04
  .byte !$08
  .byte !$10
  .byte !$20
  .byte !$40
  .byte !$80

.segment "CODE"

Reset:
  sei ; Disable IRQs.
  cld ; Disable decimal mode.
    
  lda #0
  tax
@clearMemory:
  sta $00,x ;7
  sta $0100,x ;28
  sta $0200,x
  sta $0300,x
  sta $0400,x
  sta $0500,x
  sta $0600,x
  sta $0700,x
  inx ;5
  bne @clearMemory
  
  dex ; x = 0xff
  txs ; Set up CPU stack.

  ; Set parameter stack pointer.
  lda #<(__WRAM_START__+__WRAM_SIZE__)
  sta sp
  lda #>(__WRAM_START__+__WRAM_SIZE__)
  sta sp+1

  jsr InitializeSequencer

  jsr InitializeVideo
  jsr InitializeInput
  jsr InitializeAudio

  jsr _InitializeNodeTree
  
  ; Enable NMI and various video system values.
  lda #%10010000
  sta PPU_CTRL1

  lda #%00011110
  sta PPU_CTRL2

  ;cli
  
@mainLoop:
  lda NmiStatus
  beq @mainLoop
  
  lda #0
  sta NmiStatus
  
  jsr UpdateInput
  jsr _Process
  
  sta _UpdatePaletteFlag
  
  ;lda #%00000000
  ;sta PPU_CTRL1
  
  ;lda #%10000000
  ;sta PPU_CTRL1

  jmp @mainLoop

;==================================================================

;------------------------------------------------------------------
DefaultInterrupt:
  php
  pha
  txa
  pha
  tya
  pha

  ; DMA sprite shadow RAM into OAM.
  lda #$00
  sta PPU_SPR_ADDR
  lda #$02
  sta APU_SPR_DMA

  ; Check if sprite data needs to be copied.
  lda _UpdatePaletteFlag
  beq @afterUpdatePalette
  
  lda #0
  jsr _UpdatePalette
@afterUpdatePalette:

  ; NOTE: Music processing should occur at the end,
  ; but sprite animation should occur before OAM DMA,
  ; to ensure no frame delay between animation frame
  ; change and the DMA that updates hardware sprites.
  ; TODO: Probably need to create an alternative
  ; ProcessSequences that will process music and sprite
  ; animation separately.
  jsr ProcessSequences

  lda #1
  sta NmiStatus
            
  pla
  tay
  pla
  tax
  pla
  plp

EmptyInterrupt:
  rti

.segment "VECTORS"
  .word DefaultInterrupt
  .word Reset
  .word EmptyInterrupt
