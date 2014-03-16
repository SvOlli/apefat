/*
 * PlayerEmulation.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYEREMULATION_HPP
#define PLAYEREMULATION_HPP PLAYEREMULATION_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "PsmkPlayerWidget.hpp"

/* forward declaration of Qt classes */
class QTimer;

/* forward declaration of local classes */
class Cpu6502;
class PlayerConfig;
class SoundSDL2;
class TIASound;


/*!
 \brief \todo complete documentation

*/
class PlayerEmulation : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

   */
   explicit PlayerEmulation( QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PlayerEmulation();

public slots:
   /*!
    \brief \todo

    \param fileName
   */
   void loadPlayer( const QString &fileName );
   /*!
    \brief \todo

    \param bar bar to play (255 = song)
   */
   void start();
   /*!
    \brief \todo

   */
   void stop();
   /*!
    \brief \todo

   */
   void runFrame();
   /*!
    \brief \todo

   */
   void setCurrentBar( int bar );
   /*!
    \brief \todo

    \param enabled
   */
   void setLooping( bool enabled );

   void songToMemory( SongBinary *songBinary );

signals:
   void stateUpdate( int pattern, int note );

private:
   Q_DISABLE_COPY( PlayerEmulation )

   QTimer            *mpFrameTimer; /*!< \brief \todo TODO */
   TIASound          *mpTIA; /*!< \brief \todo TODO */
   SoundSDL2         *mpSoundSDL; /*!< \brief \todo TODO */
   PlayerConfig      *mpPlayerConfig; /*!< \brief \todo TODO */
   Cpu6502           *mp6502; /*!< \brief \todo TODO */
   QByteArray        mPlayerData; /*!< \brief \todo TODO */
   int               mCurrentPattern; /*!< \brief \todo TODO */
   int               mCurrentNote;
   bool              mLoopEnabled; /*!< \brief \todo TODO */
};

#endif
