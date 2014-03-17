/*
 * PlayerEmulation.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "cpu6502.hpp"
#include "PlayerEmulation.hpp"
#include "SoundSDL2.hpp"
#include "TIASnd.hpp"

/* system headers */

/* Qt headers */
#include <QFile>
#include <QTimer>

/* local library headers */
#include <PlayerConfig.hpp>

/* local headers */


#include <QDebug>


PlayerEmulation::PlayerEmulation( QObject *parent )
: QThread( parent )
, mpFrameTimer( 0 )
, mpTIA( new TIASound( 31113 ) )
, mpSoundSDL( new SoundSDL2( mpTIA ) )
, mpPlayerConfig( new PlayerConfig( mpTIA ) )
, mp6502( new Cpu6502( mpPlayerConfig ) )
, mPlayerData()
, mCurrentPattern( 0 )
, mCurrentNote( 0 )
, mLoopEnabled( false )
{
   moveToThread( this );
}


PlayerEmulation::~PlayerEmulation()
{
   stopSong();
   delete mp6502;
   delete mpPlayerConfig;
   delete mpTIA;
   if( mpSoundSDL )
   {
      delete mpSoundSDL;
   }
}


void PlayerEmulation::songToMemory( SongBinary *songBinary )
{
   /*
      ; patterndata from $0200-$1400 (=$e200-$f400)

      song1           = $f400
      song2           = $f500
      patternArrayHlsb= $f600
      patternArrayHmsb= $f700
      patternArrayLlsb= $f800
      patternArrayLmsb= $f900

      soundTypeArray  = $fde0
      soundAttenArray = $fde8
      hatPattern      = $fdf0
      HATSOUND        = $fdf4
      HATPITCH        = $fdf5
      HATVOLUME       = $fdf6
      TEMPODELAY      = $fdf7
   */
   // clean out song memory
   for( ADDRESS a = 0xe200; a < 0xfe00; ++a )
   {
      mpPlayerConfig->poke( a, 0 );
   }

   for( int i = 0; i < mPlayerData.size(); ++i )
   {
      mpPlayerConfig->poke( 0x10000 - mPlayerData.size() + i, mPlayerData.at(i) );
   }

   for( int i = 0; i < 8; ++i )
   {
      mpPlayerConfig->poke( 0xfde0 + i, songBinary->soundTypeArray[i] );
      mpPlayerConfig->poke( 0xfde8 + i, songBinary->soundAttenArray[i] );
   }
   for( int i = 0; i < 4; ++i )
   {
      mpPlayerConfig->poke( 0xfdf0 + i, songBinary->hatPattern[i] );
   }
   mpPlayerConfig->poke( 0xfdf4, songBinary->hatType );
   mpPlayerConfig->poke( 0xfdf5, songBinary->hatPitch );
   mpPlayerConfig->poke( 0xfdf6, songBinary->hatVolume );
   mpPlayerConfig->poke( 0xfdf7, songBinary->tempoDelay );

   for( int i = 0; i < songBinary->songSize; ++i )
   {
      mpPlayerConfig->poke( 0xf400 + i, songBinary->voice0[i] );
      mpPlayerConfig->poke( 0xf500 + i, songBinary->voice1[i] );
   }
   mpPlayerConfig->poke( 0xf400 + songBinary->songSize, 255 );
   mpPlayerConfig->poke( 0xf500 + songBinary->songSize, 255 );

   for( int i = 0; i < songBinary->highBeatSize; ++i )
   {
      for( int j = 0; j < 4; ++j )
      {
         ADDRESS a = 0x0200 + 9 * songBinary->highBeatIndex[i][j];
         mpPlayerConfig->poke( 0xf600 + i * 4 + j, a & 0xff );
         mpPlayerConfig->poke( 0xf700 + i * 4 + j, a >> 8   );
      }
   }

   for( int i = 0; i < songBinary->lowBeatSize; ++i )
   {
      for( int j = 0; j < 4; ++j )
      {
         ADDRESS a = 0x0200 + 9 * songBinary->lowBeatIndex[i][j];
         mpPlayerConfig->poke( 0xf800 + i * 4 + j, a & 0xff );
         mpPlayerConfig->poke( 0xf900 + i * 4 + j, a >> 8   );
      }
   }

   for( int i = 0; i < songBinary->beatsSize; ++i )
   {
      for( int j = 0; j < 9; ++j )
      {
         mpPlayerConfig->poke( 0x0200 + 9 * i + j, songBinary->beats[i][j] );
      }
   }
}


void PlayerEmulation::loadPlayer( const QString &fileName )
{
   QFile file( fileName );
   if( file.open( QIODevice::ReadOnly ) )
   {
      mPlayerData = file.readAll();
      file.close();
   }
}


void PlayerEmulation::startSong()
{
   if( !mpFrameTimer )
   {
      // timer must be created after move to thread
      mpFrameTimer = new QTimer( this );
      connect( mpFrameTimer, SIGNAL(timeout()),
               this, SLOT(runFrame()) );
   }

   mpFrameTimer->setInterval( 1000 / 50 );
   mpFrameTimer->setSingleShot( false );

   mpSoundSDL->open();
   mp6502->reset();
   if( mCurrentPattern < 255 )
   {
      mpPlayerConfig->poke( 0x01, mCurrentPattern & 0xff );
   }
   mpSoundSDL->mute( false );
   mpFrameTimer->start();
}


void PlayerEmulation::stopSong()
{
   if( mpFrameTimer )
   {
      mpFrameTimer->stop();
   }
   if( mpSoundSDL )
   {
      mpSoundSDL->mute( true );
      mpSoundSDL->close();
   }
}


void PlayerEmulation::runFrame()
{
   while( mpPlayerConfig->keepOnRunning() )
   {
      mp6502->step();
   }

   if( mLoopEnabled )
   {
      mpPlayerConfig->poke( 0x81, mCurrentPattern & 0xff );
   }

   int pattern = mpPlayerConfig->peek(0x81);
   int note    = mpPlayerConfig->peek(0x82);

   if( (pattern != mCurrentPattern) || (note != mCurrentNote) )
   {
       mCurrentPattern = pattern;
       mCurrentNote    = note;
       emit stateUpdate( mCurrentPattern, mCurrentNote );
   }
}


void PlayerEmulation::setCurrentPattern( int pattern )
{
   mCurrentPattern = pattern;
   mpPlayerConfig->poke( 0x01, mCurrentPattern & 0xff );
}


void PlayerEmulation::setLooping( bool enabled )
{
   mLoopEnabled = enabled;
}
