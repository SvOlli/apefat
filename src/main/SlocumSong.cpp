/**
 * SlocumSong.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumSong.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


#include <QtDebug>

SlocumSong::SlocumSong( const QString &setname )
: mName( setname )
, mDelay( 0 )
, mSound( mName )
, mHiHat( mName )
, mChannel()
, mSongType( "slocum0" )
{
   SlocumBar dummy;
   mChannel[0].append( dummy );
   mChannel[1].append( dummy );
}


SlocumSong::SlocumSong( const QVariantMap &variantMap )
: mName()
, mDelay( 0 )
, mSound()
, mHiHat()
, mChannel()
, mSongType( "slocum0" )
{
   fromVariantMap( variantMap );
}


SlocumSong::~SlocumSong()
{
}


QString *SlocumSong::name()
{
   return &mName;
}


SlocumSound *SlocumSong::sound()
{
   return &mSound;
}


const SlocumSound *SlocumSong::sound() const
{
   return &mSound;
}


SlocumHiHat *SlocumSong::hihat()
{
   return &mHiHat;
}


QVariantMap SlocumSong::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList channels;

   Q_ASSERT( mChannel[0].size() == mChannel[1].size() );

   for( int i = 0; i < mChannel[0].size(); ++i )
   {
      QVariantMap entry;
      QString channelPattern( "channel%1" );
      for( int c = 0; c < 2; ++c )
      {
         entry.insert( channelPattern.arg(c), mChannel[c].at(i).toVariantMap() );
      }
      channels.append( entry );
   }

   variantMap.insert( "name", mName );
   variantMap.insert( "type", mSongType );
   variantMap.insert( "sound", mSound.toVariantMap() );
   variantMap.insert( "hihat", mHiHat.toVariantMap() );
   variantMap.insert( "channels", channels );

   return variantMap;
}


bool SlocumSong::fromVariantMap( const QVariantMap &variantMap )
{
   //! \todo switch from asserts to return value
   Q_ASSERT( variantMap.contains( "name" ) );
   mName = variantMap.value( "name" ).toString();

   Q_ASSERT( variantMap.contains( "sound" ) );
   mSound.fromVariantMap( variantMap.value( "sound" ).toMap() );

   Q_ASSERT( variantMap.contains( "hihat" ) );
   mHiHat.fromVariantMap( variantMap.value( "hihat" ).toMap() );

   Q_ASSERT( variantMap.contains( "channels" ) );
   QVariantList channels( variantMap.value( "channels" ).toList() );

#if 0
   Q_ASSERT( variantMap.contains( "type" ) );
   Q_ASSERT( variantMap.value( "type" ).toString() == mSongType );
#endif

   mChannel[0].clear();
   mChannel[1].clear();
   SlocumBar slocumBar;
   QString channelPattern( "channel%1" );
   foreach( const QVariant &entryMap, channels )
   {
      for( int c = 0; c < 2; ++c )
      {
         QVariantMap entry( entryMap.toMap() );
         Q_ASSERT( entry.contains( channelPattern.arg(c) ) );
         slocumBar.fromVariantMap( entry.value( channelPattern.arg(c) ).toMap() );
         mChannel[c].append( slocumBar );
      }
   }

   return true;
}


int SlocumSong::delay()
{
   return mDelay;
}


void SlocumSong::setDelay( int delay )
{
   mDelay = delay;
}


int SlocumSong::countUniqueBars( bool lowVolume )
{
   QList<SlocumBar> storage;

   for( int c = 0; c < 2; ++c )
   {
      foreach( const SlocumBar &bar, mChannel[c] )
      {
         // bar is patched, so the name is not considered in operator==
         // alternative: do not use const-ref and set bar.name=""
         if( (bar.isLow == lowVolume) && (!storage.contains(bar)) )
         {
            storage.append( bar );
         }
      }
   }

   return storage.count();
}


void SlocumSong::sanitize()
{
   if( mChannel[0].size() == mChannel[1].size() )
   {
      return;
   }

   QList<SlocumBar> *pShortChannel = &mChannel[0];
   QList<SlocumBar> *pLargeChannel = &mChannel[1];
   if( pShortChannel->size() > pLargeChannel->size() )
   {
      pShortChannel = &mChannel[1];
      pLargeChannel = &mChannel[0];
   }
   /* first run: remove empty patterns from larger channel */
   while( (pLargeChannel->last().isEmpty()) &&
          (pLargeChannel->size() > pShortChannel->size()) )
   {
      pLargeChannel->removeLast();
   }
   /* second run: pad shorter channel with empty patterns */
   while( (pLargeChannel->size() > pShortChannel->size()) )
   {
      pShortChannel->append( SlocumBar() );
   }

   Q_ASSERT( mChannel[0].size() == mChannel[1].size() );
}


SlocumBar *SlocumSong::getBar( quint8 channel, quint8 index )
{
   Q_ASSERT( channel < 2 );
   return &(mChannel[channel][index]);
}


bool SlocumSong::insertBar( quint8 channel, quint8 index, const SlocumBar &bar )
{
   Q_ASSERT( channel < 2 );
   Q_ASSERT( index <= mChannel[channel].size() );
   mChannel[channel].insert( index, bar );
   sanitize();
   return true;
}


void SlocumSong::deleteBar( quint8 channel, quint8 index )
{
   Q_ASSERT( channel < 2 );
   Q_ASSERT( index < mChannel[channel].size() );
   mChannel[channel].removeAt( index );
   sanitize();
}


bool SlocumSong::insertBars( quint8 index )
{
   Q_ASSERT( index <= size() );
   if( size() > 255 )
   {
      return false;
   }
   for( int c = 0; c < 2; ++c )
   {
      mChannel[c].insert( index, SlocumBar() );
   }
   sanitize();
   return true;
}


void SlocumSong::deleteBars( quint8 index )
{
   Q_ASSERT( index < size() );
   for( int c = 0; c < 2; ++c )
   {
      mChannel[c].removeAt( index );
   }
}


void SlocumSong::moveBar( quint8 fromChannel, quint8 toChannel,
                          quint8 fromIndex,   quint8 toIndex )
{
   Q_ASSERT( fromChannel < 2 );
   Q_ASSERT( toChannel < 2 );
   Q_ASSERT( fromIndex < mChannel[fromChannel].size() );
   Q_ASSERT( toIndex <= mChannel[toChannel].size() );

   mChannel[toChannel].insert( toIndex, mChannel[fromChannel].takeAt( fromIndex ) );
   sanitize();
}


int SlocumSong::size()
{
   Q_ASSERT( mChannel[0].size() == mChannel[1].size() );
   return mChannel[0].size();
}


#if 0
void SlocumSong::setDefaults()
{
   mSound.type[0] = 4;
   mSound.type[1] = 6;
   mSound.type[2] = 7;
   mSound.type[3] = 8;
   mSound.type[4] = 15;
   mSound.type[5] = 12;
   mSound.type[6] = 1;
   mSound.type[7] = 14;

   mSound.atten[0] = 8;
   mSound.atten[1] = 0;
   mSound.atten[2] = 5;
   mSound.atten[3] = 9;
   mSound.atten[4] = 0;
   mSound.atten[5] = 6;
   mSound.atten[6] = 4;
   mSound.atten[7] = 0;

   mHiHat.start  = 0;
   mHiHat.sound  = 8;
   mHiHat.pitch  = 0;
   mHiHat.volume = 7;

   for( int i = 0; i < mHiHat.size(); ++i )
   {
      mHiHat.pattern[i] = false;
   }
   mHiHat.pattern[0] = true;
   mHiHat.pattern[4] = true;
   mHiHat.pattern[8] = true;
   mHiHat.pattern[12] = true;
   mHiHat.pattern[16] = true;
   mHiHat.pattern[20] = true;
   mHiHat.pattern[24] = true;
   mHiHat.pattern[28] = true;
   mHiHat.pattern[30] = true;

   mChannel[0].clear();
   mChannel[0].append( SlocumBar() );
   mChannel[1].clear();
   mChannel[1].append( SlocumBar() );
}
#endif
