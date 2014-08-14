#
# src/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

# CMAKE_PARSE_ARGUMENTS was introduced with 2.8.3
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.3 )

PROJECT( apefat )

SET( VERSION "0.0.1" )

IF( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
   MESSAGE( FATAL_ERROR "Only out of sources builds are supported" )
ENDIF( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )

MESSAGE( STATUS "==== src ====" )
MESSAGE( STATUS "configuring for a ${CMAKE_SYSTEM_NAME} system" )

SET( CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

IF( CMAKE_TOOLCHAIN_FILE )
   MESSAGE( STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}" )
ENDIF( CMAKE_TOOLCHAIN_FILE )

IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release )
ENDIF( NOT CMAKE_BUILD_TYPE )

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   MESSAGE( STATUS "Debug build" )
   ADD_DEFINITIONS( -DDEBUG )
   SET( CMAKE_VERBOSE_MAKEFILE true )
   SET( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/root" )
ELSE( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   MESSAGE( STATUS "non-debug build" )
   IF( CMAKE_SYSTEM_NAME MATCHES [Ll]inux )
      INCLUDE( DebianHardening )
   ENDIF( CMAKE_SYSTEM_NAME MATCHES [Ll]inux )
   ADD_DEFINITIONS( -ffunction-sections -fdata-sections -DNDEBUG )
   SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections" )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )

IF( NOT CMAKE_INSTALL_PREFIX )
   MESSAGE( FATAL_ERROR "CMAKE_INSTALL_PREFIX is not set but it has to be. Something is wrong." )
ENDIF()
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )

#LINK_DIRECTORIES( /usr/local/lib/ ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )

ADD_DEFINITIONS( -Wall )
INCLUDE( FindPkgConfig )

# Qt4 is needed for gui, etc
FIND_PACKAGE( Qt4 4.6.0 REQUIRED )
MESSAGE( STATUS "Qt Edition: ${QT_EDITION}" )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES(
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
)

# SDL is needed for audio only
SET( SDL2_BUILDING_LIBRARY 1 )
INCLUDE( FindSDL2 )
#FIND_PACKAGE( SDL REQUIRED )
#FindSDL()

# end of searching for external components print out results
IF( NOT SDL2_FOUND )
   MESSAGE( STATUS "SDL2 NOT FOUND!")
   SET( FAIL TRUE )
ELSE( NOT SDL2_FOUND )
   MESSAGE( STATUS "SDL2 found." )
   MESSAGE( STATUS "  include dir: ${SDL2_INCLUDE_DIRS}" )
   MESSAGE( STATUS "  libraries: ${SDL2_LIBRARIES}" )
ENDIF( NOT SDL2_FOUND )
MESSAGE( STATUS "Qt Edition: ${QT_EDITION} Version: ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}" )
MESSAGE( STATUS "QT_USE_FILE=${QT_USE_FILE}" )

IF( FAIL )
   MESSAGE( FATAL "Not all build requirements are fulfilled" )
ENDIF( FAIL )

# additional cmake files
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake )

########## Add make dist target ################
SET( CPACK_PACKAGE_VERSION "${VERSION}" )
SET( CPACK_SOURCE_GENERATOR "TBZ2" )
SET( CPACK_SOURCE_PACKAGE_FILE_NAME "slart-${CPACK_PACKAGE_VERSION}" )
SET( CPACK_IGNORE_FILES "/CVS/;/\\\\.git\;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;\\\\.tar.gz$;/CMakeFiles/;CMakeCache.txt;refresh-copyright-and-license.pl" )
SET( CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES} )
INCLUDE(CPack)
# simulate autotools' "make dist"
#add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

SET( TRANSLATIONS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/resources/translations CACHE PATH "translations directory" )
SET( BIN_DIR ${CMAKE_BINARY_DIR}/bin CACHE PATH "binaries directory" )
SET( LIB_DIR ${CMAKE_BINARY_DIR}/lib CACHE PATH "libraries directory" )
SET( TEST_DIR ${CMAKE_BINARY_DIR}/test CACHE PATH "test binaries directory" )


SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Wno-long-long" )

# order is important: only later can access definition of earlier subdirs
ADD_SUBDIRECTORY( json )
ADD_SUBDIRECTORY( notemapping )
ADD_SUBDIRECTORY( mos6502 )
ADD_SUBDIRECTORY( main )

