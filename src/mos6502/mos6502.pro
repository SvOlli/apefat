
TEMPLATE     = lib
TARGET       = mos6502
DEPENDPATH  += .
INCLUDEPATH += .
CONFIG      += staticlib

# for SDL
include ( ../config.pri )

# Input
HEADERS += bspf.hpp \
           cpu6502.hpp \
           PlayerConfig.hpp \
           SoundSDL2.hpp \
           System.hpp \
           TIASnd.hpp
SOURCES += cpu6502.cpp \
           PlayerConfig.cpp \
           SoundSDL2.cpp \
           TIASnd.cpp
