;.import _Init

;.import _UpdateInput
.import _InitializeVideo
.import _InitializeInput

.import _UpdateKeyCodeStates

.import _InitalizeSpeed
.import _InitializeNodeTree
.import _Process
.import _CycleSpeedBit

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.importzp sp
.import __BSS_START__, __BSS_LAST__, __BSS_SIZE__
.import __STACKSIZE__

.export Reset

.exportzp _Running
.exportzp _InitScreen
.exportzp _CurrentScreenInit
.exportzp _CurrentScreenUpdate

.export _NthBitFlag

.include "c64.asm"

.segment "ZEROPAGE"

_Running:
  .res 1

_InitScreen:
  .res 1
  
_CurrentScreenInit:
  .res 2
  
_CurrentScreenUpdate:
  .res 2

.segment "BSS"

.segment "CODE"

_NthBitFlag:
  .byte $01
  .byte $02
  .byte $04
  .byte $08
  .byte $10
  .byte $20
  .byte $40
  .byte $80

Reset:
  sei

  lda #%01111111
  sta CIAICR
  sta CI2ICR

  lda CIAICR
  lda CI2ICR

  lda #%00000000
  sta IRQMASK

  lda #248
  sta RASTER

  lda SCROLY
  and #%01111111
  sta SCROLY

  ; Set up interrupt routine.
  lda #<DefaultInterrupt
  sta $fffa
  sta $fffc
  sta $fffe
  lda #>DefaultInterrupt
  sta $fffb
  sta $fffd
  sta $ffff
  
  ; Swap out Kernal & BASIC ROMs for RAM.
  ; Keep IO.
  lda #%00110101
  sta R6510

  ; Set parameter stack pointer.
  lda #<(__BSS_LAST__+__STACKSIZE__-1)
  sta sp
  lda #>(__BSS_LAST__+__STACKSIZE__-1)
  sta sp+1

  jsr _InitializeVideo
  jsr _InitializeInput
  ;jsr _InitializeAudio
  
  jsr _InitalizeSpeed
  jsr _InitializeNodeTree

  cli
  
@mainLoop:
  ;jsr _UpdateInput
  jsr _UpdateKeyCodeStates
  
  ;lda #>(@endUpdate-1)
  ;pha
  ;lda #<(@endUpdate-1)
  ;pha
  ;jmp (_CurrentScreenUpdate)
  jsr _Process
  jsr _CycleSpeedBit
@endUpdate:
  
  ; Wait for the raster to reach line $f8 (248)
  ; This loop is keeping timing stable.
  
  ; Is the raster line $f8?
  ; If so, wait for the next full screen,
  ; preventing mistimings if called too fast.
@waitFrame:
  lda RASTER
  cmp #248
  beq @waitFrame

  ; Wait for the raster to reach line $f8
  ; (should be closer to the start of this line this way).
@waitStep2:
  lda RASTER
  cmp #248
  bne @waitStep2
  
  jmp @mainLoop

;==================================================================

;------------------------------------------------------------------
DefaultInterrupt:
  pha
  txa
  pha
  tya
  pha
  
  lda #$ff
  sta VICIRQ
            
  pla
  tay
  pla
  tax
  pla

EmptyInterrupt:
  rti
