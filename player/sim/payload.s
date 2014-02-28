
.include "globals.inc"

frameDone      = $00 ; strobe: one pass is through
fixedMeasure   = $01 ; $01 != #$ff repeat this measure
fixedBeatStart = $02 ; \ if $03 < #$20 ->
fixedBeatEnd   = $03 ; / if beat > $03 beat = $02

.segment "CODE"
reset:
   ldx fixedMeasure
   cpx #$ff
   beq @skipMeasure
   stx slocumMeasure
@skipMeasure:

   ldx slocumBeat
   dex
   cpx fixedBeatEnd
   bcc @skipBeat
   ldx fixedBeatStart
   dex
   stx slocumBeat
@skipBeat:

   jsr slocumPlayer
   sta $00
   jmp reset
