
#ifndef TONES_H
#define TONES_H TONES_H

#ifdef __cplusplus
extern "C" {
#endif

#define TONES_SIZE 16
#define TONE_SIZE  32

struct tia_tone_s
{
   int id;
   char name[4];
   int percent;
   float should;
   float is;
};
typedef struct tia_tone_s tia_tone_t;

extern tia_tone_t Tone0_11[TONE_SIZE];
extern tia_tone_t Tone1_ntsc[TONE_SIZE];
extern tia_tone_t Tone1_pal[TONE_SIZE];
extern tia_tone_t Tone2_3_ntsc[TONE_SIZE];
extern tia_tone_t Tone2_3_pal[TONE_SIZE];
extern tia_tone_t Tone4_5_ntsc[TONE_SIZE];
extern tia_tone_t Tone4_5_pal[TONE_SIZE];
extern tia_tone_t Tone6_7_9_10_ntsc[TONE_SIZE];
extern tia_tone_t Tone6_7_9_10_pal[TONE_SIZE];
extern tia_tone_t Tone8_ntsc[TONE_SIZE];
extern tia_tone_t Tone8_pal[TONE_SIZE];
extern tia_tone_t Tone12_13_ntsc[TONE_SIZE];
extern tia_tone_t Tone12_13_pal[TONE_SIZE];
extern tia_tone_t Tone14_15_ntsc[TONE_SIZE];
extern tia_tone_t Tone14_15_pal[TONE_SIZE];

extern tia_tone_t *Tones_ntsc[TONES_SIZE];
extern tia_tone_t *Tones_pal[TONES_SIZE];

extern char *Tones_names[TONES_SIZE];

#ifdef __cplusplus
}
#endif

#endif
