
.include "globals.inc"
.include "vcs.inc"

.define SEPARATE_LOW_HIGH_IN_PATTERNS 1
.define SHADOW_REGISTERS 0

.define PERCCUTTER 0

.segment "RODATA"

.if 0
.include "slocumSong2.inc"
.else
.include "slocumsong.inc"
.endif

bitMaskArray:
   .byte %10000000
   .byte %01000000
   .byte %00100000
   .byte %00010000
   .byte %00001000
   .byte %00000100
   .byte %00000010
   .byte %00000001


.segment "CODE"
   
;--------------------------------------------------------------------------
; songPlayer
;--------------------------------------------------------------------------
; Plays up to two pre-programmed patterns simlutaneously.
;
; Call this once per screen-draw.
;--------------------------------------------------------------------------
slocumPlayer:
;--------------------------------------------------------------------------
; Generates tempo based on TEMPODELAY
;--------------------------------------------------------------------------
   inc slocumTempoCount
   lda slocumTempoCount
.ifconst TEMPODELAY
   eor #TEMPODELAY
.else
   eor TEMPODELAY
.endif
   bne quitTempo
   sta slocumTempoCount

   inc slocumBeat
   lda slocumBeat
   eor #32
   bne quitTempo
   sta slocumBeat

   inc slocumMeasure

quitTempo:
;--------------------------------------------------------------------------

   ; set the volume to zero
   ldx #0

   ldy slocumMeasure
   lda song1,y

   ; Check to see if the end of the song was reached
   cmp #255
   bne notEndOfSong

   ; Go back to the first measure
   stx slocumMeasure
   lda song1,x

notEndOfSong:
   jsr playPattern

   ldx slocumMeasure
   lda song2,x

   ldx #1
   ; slip through
   ; songPlayer
;--------------------------------------------------------------------------
; playPattern
;--------------------------------------------------------------------------
; Plays a pattern
;
; - ACC should contain the offset in the patternArray of the pattern to play
; - X should contain the oscillator to be used (0 or 1)
;
;--------------------------------------------------------------------------
playPattern:

   ; save unaltered patternArray offset
   sta temp16+0
; patternid = %abcdefgh
.if SEPARATE_LOW_HIGH_IN_PATTERNS
   asl
   asl
; patternid = %cdefgh00
.else
   ; save patternArray offset
   asl
   asl
   asl
; patternid = %defgh000
.endif
   sta temp8

   ; custom code to allow 1 quarter note per measure (Thrust):
   ; use beat to determine extra offset within patternArray
   lda slocumBeat
   and #%00011000
; patternid = %000ij000
.if SEPARATE_LOW_HIGH_IN_PATTERNS
   lsr
   lsr
   lsr
; patternid = %000000ij
.else
   lsr
   lsr
; patternid = %00000ij0
.endif

   ; add in original offset
.if SEPARATE_LOW_HIGH_IN_PATTERNS
   ora temp8
; patternid = %cdefghij
.else
   ora temp8
; patternid = %defghij0
.endif

   ; save osc number
   stx temp8

   tax

   ; This mod allows for high and low volume patterns.
   ; Patterns of offset greater than 128 read from a different
   ; array and play lower.
   lda temp16+0
   bmi lowPattern

   ; Loud version
   ; Get address of selected pattern
.if SEPARATE_LOW_HIGH_IN_PATTERNS
   lda patternArrayHlsb,x
   ldy patternArrayHmsb,x
.else
   lda patternArrayH,x
   ldy patternArrayH+1,x
.endif

   ; Set 0 attenuation
   ldx #0
   beq endGetPattern

lowPattern:
   ; Soft version
   ; Get address of selected pattern
.if SEPARATE_LOW_HIGH_IN_PATTERNS
   lda patternArrayLlsb,x
   ldy patternArrayLmsb,x
.else
   lda patternArrayL,x
   ldy patternArrayL+1,x
.endif

   ; Set -6 attenuation
   ldx #4

endGetPattern:
   sta temp16+0
   sty temp16+1
   stx slocumAtten

   ; The variable, beat, contains the 32nd note
   ; that the beat is currently on.
   lda slocumBeat

   ; modification for 1 quarter per measure (Thrust)
   and #%00000111
   tay

   ; Get sound/note data
   lda (temp16),y
   eor #255
   beq muteNote
   eor #255

;--------------------------------------------------------------------------
; Extract Pattern Data
;--------------------------------------------------------------------------
; Each byte of pattern data contains the frequency and
; sound type data.  This function separates and decodes them.
;
; The encoding is: the 3 high bits contain the encoded sound
; type and the lower 5 bits contain the freq data.
;
; - ACC must contain pattern byte
;
; = ACC will return the freq
; = X will return the sound type
;
; changes ACC,X
;--------------------------------------------------------------------------
   tax

   ; Extract freq data and push it
   and #%00011111
   pha

   txa
   lsr
   lsr
   lsr
   lsr
   lsr
   tax

;-----------------------
   lda slocumAtten
   clc
   adc soundAttenArray,x
   sta slocumAtten
;-----------------------

   lda soundTypeArray,x
;--------------------------------------------------------------------------

   ; Get the osc number again
   ldx temp8


noPhase:
   sta AUDC0,x
.if SHADOW_REGISTERS
   sta audc0,x
.endif
   pla
   sta AUDF0,x
.if SHADOW_REGISTERS
   sta audf0,x
.endif


   ; restore beat & #%111
   tya
   tax

;--------------------------------------------------------------------------
; Accent Reader
;--------------------------------------------------------------------------
; Each set of pattern data is followed by 4 accept bytes.
; Each bit in order represents the accent (on or off)
; of its corresponding 32nd note.  This function
; returns the attenuation of a note in a pattern.
;
; - temp16 must contain an indirect pointer to the pattern data
; - X must contain the beat && %00000111
;
; = will return the volume in ACC
;
; changes X,Y,ACC
;--------------------------------------------------------------------------
   ; Accent offset is always 8 for Thrust mod
   ldy #8

   lda (temp16),y
   and bitMaskArray,x
   beq noAccent

   ; It's an Accent, so don't attenuate
   lda #15

noAccent:
   ; No accent, so use a lower volume
   ora #13
;--------------------------------------------------------------------------

   sbc slocumAtten               ; carry flag???
muteNote:
   ldy temp8               ; Get the osc number again
   sta AUDV0,y
.if SHADOW_REGISTERS
   sta audv0,y
.endif


   ;--------------------------------------------------------------------------
   ; Super High Hat (TM)
   ;--------------------------------------------------------------------------
   ; This plays the high hat sound on the first frame of each beat indicated
   ; in hatPattern
   ;--------------------------------------------------------------------------
   ldy temp8
   beq noHat

   ; Reat high hat pattern
   lda slocumMeasure
.ifconst HATSTART
   cmp #HATSTART
.else
   cmp HATSTART
.endif
   bmi noHat

   lda slocumBeat
   and #%00000111
   tax
   lda slocumBeat
   lsr
   lsr
   lsr
   tay
   lda hatPattern,y
   and bitMaskArray,x
   beq noHat

   ; Only play had on first frame
   lda slocumTempoCount
   bne noHat

   ; Play hat
.ifconst HATPITCH
   lda #HATPITCH
.else
   lda HATPITCH
.endif
   sta AUDF1
.if SHADOW_REGISTERS
   sta audf0+1
.endif
.ifconst HATSOUND
   lda #HATSOUND
.else
   lda HATSOUND
.endif
   sta AUDC1
.if SHADOW_REGISTERS
   sta audc0+1
.endif
.ifconst HATVOLUME
   lda #HATVOLUME
.else
   lda HATVOLUME
.endif
   sta AUDV1
.if SHADOW_REGISTERS
   sta audv0+1
.endif
noHat:
   ;--------------------------------------------------------------------------

   ;--------------------------------------------------------------------------
   ; Percussion cutter
   ;--------------------------------------------------------------------------
   ; This code cuts off the sound for better percussive sounds.  You
   ; can set it to start working at a certain measure.
   ;--------------------------------------------------------------------------
.if PERCCUTTER
   lda slocumMeasure
   cmp #111   ; start measure
   bmi noCut

   lda slocumTempoCount
   and #%11111110
   beq noCut

   lda #0
   sta AUDV0
.if SHADOW_REGISTERS
   sta audv0
.endif
.endif

   rts ; playPattern OTHER RTS IN FUNCTION
