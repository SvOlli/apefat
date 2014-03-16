/**
 * PsmkPlayerWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPLAYERWIDGET_HPP
#define PSMKPLAYERWIDGET_HPP PSMKPLAYERWIDGET_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QSpinBox;
class QThread;
class QToolButton;

/* forward declaration of local classes */
class PlayerEmulation;


/*!
 * \brief The SongBinary struct
 */
struct SongBinary
{
   int            songSize;
   int            highBeatSize;
   int            lowBeatSize;
   int            beatsSize;
   unsigned char  voice0[256];
   unsigned char  voice1[256];
   int            highBeatIndex[64][4];
   int            lowBeatIndex[64][4];
   unsigned char  beats[512][9];
   unsigned char  soundTypeArray[8];
   unsigned char  soundAttenArray[8];
   unsigned char  hatPattern[4];
   unsigned char  hatVolume;
   unsigned char  hatPitch;
   unsigned char  hatType;
   unsigned char  tempoDelay;
};


class PsmkPlayerWidget : public QGroupBox
{
   Q_OBJECT

public:
   explicit PsmkPlayerWidget( QWidget *parent = 0 );
   virtual ~PsmkPlayerWidget();

   SongBinary *songBinary();

public slots:
   void setText();
   void startStop( bool play );
   void updatePlayerState( int pattern, int note );
   void updateSong();
   void setPatternByCurrent();

signals:
   void playingPattern( int pattern );

private:
   Q_DISABLE_COPY( PsmkPlayerWidget )

   void setup();

   SongBinary           *mpSongBinary;
   PlayerEmulation      *mpPlayerEmulation;
   QToolButton          *mpPlayButton;
   QToolButton          *mpLoopButton;
   QSpinBox             *mpCurrentPattern;
   QSpinBox             *mpCurrentNote;
};

#endif
