
.include "globals.inc"

.segment "VECTORS"
; nmi removed, because it will never happen on an 6507
.addr reset ; RESET
.addr reset ; IRQ: will be triggered every frame by emulator

