; nes input2.asm

.export _UpdateInput

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
_UpdateInput:
  jsr UpdateControllerStates

;------------------------------------------------------------------
UpdateControllerStates:
  rts
