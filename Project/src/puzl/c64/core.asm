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
.import __BSS_START__, __BSS_SIZE__

.export Reset

.exportzp _Running
.exportzp _InitScreen
.exportzp _CurrentScreenInit
.exportzp _CurrentScreenUpdate

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

Reset:
  sei

  lda #%01111111
  sta CIA1_ICR
  sta CIA2_ICR

  lda CIA1_ICR
  lda CIA2_ICR

  lda VIC_IMR
  ora #%00000001
  sta VIC_IMR

  lda #248
  sta VIC_HLINE

  lda VIC_CTRL1
  and #%01111111
  sta VIC_CTRL1

  ; Set up interrupt routine.
  lda #<DefaultInterrupt
  sta $fffa
  sta $fffc
  sta $fffe
  lda #>DefaultInterrupt
  sta $fffb
  sta $fffd
  sta $ffff

  ; Set parameter stack pointer.
  lda #<(__BSS_START__+__BSS_SIZE__)
  sta sp
  lda #>(__BSS_START__+__BSS_SIZE__)
  sta sp+1
  
  lda #%00110101
  sta LORAM

  jsr _InitializeVideo
  jsr _InitializeInput
  ;jsr _InitializeAudio

  cli
  
  jsr _InitalizeSpeed
  jsr _InitializeNodeTree
  
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
  lda VIC_HLINE
  cmp #248
  beq @waitFrame

  ; Wait for the raster to reach line $f8
  ; (should be closer to the start of this line this way).
@waitStep2:
  lda VIC_HLINE
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
  
  ;lda _MusicStatus
  ;beq @endProcessMusic
  ;jsr _ProcessMusic
;@endProcessMusic:

  lda #$ff
  sta VIC_IRR
            
  pla
  tay
  pla
  tax
  pla

EmptyInterrupt:
  rti
