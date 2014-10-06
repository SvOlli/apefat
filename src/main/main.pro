
TEMPLATE     = app
TARGET       = apefat
DEPENDPATH  += .
INCLUDEPATH += . ../json ../notemapping ../mos6502
LIBS        += -L../json -ljson
LIBS        += -L../notemapping -lnotemapping
LIBS        += -L../mos6502 -lmos6502
QT          += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# for SDL
include ( ../config.pri )

# Input
HEADERS += MainWidget.hpp \
           MainWindow.hpp \
           PlayerEmulation.hpp \
           PsmkBeatWidget.hpp \
           PsmkConfig.hpp \
           PsmkContextMenu.hpp \
           PsmkHiHatWidget.hpp \
           PsmkInstrumentComboBox.hpp \
           PsmkInstrumentsWidget.hpp \
           PsmkNoteWidget.hpp \
           PsmkPacker.hpp \
           PsmkPatternProxyWidget.hpp \
           PsmkPatternSelector.hpp \
           PsmkPatternSequenceDialog.hpp \
           PsmkPatternSequenceEditor.hpp \
           PsmkPatternWidget.hpp \
           PsmkPitchComboBox.hpp \
           PsmkPlayerWidget.hpp \
           PsmkSongWidget.hpp \
           PsmkToneComboBox.hpp \
           QxtLabel.hpp \
           Trace.hpp
SOURCES += Application.cpp \
           MainWidget.cpp \
           MainWindow.cpp \
           PlayerEmulation.cpp \
           PsmkBeatWidget.cpp \
           PsmkConfig.cpp \
           PsmkContextMenu.cpp \
           PsmkHiHatWidget.cpp \
           PsmkInstrumentComboBox.cpp \
           PsmkInstrumentsWidget.cpp \
           PsmkNoteWidget.cpp \
           PsmkPacker.cpp \
           PsmkPatternProxyWidget.cpp \
           PsmkPatternSelector.cpp \
           PsmkPatternSequenceDialog.cpp \
           PsmkPatternSequenceEditor.cpp \
           PsmkPatternWidget.cpp \
           PsmkPitchComboBox.cpp \
           PsmkPlayerWidget.cpp \
           PsmkSongWidget.cpp \
           PsmkToneComboBox.cpp \
           QxtLabel.cpp \
           Trace.cpp
RESOURCES += ../resources/apefat.qrc
