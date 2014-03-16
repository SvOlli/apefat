#
# src/apps/VCSTracker/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/main ====" )

PROJECT( apefat )

INCLUDE( LocalTools )
INCLUDE( FindPkgConfig )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${JSON_INCLUDE_DIR}
   ${NOTEMAPPING_INCLUDE_DIR}
   ${MOS6502_INCLUDE_DIR}
   ${SDL_INCLUDE_DIR}
)

# workaround for mxe/windows
IF( NOT SDL_LIBRARIES )
   MESSAGE( STATUS "SDL not found, using workaround" )
   IF( ${CMAKE_COMPILER_IS_MINGW} )
      SET( SDL_LIBRARIES SDL dxguid )
   ELSE( ${CMAKE_COMPILER_IS_MINGW} )
      SET( SDL_LIBRARIES SDL )
   ENDIF( ${CMAKE_COMPILER_IS_MINGW} )
ENDIF( NOT SDL_LIBRARIES )

ADD_SOURCE( Application             cpp         )
ADD_SOURCE( MainWindow              cpp hpp moc )
ADD_SOURCE( MainWidget              cpp hpp moc )
ADD_SOURCE( PlayerEmulation         cpp hpp moc )
ADD_SOURCE( PsmkBeatWidget          cpp hpp moc )
ADD_SOURCE( PsmkClipboard           cpp hpp moc )
ADD_SOURCE( PsmkConfig              cpp hpp     )
ADD_SOURCE( PsmkContextMenu         cpp hpp moc )
ADD_SOURCE( PsmkHiHatWidget         cpp hpp moc )
ADD_SOURCE( PsmkInstrumentComboBox  cpp hpp moc )
ADD_SOURCE( PsmkInstrumentsWidget   cpp hpp moc )
ADD_SOURCE( PsmkMainWidget          cpp hpp moc )
ADD_SOURCE( PsmkPacker              cpp hpp moc )
ADD_SOURCE( PsmkPatternSelector     cpp hpp moc )
ADD_SOURCE( PsmkPatternWidget       cpp hpp moc )
ADD_SOURCE( PsmkPitchComboBox       cpp hpp moc )
ADD_SOURCE( PsmkPlayerWidget        cpp hpp moc )
ADD_SOURCE( PsmkToneComboBox        cpp hpp moc )
ADD_SOURCE( PsmkNoteWidget          cpp hpp moc )
ADD_SOURCE( QxtLabel                cpp hpp moc )
ADD_SOURCE( Trace                   cpp hpp     )

SET( apefat_RESOURCES
   ../resources/apefat.qrc
)

QT4_WRAP_CPP( apefat_MOC_GENERATED
   ${apefat_MOC_HEADERS}
)

QT4_ADD_RESOURCES( apefat_RESOURCES_RCC
   ${apefat_RESOURCES}
)

ADD_EXECUTABLE( apefat ${GUI_TYPE}
   ${apefat_MOC_GENERATED}
   ${apefat_HEADERS}
   ${apefat_SOURCES}
   ${apefat_RESOURCES_RCC}
)

ADD_DEPENDENCIES( apefat
   json
   notemapping
)

TARGET_LINK_LIBRARIES( apefat
   ${JSON_LIBRARY_FLAGS}
   ${NOTEMAPPING_LIBRARY_FLAGS}
   ${MOS6502_LIBRARY_FLAGS}
   ${SDL_LIBRARIES}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS apefat
   LIBRARY DESTINATION lib
   RUNTIME DESTINATION bin
)

MESSAGE( STATUS "SDL_LIBRARIES=${SDL_LIBRARIES}" )
