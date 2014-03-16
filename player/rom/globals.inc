
; configuration parameters

; timer values for vblank and overscan are for TIM64TI, screen is for TIM1KTI
; PAL values, determined by trial an error
.define TIMER_VBLANK   $2a ; ~2688 cycles
.define TIMER_SCREEN   $13
.define TIMER_OVERSCAN $14 ; ~1280 cycles

; all symbols visible to other source files

; 0pglobal.s
.globalzp schedule
.globalzp psmkAttenuation
.globalzp psmkPatternIdx
.globalzp psmkBeatIdx
.globalzp psmkTempoCount
.globalzp temp8
.globalzp temp16
.globalzp localramstart

; main.s
.global   reset
; each one of these can be called: lda #DELAY : jsr x + 2
.global   waitvblank   ; called with jsr; x,y untouched
.global   waitscreen   ; called with jsr; x,y untouched
.global   waitoverscan ; called with jmp; y untouched

; payload.s
.global   payload

; slocumplayer.s
.global   psmkPlayer

.linecont +
   .define partsaddrlist \
   payload-1
.linecont -

