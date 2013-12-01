#
# src/apps/json/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/json ====" )

PROJECT( json )

INCLUDE( LocalTools )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
)

ADD_SOURCE( json                 cpp h       )

QT4_WRAP_CPP( json_MOC_GENERATED
   ${json_MOC_HEADERS}
)

ADD_LIBRARY( json
   ${json_MOC_GENERATED}
   ${json_HEADERS}
   ${json_SOURCES}
)

PROPAGATE_LIBRARY( json )
