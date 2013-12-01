#
# src/apps/notemapping/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/notemapping ====" )

PROJECT( notemapping )

INCLUDE( LocalTools )

INCLUDE_DIRECTORIES(
)

ADD_SOURCE( Tones             c   h   )
ADD_SOURCE( Tone0_11          c       )
ADD_SOURCE( Tone1_ntsc        c       )
ADD_SOURCE( Tone1_pal         c       )
ADD_SOURCE( Tone2_3_ntsc      c       )
ADD_SOURCE( Tone2_3_pal       c       )
ADD_SOURCE( Tone4_5_ntsc      c       )
ADD_SOURCE( Tone4_5_pal       c       )
ADD_SOURCE( Tone6_7_9_10_ntsc c       )
ADD_SOURCE( Tone6_7_9_10_pal  c       )
ADD_SOURCE( Tone8_ntsc        c       )
ADD_SOURCE( Tone8_pal         c       )
ADD_SOURCE( Tone12_13_ntsc    c       )
ADD_SOURCE( Tone12_13_pal     c       )
ADD_SOURCE( Tone14_15_ntsc    c       )
ADD_SOURCE( Tone14_15_pal     c       )

ADD_LIBRARY( notemapping
   ${notemapping_HEADERS}
   ${notemapping_SOURCES}
)

PROPAGATE_LIBRARY( notemapping )
