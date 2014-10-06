
win32 {
INCLUDEPATH  += C:/Qt/SDL2-2.0.3/include
QMAKE_LIBDIR += C:/Qt/SDL2-2.0.3/lib/x86
LIBS         += -lSDL2
} else {
INCLUDEPATH  += /usr/include/SDL2
QMAKE_LIBDIR +=
LIBS         += -lSDL2
}

