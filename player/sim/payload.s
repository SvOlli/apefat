
.include "globals.inc"

frameDone      = $00 ; strobe: one pass is through

.segment "CODE"
reset:
   jsr psmkPlayer
   lda #$00 ; just to have a defined value in ram
   sta $00
   beq reset
