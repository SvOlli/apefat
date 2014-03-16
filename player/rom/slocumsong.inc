
; Paul Slocum
; http://qotile.net
;
;
; (any text to the right of a semicolon is a comment)
;
;-------------------------------------------------------------
; SONG DATA FILE
;-------------------------------------------------------------
;
; This file contains all the data that makes up a song for
; the song player.  By editing this file, you can write your
; own music on the 2600.  The process involves writing short 
; note patterns. arranging them
; by fours into full patterns, then sequencing those patterns
; into a song.  After you write a pattern, you'll add it to the
; pattern arrays, and then can use it in one or both of the song
; pattern lists.  There are also other parameters you can adjust
; like the auto high hat and tempo.  
; 
; This file contains data for a short demo song and I've
; explained everything that I've done.
;
; To learn how the song file works, I suggest that
; you scroll down to the bottom of this file and 
; look at the patterns first, then work your way up
; to the pattern arrays and then the song pattern
; lists.
;
; This file explains the mechanics of how to create a song,
; but for note tables and lots of tips and tricks you'll
; want to get my 2600 music programming guide off my
; website.
;



;-------------------------------------------------------------
; Tempo
;-------------------------------------------------------------
; TEMPODELAY sets the song tempo.  Higher values mean a slower
; song.  1 is extremely fast and 10 is very slow.  The tempo 
; is currently only adjustable in very large increments.
; Future versions of the software will allow more precise tempo
; settings.  Typically I use a TEMPODELAY or 3 or 4.

.define TEMPODELAY 3


;-------------------------------------------------------------
; Sound Attenuation Array
;-------------------------------------------------------------
; This section allows you to change the volume
; of each sound type globally.  Many times you'll
; have a certain sound that ends up being too loud,
; You can fix that here.  Note that a larger value
; makes the sound quieter.  Set all 8 values to zero
; if you want all sounds at full volume.  10 is probably 
; about the highest amount of attenuation you'll want.
;
; 000=square  001=bass  010=pitfall  011=noise
; 100=buzz    101=lead  110=saw      111=lowbass
psmkSoundAttenArray:
   .byte 8, 0, 5, 9
   .byte 0, 6, 4, 0



;-------------------------------------------------------------
; Sound Type Array
;-------------------------------------------------------------
; This array allows you to change the sound types
; that are encoded into the first three bits of
; notes (discusses later.)  I'd suggest leaving
; this array alone until you are pretty comfortable
; writing music for the 2600.  I rarely change these
; settings.
;
; There are only 10 unique sound types on the Atari.
; I've selected the 8 sound types that
; I think are the most useful.  The two that I have not 
; included in the default setup are 2 and 14.  2 is a 
; low rumble engine sound and 14 is a very low bass 
; sound.  In addition to replacing a default sound
; with 2 or 14, you could also use this array to gain
; more precise volume control over a sound.  Say
; you want more control over the Square sound:  Just
; replace a sound you aren't using with another
; Square (4) and then using the Sound Attenuation
; Array, you can have a loud Square sound and a 
; quieter Square. 
;
;
; Default Sound Type Setup:
; 000 0 Square  = 4
; 001 1 Bass    = 6
; 010 2 Pitfall = 7
; 011 3 Noise   = 8
;
; 100 4 Buzz    = 15
; 101 5 Lead    = 12
; 110 6 Saw     = 1
; 111 7 Engine  = 3
;
psmkSoundTypeArray:
    .byte 4,6,7,8
    .byte 15,12,1,14


;-------------------------------------------------------------
; Auto High Hat
;-------------------------------------------------------------
; The latest driver has built-in support to
; play a high-hat sound.  This allows you to have
; a high hat rhythm playing without having to add it to
; your pattern data.  The driver plays the high-hat
; click very short, so often you will barely notice
; any interference with your song.  It plays on
; the same oscillator as the "song2" list of patterns.
; If it is interfering with your music too much,
; just swap song1 and song2.
;
; You specify the high hat pattern in hatPattern.  Each
; bit corresponds to one 32nd note.  Set the bit to one
; to have it play on that beat, or zero to mute the high
; hat on that beat.  The pattern repeats each measure.
;
; You can also specify the measure that the high hat
; starts playing with HATSTART.  You may want to have
; and intro part without the high hat then have it
; come in later.  Or if you don't want to use it at all,
; then set HATSTART to 255.
;
; Finally, you can specify the high hat volume (0-15), the
; pitch (0-31), and the high hat sound type.  Refer to the
; table above for sound type values.  I typically use
; sound type 8 (Noise) and pitch 0 which sounds like a pretty
; good high hat.  But you may find other interesting settings.
;

hatPattern:
   .byte %10001000
   .byte %10001000
   .byte %10001000
   .byte %10001010

.define HATVOLUME $07
.define HATPITCH  $00
.define HATSOUND  $08

;-------------------------------------------------------------
; Song Data
;-------------------------------------------------------------
; song1 and song2 are lists of four pattern sets to be
; played out of patternArrayH and patternArrayL.
; Both song1 and song2 will be played simlutaneously 
; by the software, one on each of the two oscillators,
; so each song should be exactly the same length or you
; may have some problems!  
;
; Each number (after ".byte") refers
; to a four pattern set in the patternArrayH (loud
; patterns) or patternArrayL (softer patterns).  
;
; "song1" and "song2" must be against the left
; edge, and each ".byte" statement must have at
; least one space or tab to the left of it.  I
; have only put two pattern numbers on each line
; but you can put as many as you want (each number
; separated by a comma).  A song cannot
; exceed 255 patterns.
;
; You must put a 255 at the end of the pattern list
; to let the driver know that it has reached the end.
; The player will automatically loop back to the beginning
; of the song when it reaches the end.
;
; I suggest that you comment your songs so you can
; remember what's going on where.  (Use a semicolon
; to indicate a comment)
;
; valid numbers here are 0-31 and 64-159


psmkSong1:
   ; Just the drums:
   ; (Here, I'm playing the same
   ; drum pattern on both oscillators
   ; which gives a phasing effect.)
   .byte 1,2
   .byte 1,3

   ; Bass comes in (on song1)
   ;.byte 64,65
   .byte 192,193
   .byte 64,65

   .byte 64,65
   .byte 64,0

   ; Melody part comes in
   ; (Still playing bass on song1)
   .byte 64,65
   .byte 64,65

   .byte 64,65
   .byte 64,0

   .byte 64,65
   .byte 64,65

   .byte 64,65
   .byte 64,0

   ; End of song marker
   .byte 255


psmkSong2:
   ; Just the drums:
   .byte 1,2
   .byte 1,3

   ; Bass comes in
   ; (Still playing drums on song2)
   .byte 1,2
   .byte 1,3

   .byte 1,2
   .byte 1,3

   ; Melody comes in on song2
   ; (I'm now switching to a drum
   ; part that also has a melody.)
   .byte 4,5
   .byte 4,6

   .byte 4,5
   .byte 4,6

   .byte 4,5
   .byte 4,6

   .byte 4,5
   .byte 4,6

   ; End of song marker
   .byte 255



;-------------------------------------------------------------
; Pattern Arrays
;-------------------------------------------------------------
; The pattern arrays contains sets of four patterns. The
; numbers in the song1 and song2 lists reference
; the pattern arrays.
;
; There are two pattern arrays.  Patterns in 
; patternArrayH will play louder, while patterns 
; in patternArrayL will play softer.  This gives you
; a bit more control over volume.  You can even have
; a soft and loud version of the same pattern.
;
; In my original driver, patterns were a full measure
; long.  The reason I've divided the patterns into
; fours is because it results in a lot more reuse of
; pattern data.  You can reuse patterns as much as
; you want.  For example, you may have a drum beat
; and want to add a fill.  You can reuse the first
; three patterns, and create a new fill pattern
; for the end of the measure.  This ends up saving
; a lot of space.
;
; I always put a comment
; to the right with the pattern number so when I'm 
; writing the song (above) I can easily see the number
; of the pattern.  
;
; You'll find data for each of these patterns below.
; Make sure the name in the patternArray matches the
; name below or you'll get an error.  patternArrayL
; and patternArrayH may have a maximum of 64 sets of
; 4 patterns each.
;
; Note that pattern sets in patternArrayH start at
; zero, while pattern sets in patternArrayL start at
; 64.
;


psmkHiVolPatternsLo:                   ; starts at 0

   ; Muted pattern
   .lobytes mute,mute,mute,mute         ;0

   ; My 3 drum patterns
   .lobytes KickDrum1a,Snare1a,KickDrum1b,Snare1a   ;1
   .lobytes KickDrum1a,Snare1a,KickDrum1b,Snare1b   ;2
   .lobytes KickDrum1a,Snare1a,KickDrum1a,Snare1c   ;3

   ; 3 More drum patterns, but with a melody
   .lobytes KickMel1a,Snare1a,KickMel1b,Snare1a   ;4
   .lobytes KickMel1a,Snare1a,KickMel1d,Snare1b   ;5
   .lobytes KickMel1c,Snare1a,KickMel1e,Snare1c   ;6

psmkHiVolPatternsHi:                   ; starts at 0
   ; Muted pattern
   .hibytes mute,mute,mute,mute         ;0

   ; My 3 drum patterns
   .hibytes KickDrum1a,Snare1a,KickDrum1b,Snare1a   ;1
   .hibytes KickDrum1a,Snare1a,KickDrum1b,Snare1b   ;2
   .hibytes KickDrum1a,Snare1a,KickDrum1a,Snare1c   ;3

   ; 3 More drum patterns, but with a melody
   .hibytes KickMel1a,Snare1a,KickMel1b,Snare1a   ;4
   .hibytes KickMel1a,Snare1a,KickMel1d,Snare1b   ;5
   .hibytes KickMel1c,Snare1a,KickMel1e,Snare1c   ;6

   ; Lower volume patterns
psmkLoVolPatternsLo:                   ; start at 64
   ; My 2 bass patterns
   .lobytes Bass1a, Bass1a, Bass1b, Bass1b      ;64
   .lobytes Bass1c, Bass1c, Bass1b, Bass1b      ;65

psmkLoVolPatternsHi:                   ; start at 64
   ; My 2 bass patterns
   .hibytes Bass1a, Bass1a, Bass1b, Bass1b      ;64
   .hibytes Bass1c, Bass1c, Bass1b, Bass1b      ;65

;-------------------------------------------------------------
; Pattern Data
;-------------------------------------------------------------
; This is where the note patterns are defined.  Each pattern
; is a quarter note long and is divided into 8 steps (32nd
; notes).  Each step in the pattern specifies the pitch
; and sound type for that pitch.  These patterns are
; arranged by fours into full measure patterns in the
; pattern arrays above.
;
; Way below you'll find data for the patterns listed in the
; pattern arrays above.  Each one has a name and is followed
; by 9 .bytes (numbers) of data.  Each of
; the first 8 numbers in the data set represents 
; a 32nd note step.  The .byte after that determines
; which 32nd notes are accented (louder).
;
; It is very helpful if you reuse patterns you've written.
; Find one that's similar to what you need, copy it, 
; and modify it.
; 
; Note that the percent sign means that you
; are entering a binary number.
;
;
; Note Encoding
; ----------------------------------------------------------
;
; The 8 note numbers are encoded with sound type and pitch.
; I put two 32nd notes per line like this:
;
;   .byte %00111000, %00111000
;
; The first three bits (1's and  0's) determine the 
; sound type according to the following table:
;
; 000 Square  (high square wave)
; 001 Bass    (fat bass sound)
; 010 Pitfall (sound of hitting a lot in pitfall)
; 011 Noise   (white noise)
; 100 Buzz    (hard buzzy sound)
; 101 Lead    (lower square wave)
; 110 Saw     (sounds kind of like a sawtooth wave)
; 111 Engine  (engine sound)
;
;
; The remaining 5 bits determine the pitch.
;
; 11111 is a very low pitch
; 00001 us a very high pitch
; 00100 is somewhere in between
;
; (!) Note that 255 or %11111111 means no sound (a rest)
;
; Some examples:
;
; %00111100 = bass with a low pitch
; %10000010 = buzz sound with a high pitch
; %11100101 = engine sound with a medium pitch
; %11111111 = no sound
;
;
; The pitch on the Atari is not all on a musical scale,
; so you'll end up with some really out-of-tune stuff
; if you aren't careful.  Refer to my 2600 music 
; programming guide for some sets of in-tune notes.
;
;
; Accent Encoding
; ----------------------------------------------------------
;
; There will be ninth number at the end of each pattern like:
;  
;   .byte %10001000
;
; This determines which notes are accented.  Good accents can 
; really make a pattern a lot better.  Notice there are 
; 8 bits, and conveniently they correspond to each of the 8
; notes in the pattern!  The way I have it set up above, 
; accents fall on each 8th note.  
;
; 1 = accent (loud), 0 = no accent (no as loud)
; 
; To just make the whole beat quieter, use all zeros:
;
; .byte %00000000
;
; To make the whole beat louder:
;
; .byte %11111111
;
; Here's an accent pattern I use often:
;
; .byte %10001000
;
;
;
; Patterns:
; ----------------------------------------------------------
;
; Here are the patterns for my example song.  
; I try to name them
; meaningful names so I'll remember what they are.  
; You can name then whatever you want, just
; make sure your names start with a letter.


; Here are my kick drum patterns:

KickDrum1a:
   .byte %10011110, %01111110
   .byte 255,255
   .byte 255,255
   .byte 255,255

   .byte %10000000

KickDrum1b:
   .byte %10011110, 255
   .byte 255,255
   .byte 255,255
   .byte %10011110, 255

   .byte %10000000


; Now this is a kick drum with some melody mixed in.
; Since you only have two oscillators, you have to
; cram as much as possible into your patterns.

KickMel1a:
   .byte %10011110, %01111110
   .byte %10101100, 255
   .byte %10110110, 255
   .byte %10111110, 255

   .byte %10000000

KickMel1b:
   .byte %10011110, 255
   .byte %10111110, 255
   .byte %10110110, 255
   .byte %10011110, 255

   .byte %10000000


KickMel1c:
   .byte %10011110, %01111110
   .byte %00010110, 255
   .byte %00011001, 255
   .byte %00011100, 255

   .byte %10000000

KickMel1d:
   .byte %10011110, 255
   .byte %11011110, 255
   .byte %10110110, 255
   .byte %11011011, 255

   .byte %10000000

KickMel1e:
   .byte %10011110, 255
   .byte %01000100, 255
   .byte %01000010, 255
   .byte %01000100, 255

   .byte %10000000


; My snare drum parts.  In the pattern arrays I'll
; alternate between kick and snare to make the beat.

Snare1a:
   .byte %10000100, %01100100
   .byte %01100101, %01100101
   .byte 255, 255
   .byte 255, 255
   
   .byte %11000000

Snare1b:
   .byte %10000100, %01100100
   .byte %01100101, %01100101
   .byte %10011110, 255
   .byte 255, 255
   
   .byte %11000000

Snare1c:
   .byte %10000100, %01100100
   .byte %01100101, %01100101
   .byte %10000100, %01100100
   .byte %10000100, %01100101
   
   .byte %11000000


; Here are my bass parts:

Bass1a:
   .byte %00110011, %00110011
   .byte 255, 255
   .byte %00110011, %00110011
   .byte %00110011, 255

   .byte %10001000

Bass1b:
   .byte %00101110, %00101110
   .byte 255,255
   .byte %00101110, %00101110
   .byte %00101110,255

   .byte %10001000

Bass1c:
   .byte %00110111, %00110111
   .byte 255,255
   .byte %00110111, %00110111
   .byte %00110111,255

   .byte %10001000


; I always have a muted pattern.
; You'll pretty much always need this.

mute:
   .byte 255,255,255,255
   .byte 255,255,255,255

   .byte 255
