
.include "vcs.inc"
.include "globals.inc"

.segment "ZEROPAGE"

frmcnt = localramstart+0

.segment "RODATA"

.segment "CODE"

payload:
   jsr psmkPlayer
   jsr waitvblank
   jsr waitscreen
   jmp waitoverscan
