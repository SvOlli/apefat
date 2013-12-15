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
, mVoice()
, mSongType( "slocum0" )
{
   SlocumBar dummy;
   mVoice[0].append( dummy );
   mVoice[1].append( dummy );
}


SlocumSong::SlocumSong( const QVariantMap &variantMap )
: mName()
, mDelay( 0 )
, mSound()
, mHiHat()
, mVoice()
, mSongType( "slocum0" )
{
   fromVariantMap( variantMap );
   if( mVoice[0].size() < 1 )
   {
      mVoice[0].append( SlocumBar() );
   }
   if( mVoice[1].size() < 1 )
   {
      mVoice[1].append( SlocumBar() );
   }
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


SlocumBarList *SlocumSong::voice( int i )
{
   Q_ASSERT( (i>=0) && (i<2) );
   return &mVoice[i];
}


QVariantMap SlocumSong::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList voices;

   for( int c = 0; c < 2; ++c )
   {
      QVariantList voice;
      for( int i = 0; i < mVoice[c].size(); ++i )
      {
         voice.append( mVoice[c].at(i).toVariantMap() );
      }
      voices.append( QVariant(voice) );
   }

   variantMap.insert( "name", mName );
   variantMap.insert( "delay", mDelay );
   variantMap.insert( "type", mSongType );
   variantMap.insert( "sound", mSound.toVariantMap() );
   variantMap.insert( "hihat", mHiHat.toVariantMap() );
   variantMap.insert( "voices", voices );

   return variantMap;
}


bool SlocumSong::fromVariantMap( const QVariantMap &variantMap )
{
   //! \todo switch from asserts to return value
   Q_ASSERT( variantMap.contains( "name" ) );
   mName = variantMap.value( "name" ).toString();

#if 0
   bool ok = true;
   Q_ASSERT( variantMap.contains( "delay" ) );
   mDelay = variantMap.value( "delay" ).toInt( ok );
   Q_ASSERT( ok );
#endif

   Q_ASSERT( variantMap.contains( "sound" ) );
   mSound.fromVariantMap( variantMap.value( "sound" ).toMap() );

   Q_ASSERT( variantMap.contains( "hihat" ) );
   mHiHat.fromVariantMap( variantMap.value( "hihat" ).toMap() );

   Q_ASSERT( variantMap.contains( "voices" ) );
   QVariantList voices( variantMap.value( "voices" ).toList() );

#if 0
   Q_ASSERT( variantMap.contains( "type" ) );
   Q_ASSERT( variantMap.value( "type" ).toString() == mSongType );
#endif

   mVoice[0].clear();
   mVoice[1].clear();
   SlocumBar slocumBar;

   Q_ASSERT( voices.size() == 2 );
   for( int c = 0; c < voices.size(); ++c )
   {
      Q_ASSERT( voices.at(c).type() == QVariant::List );
      QVariantList voice( voices.at(c).toList() );
      for( int i = 0; i < voice.size(); ++i )
      {
         slocumBar.fromVariantMap( voice.at(i).toMap() );
         mVoice[c].append( slocumBar );
      }
   }

   if( mVoice[0].size() < 1 )
   {
      mVoice[0].append( SlocumBar() );
   }
   if( mVoice[1].size() < 1 )
   {
      mVoice[1].append( SlocumBar() );
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
      foreach( const SlocumBar &bar, mVoice[c] )
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
   if( !mVoice[0].size() )
   {
      mVoice[0].append( SlocumBar() );
   }
   if( !mVoice[1].size() )
   {
      mVoice[1].append( SlocumBar() );
   }

   if( mVoice[0].size() == mVoice[1].size() )
   {
      return;
   }

   QList<SlocumBar> *pShortVoice = &mVoice[0];
   QList<SlocumBar> *pLargeVoice = &mVoice[1];
   if( pShortVoice->size() > pLargeVoice->size() )
   {
      pShortVoice = &mVoice[1];
      pLargeVoice = &mVoice[0];
   }
   /* first run: remove empty patterns from larger voice */
   while( (pLargeVoice->last().isEmpty()) &&
          (pLargeVoice->size() > pShortVoice->size()) )
   {
      pLargeVoice->removeLast();
   }
   /* second run: pad shorter voice with empty patterns */
   while( (pLargeVoice->size() > pShortVoice->size()) )
   {
      pShortVoice->append( SlocumBar() );
   }

   Q_ASSERT( mVoice[0].size() == mVoice[1].size() );
}


int SlocumSong::size( quint8 voice )
{
   Q_ASSERT( voice < 2 );
   return mVoice[voice].size();
}
