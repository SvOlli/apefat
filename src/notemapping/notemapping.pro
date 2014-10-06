
TEMPLATE     = lib
TARGET       = notemapping
DESTDIR      = ../lib
DEPENDPATH  += .
INCLUDEPATH += .
CONFIG      += staticlib

# Input
HEADERS += Tones.h
SOURCES += Tone0_11.c \
           Tone12_13_ntsc.c \
           Tone12_13_pal.c \
           Tone14_15_ntsc.c \
           Tone14_15_pal.c \
           Tone1_ntsc.c \
           Tone1_pal.c \
           Tone2_3_ntsc.c \
           Tone2_3_pal.c \
           Tone4_5_ntsc.c \
           Tone4_5_pal.c \
           Tone6_7_9_10_ntsc.c \
           Tone6_7_9_10_pal.c \
           Tone8_ntsc.c \
           Tone8_pal.c \
           Tones.c
