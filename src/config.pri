
win32 {
INCLUDEPATH  += C:/Qt/SDL2-2.0.3/include
QMAKE_LIBDIR += C:/Qt/SDL2-2.0.3/lib/x86
LIBS         += -lSDL2
} else {
CONFIG       += link_pkgconfig
PKGCONFIG    += sdl2
# should also work with SDL1.2
}

