
#include "Tones.h"

tia_tone_t *Tones_pal[TONES_SIZE] =
{
   &Tone0_11[0],
   &Tone1_pal[0],
   &Tone2_3_pal[0],
   &Tone2_3_pal[0],
   &Tone4_5_pal[0],
   &Tone4_5_pal[0],
   &Tone6_7_9_10_pal[0],
   &Tone6_7_9_10_pal[0],
   &Tone8_pal[0],
   &Tone6_7_9_10_pal[0],
   &Tone6_7_9_10_pal[0],
   &Tone0_11[0],
   &Tone12_13_pal[0],
   &Tone12_13_pal[0],
   &Tone14_15_pal[0],
   &Tone14_15_pal[0]
};

tia_tone_t *Tones_ntsc[TONES_SIZE] =
{
   &Tone0_11[0],
   &Tone1_ntsc[0],
   &Tone2_3_ntsc[0],
   &Tone2_3_ntsc[0],
   &Tone4_5_ntsc[0],
   &Tone4_5_ntsc[0],
   &Tone6_7_9_10_ntsc[0],
   &Tone6_7_9_10_ntsc[0],
   &Tone8_ntsc[0],
   &Tone6_7_9_10_ntsc[0],
   &Tone6_7_9_10_ntsc[0],
   &Tone0_11[0],
   &Tone12_13_ntsc[0],
   &Tone12_13_ntsc[0],
   &Tone14_15_ntsc[0],
   &Tone14_15_ntsc[0]
};

char *Tones_names[TONES_SIZE] =
{
   "none",
   "buzzy",
   "rumble",
   "enduro",
   "pure high",
   "pure high*",
   "motor",
   "saw",
   "jet",
   "saw*",
   "motor*",
   "none*",
   "pure low",
   "pure low*",
   "diesel",
   "unleaded"
};
