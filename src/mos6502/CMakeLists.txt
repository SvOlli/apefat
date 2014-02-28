#
# src/apps/mos6502/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/mos6502 ====" )

PROJECT( mos6502 )

INCLUDE( LocalTools )

INCLUDE_DIRECTORIES( ${SDL_INCLUDE_DIR}
)

ADD_SOURCE( bspf                  hpp )
ADD_SOURCE( cpu6502           cpp hpp )
ADD_SOURCE( PlayerConfig      cpp hpp )
ADD_SOURCE( SoundSDL2         cpp hpp )
ADD_SOURCE( System                hpp )
ADD_SOURCE( TIASnd            cpp hpp )

ADD_LIBRARY( mos6502
   ${mos6502_HEADERS}
   ${mos6502_SOURCES}
)

PROPAGATE_LIBRARY( mos6502 )
