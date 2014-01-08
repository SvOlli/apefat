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
)

ADD_SOURCE( Application          cpp         )
ADD_SOURCE( BarWidget            cpp hpp moc )
ADD_SOURCE( BarsTabWidget        cpp hpp moc )
ADD_SOURCE( BeatWidget           cpp hpp moc )
ADD_SOURCE( HiHatWidget          cpp hpp moc )
ADD_SOURCE( MainWindow           cpp hpp moc )
ADD_SOURCE( MainWidget           cpp hpp moc )
ADD_SOURCE( NoteWidget           cpp hpp moc )
ADD_SOURCE( PitchComboBox        cpp hpp moc )
ADD_SOURCE( SlocumBar            cpp hpp     )
ADD_SOURCE( SlocumBarList        cpp hpp     )
ADD_SOURCE( SlocumBeat           cpp hpp     )
ADD_SOURCE( SlocumHiHat          cpp hpp     )
ADD_SOURCE( SlocumNote           cpp hpp     )
ADD_SOURCE( SlocumSong           cpp hpp     )
ADD_SOURCE( SlocumSound          cpp hpp     )
ADD_SOURCE( SongTabWidget        cpp hpp moc )
ADD_SOURCE( SoundComboBox        cpp hpp moc )
ADD_SOURCE( SoundWidget          cpp hpp moc )
ADD_SOURCE( Trace                cpp hpp     )
ADD_SOURCE( VoiceModel           cpp hpp moc )
ADD_SOURCE( VoiceWidget          cpp hpp moc )

QT4_WRAP_CPP( apefat_MOC_GENERATED
   ${apefat_MOC_HEADERS}
)

ADD_EXECUTABLE( apefat ${GUI_TYPE}
   ${apefat_MOC_GENERATED}
   ${apefat_HEADERS}
   ${apefat_SOURCES}
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
