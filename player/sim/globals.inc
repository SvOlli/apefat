
; configuration parameters

; timer values for vblank and overscan are for TIM64TI, screen is for TIM1KTI
; PAL values, determined by trial an error
.define TIMER_VBLANK   $2a ; ~2688 cycles
.define TIMER_SCREEN   $13
.define TIMER_OVERSCAN $14 ; ~1280 cycles

; all symbols visible to other source files

; 0pglobal.s
.globalzp slocumAtten
.globalzp slocumMeasure
.globalzp slocumBeat
.globalzp slocumTempoCount
.globalzp temp8
.globalzp temp16
.globalzp localramstart

; main.s
.global   reset
; each one of these can be called: lda #DELAY : jsr x + 2
.global   waitvblank   ; called with jsr; x,y untouched
.global   waitscreen   ; called with jsr; x,y untouched
.global   waitoverscan ; called with jmp; y untouched

.global   slocumPlayer

.global   song1
.global   song2
.global   patternArrayHlsb
.global   patternArrayHmsb
.global   patternArrayLlsb
.global   patternArrayLmsb
.global   patterns
.global   soundTypeArray
.global   soundAttenArray
.global   hatPattern
.global   HATSTART
.global   HATVOLUME
.global   HATPITCH
.global   HATSOUND
.global   TEMPODELAY
