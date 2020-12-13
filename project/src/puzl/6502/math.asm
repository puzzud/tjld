; 6502 math.asm

.autoimport on
  
.exportzp mathOperandLo1, mathOperandHi1
.exportzp mathOperandLo2, mathOperandHi2
.exportzp mathTmp1, mathTmp2
.export AddSignedByteToSignedWord

.segment "ZEROPAGE"

mathOperandLo1:
  .res 1

mathOperandHi1:
  .res 1

mathOperandLo2:
  .res 1

mathOperandHi2:
  .res 1

mathTmp1:
  .res 1

mathTmp2:
  .res 1

.segment "CODE"

;------------------------------------------------------------------
; inputs:
;  - baseOperand: mathOperandHi1/mathOperandLo1, word sized signed number to add to.
;  - addOperand: mathOperandLo2, byte sized signed number to add to other operand.
; outputs:
;  - return: x/a, resulting signed word.
; notes:
;  - Squashes a, x, mathOperandHi2
AddSignedByteToSignedWord:
  lda #0
  sta mathOperandHi2 ; Initial velocity high byte.
  
  lda mathOperandLo2
  beq @addZero
  bpl @afterNegative
  dec mathOperandHi2 ; Make high byte negative.

@afterNegative:
  clc
  adc mathOperandLo1
  sta mathOperandLo1
  pha
  lda mathOperandHi2
  adc mathOperandHi1
  
  ; Set return values.
  tax
  pla

  rts

@addZero:
  ; Set return values.
  lda mathOperandLo1
  ldx mathOperandHi1

  rts
