
.include "globals.inc"

.segment "ZEROPAGE"

; must be first, at $80
schedule:
   .byte 0
slocumAtten:
   .byte 0
slocumMeasure:
   .byte 0
slocumBeat:
   .byte 0
slocumTempoCount:
   .byte 0
temp8:
   .byte 0
temp16:
   .word 0

localramstart:
