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
#include <cstring>

/* Qt headers */

/* local library headers */

/* local headers */


#include <QtDebug>
#include "json.h"

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

   bool ok = true;
   Q_ASSERT( variantMap.contains( "delay" ) );
   mDelay = variantMap.value( "delay" ).toInt( &ok );
   Q_ASSERT( ok );

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


int SlocumSong::size( quint8 voice )
{
   Q_ASSERT( voice < 2 );
   return mVoice[voice].size();
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


QString SlocumSong::toHex( quint8 value )
{
   QString retval( QString::number( value, 16 ) );
   retval = retval.rightJustified( 2, '0' );
   retval.prepend( "$" );
   return retval;
}


QString SlocumSong::toBinary( quint8 value )
{
   QString retval( QString::number( value, 2 ) );
   retval = retval.rightJustified( 8, '0' );
   retval.prepend( "%" );
   return retval;
}


QByteArray SlocumSong::toSourceCode()
{
   /*! \todo make template editable */
   SlocumBarList patternsLow;
   SlocumBarList patternsHigh;
   QList<SlocumBeat> beatStore;
   QStringList voice0;
   QStringList voice1;
   QStringList patternArrayLlsb;
   QStringList patternArrayLmsb;
   QStringList patternArrayHlsb;
   QStringList patternArrayHmsb;
   QStringList beats;
   QStringList soundAttenArray;
   QStringList soundTypeArray;
   QString hatPattern;

   QString tmplString(
            "\n"
            ".define TEMPODELAY %TEMPODELAY%\n"
            "\n"
            "soundTypeArray:\n"
            "   .byte %SOUNDTYPEARRAY%\n"
            "\n"
            "soundAttenArray:\n"
            "   .byte %SOUNDATTENARRAY%\n"
            "\n"
            "hatPattern:\n"
            "   .byte %HATPATTERN%\n"
            "\n"
            ".define HATSTART  %HATSTART%\n"
            ".define HATVOLUME %HATVOLUME%\n"
            ".define HATPITCH  %HATPITCH%\n"
            ".define HATSOUND  %HATSOUND%\n"
            "\n"
            "song1:\n"
            "%VOICE0%\n"
            "song2:\n"
            "%VOICE1%\n"
            "\n"
            "patternArrayHlsb:\n"
            "%PATTERNARRAYHLSB%\n"
            "patternArrayHmsb:\n"
            "%PATTERNARRAYHMSB%\n"
            "patternArrayLlsb:\n"
            "%PATTERNARRAYLLSB%\n"
            "patternArrayLmsb:\n"
            "%PATTERNARRAYLMSB%\n"
            "\n"
            "%BEATS%\n"
            );

   sanitize();

   for( int i = 0; i < SlocumSound::size(); ++i )
   {
      soundAttenArray.append( toHex( mSound.atten[i] ) );
      soundTypeArray.append( toHex( mSound.type[i] ) );
   }

   for( int i = 0; i < SlocumHiHat::size(); ++i )
   {
      if( (i % 8) == 0 )
      {
         hatPattern.append( '%' );
      }
      hatPattern.append( mHiHat.pattern[i] ? '1' : '0' );
      if( (i % 8) == 7 )
      {
         hatPattern.append( ',' );
      }
   }
   Q_ASSERT( hatPattern.endsWith(',') );
   hatPattern.chop(1); // cut off last ','

   SlocumBarList voiceList;
   voiceList.append( mVoice[0] );
   voiceList.append( mVoice[1] );

   foreach( const SlocumBar &bar, voiceList )
   {
      SlocumBarList &list = bar.isLow ? patternsLow : patternsHigh;
      if( !list.contains( bar ) )
      {
         list.append( bar );
      }
      for( int i = 0; i < SlocumBar::size(); ++i )
      {
         if( !beatStore.contains( bar.beat[i] ) )
         {
            beatStore.append( bar.beat[i] );
         }
      }
   }
   foreach( const SlocumBar &bar, mVoice[0] )
   {
      int index = bar.isLow ? patternsLow.indexOf( bar ) | 128 : patternsHigh.indexOf( bar );
      voice0.append( QString("   .byte %1 ; %2").arg( QString::number(index), bar.name ) );
   }
   foreach( const SlocumBar &bar, mVoice[1] )
   {
      int index = bar.isLow ? patternsLow.indexOf( bar ) | 128 : patternsHigh.indexOf( bar );
      voice1.append( QString("   .byte %1 ; %2").arg( QString::number(index), bar.name ) );
   }

   foreach( const SlocumBar &bar, patternsHigh )
   {
      QStringList msb;
      QStringList lsb;
      for( int i = 0; i < SlocumBar::size(); ++i )
      {
         int index = beatStore.indexOf( bar.beat[i] );
         lsb.append( QString("<beat%1").arg(index) );
         msb.append( QString(">beat%1").arg(index) );
      }

      patternArrayHlsb.append( QString("   .byte %1 ; %2").arg(lsb.join(","), bar.name) );
      patternArrayHmsb.append( QString("   .byte %1 ; %2").arg(msb.join(","), bar.name) );
   }
   foreach( const SlocumBar &bar, patternsLow )
   {
      QStringList msb;
      QStringList lsb;
      for( int i = 0; i < SlocumBar::size(); ++i )
      {
         int index = beatStore.indexOf( bar.beat[i] );
         lsb.append( QString("<beat%1").arg(index) );
         msb.append( QString(">beat%1").arg(index) );
      }

      patternArrayLlsb.append( QString("   .byte %1 ; %2").arg(lsb.join(","), bar.name) );
      patternArrayLmsb.append( QString("   .byte %1 ; %2").arg(msb.join(","), bar.name) );
   }

   for( int i = 0; i < beatStore.size(); ++i )
   {
      beats.append( QString("beat%1: ; %2").arg(QString::number(i),beatStore[i].name) );
      const SlocumBeat &beat = beatStore[i];
      QStringList notes;
      quint8 accent = 0;
      for( int n = 0; n < SlocumBeat::size(); ++n )
      {
         notes.append( toBinary( (beat.note[n].sound << 5) | beat.note[n].pitch ) );
         accent <<= 1;
         accent |= beat.note[n].accent ? 1 : 0;
      }
      notes.append( toBinary( accent ) );
      beats.append( QString("   .byte %1").arg(notes.join(",")) );
   }

   tmplString.replace( "%TEMPODELAY%", QString::number(mDelay) );
   tmplString.replace( "%SOUNDTYPEARRAY%", soundTypeArray.join(",") );
   tmplString.replace( "%SOUNDATTENARRAY%", soundAttenArray.join(",") );
   tmplString.replace( "%HATPATTERN%", hatPattern );
   tmplString.replace( "%HATSTART%", QString::number(mHiHat.start) );
   tmplString.replace( "%HATVOLUME%", QString::number(mHiHat.volume) );
   tmplString.replace( "%HATPITCH%", QString::number(mHiHat.pitch) );
   tmplString.replace( "%HATSOUND%", QString::number(mHiHat.sound) );
   tmplString.replace( "%VOICE0%", voice0.join("\n") );
   tmplString.replace( "%VOICE1%", voice1.join("\n") );
   tmplString.replace( "%PATTERNARRAYHLSB%", patternArrayHlsb.join("\n") );
   tmplString.replace( "%PATTERNARRAYHMSB%", patternArrayHmsb.join("\n") );
   tmplString.replace( "%PATTERNARRAYLLSB%", patternArrayLlsb.join("\n") );
   tmplString.replace( "%PATTERNARRAYLMSB%", patternArrayLmsb.join("\n") );
   tmplString.replace( "%BEATS%", beats.join("\n") );

   return tmplString.toUtf8();
}


void SlocumSong::toSongBinary( SongBinary *songBinary )
{
   int i;

   // clear out entire binary song data
   ::memset( songBinary, 0, sizeof(SongBinary) );

   songBinary->tempoDelay = (quint8)(mDelay & 0xff);
   for( i = 0; i < SlocumSound::size(); ++i )
   {
      songBinary->soundAttenArray[i] = mSound.atten[i];
      songBinary->soundTypeArray[i]  = mSound.type[i];
   }

   for( i = 0; i < SlocumHiHat::size(); ++i )
   {
      if( mHiHat.pattern[i] )
      {
         songBinary->hatPattern[i/8] |= 1 << (7 - i % 8);
      }
   }
   songBinary->hatPitch  = mHiHat.pitch;
   songBinary->hatSound  = mHiHat.sound;
   songBinary->hatStart  = mHiHat.start;
   songBinary->hatVolume = mHiHat.volume;

   SlocumBar muted( QString("muted") );
   muted.isLow = true; // usually less used
   SlocumBarList voiceList;
   voiceList.append( muted );
   voiceList.append( mVoice[0] );
   voiceList.append( mVoice[1] );

   QList<SlocumBeat> beatStore;
   SlocumBarList patternsLow;
   SlocumBarList patternsHigh;
   foreach( const SlocumBar &bar, voiceList )
   {
      SlocumBarList &list = bar.isLow ? patternsLow : patternsHigh;
      if( !list.contains( bar ) )
      {
         list.append( bar );
      }
      for( int i = 0; i < SlocumBar::size(); ++i )
      {
         if( !beatStore.contains( bar.beat[i] ) )
         {
            beatStore.append( bar.beat[i] );
         }
      }
   }

   int voiceSize0 = 0;
   int voiceSize1 = 0;
   int index;
   foreach( const SlocumBar &bar, mVoice[0] )
   {
      index = bar.isLow ? patternsLow.indexOf( bar ) | 128 : patternsHigh.indexOf( bar );
      songBinary->voice0[voiceSize0++] = index;
   }
   foreach( const SlocumBar &bar, mVoice[1] )
   {
      index = bar.isLow ? patternsLow.indexOf( bar ) | 128 : patternsHigh.indexOf( bar );
      songBinary->voice1[voiceSize1++] = index;
   }
   index = muted.isLow ? patternsLow.indexOf( muted ) | 128 : patternsHigh.indexOf( muted );
   while( voiceSize0 < voiceSize1 )
   {
      songBinary->voice0[voiceSize0++] = index;
   }
   while( voiceSize1 < voiceSize0 )
   {
      songBinary->voice1[voiceSize1++] = index;
   }
   songBinary->songSize = voiceSize0;

   songBinary->highBeatSize = 0;
   foreach( const SlocumBar &bar, patternsHigh )
   {
      for( int b = 0; b < SlocumBar::size(); ++b )
      {
         int index = beatStore.indexOf( bar.beat[b] );
         songBinary->highBeatIndex[songBinary->highBeatSize][b] = index;
      }
      songBinary->highBeatSize++;
   }

   songBinary->lowBeatSize = 0;
   foreach( const SlocumBar &bar, patternsLow )
   {
      for( int b = 0; b < SlocumBar::size(); ++b )
      {
         int index = beatStore.indexOf( bar.beat[b] );
         songBinary->lowBeatIndex[songBinary->lowBeatSize][b] = index;
      }
      songBinary->lowBeatSize++;
   }

   songBinary->beatsSize = 0;
   foreach( const SlocumBeat &beat, beatStore )
   {
      quint8 accent = 0;
      for( int n = 0; n < SlocumBeat::size(); ++n )
      {
         songBinary->beats[songBinary->beatsSize][n] = (beat.note[n].sound << 5) | beat.note[n].pitch;
         accent <<= 1;
         accent |= beat.note[n].accent ? 1 : 0;
      }
      songBinary->beats[songBinary->beatsSize++][SlocumBeat::size()] = accent;
   }
}
