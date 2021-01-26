.import _InitializeVideo
.import _InitializeInput
.import InitializeAudio

.import _UpdateInput

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
  
  inx ; x = 0
    
@waitSync1:
  bit PPU_STATUS
@waitSync1Loop:
  bit PPU_STATUS
  bpl @waitSync1Loop

@clearPalette:
  ldy #$3f
  sty PPU_VRAM_ADDR2
  stx PPU_VRAM_ADDR2
  
  lda #$0f
  ldx #32
@clearPaletteLoop:
  sta PPU_VRAM_IO
  dex
  bne @clearPaletteLoop

@clearVram:
  txa
  ldy #$20
  sty PPU_VRAM_ADDR2
  sta PPU_VRAM_ADDR2
  
  ldy #16
@clearVramLoop:
  sta PPU_VRAM_IO
  inx
  bne @clearVramLoop
  dey
  bne @clearVramLoop

  ; Set parameter stack pointer.
  lda #<(__RAM_START__+__RAM_SIZE__)
  sta sp
  lda #>(__RAM_START__+__RAM_SIZE__)
  sta sp+1

@waitSync2:
  bit PPU_STATUS
@waitSync2Loop:
  bit PPU_STATUS
  bpl @waitSync2Loop

  lda #0
  sta PPU_VRAM_ADDR1
  sta PPU_VRAM_ADDR1

  ;jsr _InitializeInput
  ;jsr InitializeAudio

  jsr _InitializeNodeTree
  
  ; Enable NMI.
  lda #%10010000
  sta PPU_CTRL1

  lda #$1e
  sta PPU_CTRL2

  ;cli
  
@mainLoop:
  lda NmiStatus
  beq @mainLoop
  
  lda #0
  sta NmiStatus
  
  ;jsr _UpdateInput
  jsr _Process

  lda #1
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

  ; Update video.
  lda _UpdatePaletteFlag
  beq @afterUpdatePalette
  
  lda #0
  jsr _UpdatePalette
@afterUpdatePalette:

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
