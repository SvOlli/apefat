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
#include "SlocumSong.hpp"

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

    \param song
    \param bar
   */
   void setSong( SlocumSong *song, SlocumBar *bar = 0 );
   /*!
    \brief \todo

    \param fileName
   */
   void loadPlayer( const QString &fileName );
   /*!
    \brief \todo

    \param bar bar to play (255 = song)
   */
   void start( unsigned char bar = 255, bool loop = false );
   /*!
    \brief \todo

   */
   void stop();
   /*!
    \brief \todo

   */
   void runFrame();

signals:
   /*!
    \brief \todo

   */
   void state( const QString &msg );

private:
   Q_DISABLE_COPY( PlayerEmulation )

   /*!
    \brief \todo

   */
   void songToMemory();

   QTimer            *mpFrameTimer; /*!< \brief \todo TODO */
   TIASound          *mpTIA; /*!< \brief \todo TODO */
   SoundSDL2         *mpSoundSDL; /*!< \brief \todo TODO */
   PlayerConfig      *mpPlayerConfig; /*!< \brief \todo TODO */
   Cpu6502           *mp6502; /*!< \brief \todo TODO */
   SlocumSong        *mpSlocumSong; /*!< \brief \todo TODO */
   SongBinary        mSongBinary; /*!< \brief \todo TODO */
   QByteArray        mPlayerData; /*!< \brief \todo TODO */
};

#endif
