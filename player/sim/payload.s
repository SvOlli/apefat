
.include "globals.inc"

frameDone      = $00 ; strobe: one pass is through
setMeasure     = $01 ; $01 != #$ff repeat this measure

.segment "CODE"
reset:
   ldx setMeasure
   cpx #$ff
   beq @skipMeasure
   stx slocumMeasure
   ldx #$ff
   stx setMeasure
   inx
   stx slocumBeat
@skipMeasure:

   jsr slocumPlayer
   lda #$00 ; just to have a defined value in ram
   sta $00
   jmp reset
