; nes input2.asm

.export InitializeInput
.export UpdateInput

.export _ControllerAxisXState
.export _ControllerAxisYState
.export _ControllerButtonState

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.include "nes.asm"

.segment "BSS"

.segment "ZEROPAGE"

_ControllerAxisXState:
.res 1

_ControllerAxisYState:
.res 1

_ControllerButtonState:
.res 1

.segment "CODE"

;------------------------------------------------------------------
InitializeInput:
  lda #0
  sta _ControllerButtonState
  sta _ControllerAxisXState
  sta _ControllerAxisYState

  rts

;------------------------------------------------------------------
UpdateInput:
  ldx #1
  stx APU_PAD1
  dex
  stx APU_PAD1

  stx _ControllerButtonState ; x = 0
  stx _ControllerAxisXState
  stx _ControllerAxisYState

  ; Get 8 button states.
  ldx #8
@gamepadButtonLoop:
  lda APU_PAD1
  lsr
  ror _ControllerButtonState
  dex
  bne @gamepadButtonLoop
  
  ; Cache all button states in x.
  lda _ControllerButtonState
  tax

  ; Shift out top nibble to store axis states.
@processAxes:

@processRight:
  rol
  bcc @processLeft
  inc _ControllerAxisXState
@processLeft:
  rol
  bcc @processDown
  dec _ControllerAxisXState
@processDown:
  rol
  bcc @processUp
  inc _ControllerAxisYState
@processUp:
  rol
  bcc @afterProcessAxes
  dec _ControllerAxisYState
@afterProcessAxes:

  ; Restore from x and save 4 lower controller button states.
  txa
  and #%00001111
  sta _ControllerButtonState

  rts
