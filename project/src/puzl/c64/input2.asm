; c64 input2.asm

.export _UpdateInput

.export _ControllerAxisXState
.export _ControllerAxisYState
.export _ControllerButtonState

.import _UpdateKeyCodeStates

.autoimport on
  
.importzp sp, sreg, regsave, regbank
.importzp tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
.macpack longbranch

.include "c64.asm"

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
_UpdateInput:
  ;jsr _UpdateKeyCodeStates
  jsr UpdateControllerStates

;------------------------------------------------------------------
UpdateControllerStates:
  lda CIDDRA
  pha
  
  cli

  lda #$00
  
  sta _ControllerAxisXState
  sta _ControllerAxisYState
  
  sta CIDDRA

  lda CIAPRA
  eor #$ff
  and #%00011111
  tay ; Storing into y for later.

  pla
  sta CIDDRA

  sei

  tya ; a is back to controller button state.
  
@checkPressingUp:
  lsr
  bcc @notPressingUp
  dec _ControllerAxisYState
  
@notPressingUp:
@checkPressingDown:
  lsr
  bcc @notPressingDown
  inc _ControllerAxisYState

@notPressingDown:
@checkPressingLeft:
  lsr
  bcc @notPressingLeft
  dec _ControllerAxisXState

@notPressingLeft:
@checkPressingRight:
  lsr
  bcc @notPressingRight
  inc _ControllerAxisXState

@notPressingRight:
@finish:
  sta _ControllerButtonState

  rts
