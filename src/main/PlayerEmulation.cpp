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
: QObject( parent )
, mpFrameTimer( new QTimer( this ) )
, mpTIA( new TIASound( 31113 ) )
, mpSoundSDL( new SoundSDL2( mpTIA ) )
, mpPlayerConfig( new PlayerConfig( mpTIA ) )
, mp6502( new Cpu6502( mpPlayerConfig ) )
, mpSlocumSong( 0 )
, mSongBinary()
, mPlayerData()
, mCurrentBar( 0 )
, mLoopEnabled( false )
{
   mpFrameTimer->setInterval( 1000 / 50 );
   mpFrameTimer->setSingleShot( false );
   mpSoundSDL->open();
   mpSoundSDL->mute( true );

   connect( mpFrameTimer, SIGNAL(timeout()),
            this, SLOT(runFrame()) );
}


PlayerEmulation::~PlayerEmulation()
{
   mpSoundSDL->close();
   delete mp6502;
   delete mpPlayerConfig;
   delete mpTIA;
   delete mpSoundSDL;
}


void PlayerEmulation::setSong( SlocumSong *song, SlocumBar *bar )
{
   mpSlocumSong = song;
}


void PlayerEmulation::songToMemory()
{
   mpSlocumSong->toSongBinary( &mSongBinary );
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
      HATSTART        = $fdf4
      HATVOLUME       = $fdf5
      HATPITCH        = $fdf6
      HATSOUND        = $fdf7
      TEMPODELAY      = $fdf8
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
      mpPlayerConfig->poke( 0xfde0 + i, mSongBinary.soundTypeArray[i] );
      mpPlayerConfig->poke( 0xfde8 + i, mSongBinary.soundAttenArray[i] );
   }
   for( int i = 0; i < 4; ++i )
   {
      mpPlayerConfig->poke( 0xfdf0 + i, mSongBinary.hatPattern[i] );
   }
   mpPlayerConfig->poke( 0xfdf4, mSongBinary.hatStart );
   mpPlayerConfig->poke( 0xfdf5, mSongBinary.hatVolume );
   mpPlayerConfig->poke( 0xfdf6, mSongBinary.hatPitch );
   mpPlayerConfig->poke( 0xfdf7, mSongBinary.hatSound );
   mpPlayerConfig->poke( 0xfdf8, mSongBinary.tempoDelay );

   for( int i = 0; i < mSongBinary.songSize; ++i )
   {
      mpPlayerConfig->poke( 0xf400 + i, mSongBinary.voice0[i] );
      mpPlayerConfig->poke( 0xf500 + i, mSongBinary.voice1[i] );
   }
   mpPlayerConfig->poke( 0xf400 + mSongBinary.songSize, 255 );
   mpPlayerConfig->poke( 0xf500 + mSongBinary.songSize, 255 );

   for( int i = 0; i < mSongBinary.highBeatSize; ++i )
   {
      for( int j = 0; j < 4; ++j )
      {
         ADDRESS a = 0x0200 + 9 * mSongBinary.highBeatIndex[i][j];
         mpPlayerConfig->poke( 0xf600 + i * 4 + j, a & 0xff );
         mpPlayerConfig->poke( 0xf700 + i * 4 + j, a >> 8   );
      }
   }

   for( int i = 0; i < mSongBinary.lowBeatSize; ++i )
   {
      for( int j = 0; j < 4; ++j )
      {
         ADDRESS a = 0x0200 + 9 * mSongBinary.lowBeatIndex[i][j];
         mpPlayerConfig->poke( 0xf800 + i * 4 + j, a & 0xff );
         mpPlayerConfig->poke( 0xf900 + i * 4 + j, a >> 8   );
      }
   }

   for( int i = 0; i < mSongBinary.beatsSize; ++i )
   {
      for( int j = 0; j < 9; ++j )
      {
         mpPlayerConfig->poke( 0x0200 + 9 * i + j, mSongBinary.beats[i][j] );
      }
   }

   mpPlayerConfig->poke( 0x01, 0xff );
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


void PlayerEmulation::start()
{
   mp6502->reset();
   songToMemory();
   if( mCurrentBar < 255 )
   {
      mpPlayerConfig->poke( 0x01, mCurrentBar & 0xff );
   }
   mpSoundSDL->mute( false );
   mpFrameTimer->start();
}


void PlayerEmulation::stop()
{
   mpSoundSDL->mute( true );
   mpFrameTimer->stop();
}


void PlayerEmulation::runFrame()
{
   while( mpPlayerConfig->keepOnRunning() )
   {
      mp6502->step();
   }
   QString stateMsg;
   stateMsg.sprintf( "<pre>%02x:%02x:%02x:%02x %x:%02x:%x %x:%02x:%x</pre>",
                     mpPlayerConfig->peek(0x80),
                     mpPlayerConfig->peek(0x81),
                     mpPlayerConfig->peek(0x82),
                     mpPlayerConfig->peek(0x83),
                     mpPlayerConfig->peek(0x15),
                     mpPlayerConfig->peek(0x17),
                     mpPlayerConfig->peek(0x19),
                     mpPlayerConfig->peek(0x16),
                     mpPlayerConfig->peek(0x18),
                     mpPlayerConfig->peek(0x1a) );

   int bar = mpPlayerConfig->peek(0x81);
   if( bar != mCurrentBar )
   {
      if( mLoopEnabled )
      {
         mpPlayerConfig->poke( 0x81, mCurrentBar & 0xff );
      }
      else
      {
         mCurrentBar = bar;
         emit currentBar( mCurrentBar );
      }
   }
   emit state( stateMsg );

   //mpPlayerConfig->dumpMem();
}


void PlayerEmulation::setCurrentBar( int bar )
{
   mCurrentBar = bar;
   mpPlayerConfig->poke( 0x01, mCurrentBar & 0xff );
}


void PlayerEmulation::setLooping( bool enabled )
{
   mLoopEnabled = enabled;
}
