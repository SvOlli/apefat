
.include "globals.inc"
.include "vcs.inc"

; changes compared to Paul Slocum Music Kit 2.0
; - ported from dasm to ca65
; - split of words in tables containing hi/lo-byte, doubling #patterns
; - shifted bit 7 of patternid to bit 6 to indicate low volume pattern
; - bit 7 of patternid for voice0 now disables hihat for that pattern

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
psmkPlayer:
;--------------------------------------------------------------------------
; Generates tempo based on TEMPODELAY
;--------------------------------------------------------------------------
   inc psmkTempoCount
   lda psmkTempoCount
   and #$7f
.ifconst TEMPODELAY
   eor #TEMPODELAY
.else
   eor TEMPODELAY
.endif
   bne quitTempo
   sta psmkTempoCount

   inc psmkBeatIdx
   lda psmkBeatIdx
   eor #$20
   bne quitTempo
   sta psmkBeatIdx

   inc psmkPatternIdx

quitTempo:
;--------------------------------------------------------------------------

   ldx psmkPatternIdx
   lda psmkSong1,x

   ; Check to see if the end of the song was reached
   cmp #$ff
   bne notEndOfSong

   ; Go back to the first measure
   ldx #$00
   stx psmkPatternIdx
   lda psmkSong1,x

notEndOfSong:
   and #$80
   ora psmkTempoCount
   sta psmkTempoCount
   lda psmkSong1,x
   
   ldx #$00 ; first voice
   jsr playPattern

   ldx psmkPatternIdx
   lda psmkSong2,x

   ldx #$01 ; second voice
   ; slip through
   ; songPlayer
;--------------------------------------------------------------------------
; playPattern
;--------------------------------------------------------------------------
; Plays a pattern
;
; - A should contain the offset in the patternArray of the pattern to play
; - X should contain the oscillator to be used (0 or 1)
;
;--------------------------------------------------------------------------
playPattern:

   ; save unaltered patternArray offset
   sta temp16+0
   asl
   asl
   sta temp8

   ; custom code to allow 1 quarter note per measure (Thrust):
   ; use beat to determine extra offset within patternArray
   lda psmkBeatIdx
   and #%00011000
   lsr
   lsr
   lsr

   ; add in original offset
   ora temp8

   ; save osc number
   stx temp8

   tax

   ; This mod allows for high and low volume patterns.
   ; Patterns of offset greater than 64 read from a different
   ; array and play lower.
   bit temp16+0
   bvs lowPattern

   ; Loud version
   ; Get address of selected pattern
   lda psmkHiVolPatternsLo,x
   ldy psmkHiVolPatternsHi,x

   ; Set 0 attenuation
   ldx #$00
   beq endGetPattern

lowPattern:
   ; Soft version
   ; Get address of selected pattern
   lda psmkLoVolPatternsLo,x
   ldy psmkLoVolPatternsHi,x

   ; Set -6 attenuation
   ldx #$04

endGetPattern:
   sta temp16+0
   sty temp16+1
   stx psmkAttenuation

   ; The variable, beat, contains the 32nd note
   ; that the beat is currently on.
   lda psmkBeatIdx

   ; modification for 1 quarter per measure (Thrust)
   and #%00000111
   tay

   ; Get sound/note data
   lda (temp16),y
   eor #$ff
   beq muteNote
   eor #$ff

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
   lda psmkAttenuation
   clc
   adc psmkSoundAttenArray,x
   sta psmkAttenuation
;-----------------------

   lda psmkSoundTypeArray,x
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
   ldy #$08

   lda (temp16),y
   and bitMaskArray,x
   beq noAccent  ; eq also sets carry

   ; It's an Accent, so don't attenuate
   lda #$0f

noAccent:
   ; No accent, so use a lower volume
   ora #$0d
;--------------------------------------------------------------------------

   sbc psmkAttenuation               ; carry flag???
muteNote:
   ldx temp8               ; Get the osc number again
   sta AUDV0,x
.if SHADOW_REGISTERS
   sta audv0,x
.endif


   ;--------------------------------------------------------------------------
   ; Super High Hat (TM)
   ;--------------------------------------------------------------------------
   ; This plays the high hat sound on the first frame of each beat indicated
   ; in hatPattern
   ;--------------------------------------------------------------------------
   lda temp8
   beq noHat

   ; Only play had on first frame
   lda psmkTempoCount
   bne noHat

   lda psmkBeatIdx
   and #%00000111
   tax
   lda psmkBeatIdx
   lsr
   lsr
   lsr
   tay
   lda psmkHatPattern,y
   and bitMaskArray,x
   beq noHat

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
   lda psmkPatternIdx
   cmp #111   ; start measure
   bmi noCut

   lda psmkTempoCount
   and #%11111110
   beq noCut

   lda #0
   sta AUDV0
.if SHADOW_REGISTERS
   sta audv0
.endif
.endif

   rts ; playPattern OTHER RTS IN FUNCTION
