/**
 * PsmkPacker.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPacker.hpp"

/* system headers */
#include <cstring>

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTextEdit>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#endif
#include <QClipboard>
#include <QMimeData>

/* local library headers */

/* local headers */
#include "PsmkBeatWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkNoteWidget.hpp"
#include "PsmkPatternWidget.hpp"
#include "PsmkPatternSelector.hpp"
#include "PsmkSongWidget.hpp"


#include "Trace.hpp"


PsmkPacker::PsmkPacker( QWidget *parent )
: QGroupBox( parent )
, mpMessage1( new QLabel( this ) )
, mPatternCount( 0 )
, mBeatStore()
, mHighPatternStore()
, mLowPatternStore()
, mBeatCounter()
, mVoice()
, mBeatIndex()
{
   QBoxLayout *layout = new QVBoxLayout( this );

   layout->addWidget( mpMessage1 );

   setTexts();
}


PsmkPacker::~PsmkPacker()
{
}


void PsmkPacker::setTexts()
{
   setTitle( tr("Objects in use:") );
   updateMessage();
}


void PsmkPacker::updateMessage()
{
   int bytes = 0;
   bytes += 8; // soundAttenArray
   bytes += 8; // soundTypeArray
   bytes += 4; // hihat pattern
   bytes += mPatternCount + 2; // +2 end markers
   bytes += 8 * mHighPatternStore.count();
   bytes += 8 * mLowPatternStore.count();
   bytes += 9 * mBeatStore.count();

   mpMessage1->setText( tr( "Unique High Patterns: %1/64\n"
                            "Unique Low Patterns: %2/64\n"
                            "Unique Beats: %3\n"
                            "Reduced Size: %4 Bytes"
                            ).arg( QString::number( mHighPatternStore.count() ),
                                   QString::number( mLowPatternStore.count() ),
                                   QString::number( mBeatStore.count() ),
                                   QString::number( bytes ) ) );
}


int PsmkPacker::indexOfBeat( const QByteArray &beat ) const
{
   return mBeatStore.indexOf( beat );
}


int PsmkPacker::indexOfBeat( const PsmkBeatWidget *beat ) const
{
   return indexOfBeat( beat->toBinary() );
}


int PsmkPacker::addBeat( const QByteArray &beat )
{
   if( !mBeatStore.contains( beat ) )
   {
      mBeatStore.append( beat );
   }
   int idx = indexOfBeat( beat );
   mBeatCounter.insert( idx, mBeatCounter.value( idx ) + 1 );
   return idx;
}


int PsmkPacker::addBeat( const PsmkBeatWidget *beat )
{
   return addBeat( beat->toBinary() );
}


int PsmkPacker::beatCount( const QByteArray &beat ) const
{
   return mBeatCounter.value( indexOfBeat( beat ) );
}


int PsmkPacker::beatCount( const PsmkBeatWidget *beat ) const
{
   return beatCount( beat->toBinary() );
}


int PsmkPacker::indexOfPattern( const QByteArray &pattern ) const
{
   int index = 0;
   bool isLow = false;
   switch( pattern.at(0) )
   {
   case 0:
      isLow = false;
      index = mHighPatternStore.indexOf( pattern );
      break;
   case 1:
      isLow = true;
      index = mLowPatternStore.indexOf( pattern );
      break;
   default:
      Q_ASSERT( false );
   }
   if( index >= 64 )
   {
      //! \todo send signal
      index = 63;
   }
   return index | (isLow ? 64 : 0);
}


int PsmkPacker::indexOfPattern( const PsmkPatternWidget *pattern ) const
{
   return indexOfPattern( pattern->toBinary() );
}


void PsmkPacker::clearStorage()
{
   mBeatStore.clear();
   mHighPatternStore.clear();
   mLowPatternStore.clear();
   mBeatCounter.clear();
   mVoice[0].clear();
   mVoice[1].clear();
   mPatternCount = 0;
   ::memset( &mBeatIndex, 0, sizeof(mBeatIndex) );
}


int PsmkPacker::addPattern( const QByteArray &pattern )
{
   switch( pattern.at(0) )
   {
   case 0:
      if( !mHighPatternStore.contains( pattern ) )
      {
         mHighPatternStore.append( pattern );
      }
      break;
   case 1:
      if( !mLowPatternStore.contains( pattern ) )
      {
         mLowPatternStore.append( pattern );
      }
      break;
   default:
      Q_ASSERT( false );
   }
   mPatternCount++;
   return indexOfPattern( pattern );
}


int PsmkPacker::addPattern( const PsmkPatternWidget *pattern )
{
   return addPattern( pattern->toBinary() );
}


QByteArray PsmkPacker::toSourceCode( const PsmkSongWidget *main )
{
   update( main );
   int index = 0;
   const QString &opcode( "   .byte " );
   /*! \todo make template editable */
   QStringList patternArrayH;
   QStringList patternArrayL;
   QStringList beats;

   QStringList soundTypeAttenArray( toHex(main->mpPsmkInstrumentsWidget->toBinary()) );
   int soundTypeAttenArraySize = soundTypeAttenArray.size() / 2;
   QByteArray hiHatConfig( main->mpPsmkHiHatWidget->toBinary() );

   QString tmplString(
            "\n"
            ".define TEMPODELAY %TEMPODELAY%\n"
            "\n"
            "psmkSoundTypeArray:\n"
            "%SOUNDTYPEARRAY%\n"
            "\n"
            "psmkSoundAttenArray:\n"
            "%SOUNDATTENARRAY%\n"
            "\n"
            "psmkHatPattern:\n"
            "%HATPATTERN%\n"
            "\n"
            ".define HATVOLUME %HATVOLUME%\n"
            ".define HATPITCH  %HATPITCH%\n"
            ".define HATSOUND  %HATTYPE%\n"
            "\n"
            "psmkSong1:\n"
            "%VOICE0%\n"
            "psmkSong2:\n"
            "%VOICE1%\n"
            "\n"
            "psmkHiVolPatternsLo:\n"
            "%PATTERNARRAYHLSB%\n"
            "psmkHiVolPatternsHi:\n"
            "%PATTERNARRAYHMSB%\n"
            "psmkLoVolPatternsLo:\n"
            "%PATTERNARRAYLLSB%\n"
            "psmkLoVolPatternsHi:\n"
            "%PATTERNARRAYLMSB%\n"
            "\n"
            "%BEATS%\n"
            );

   for( int i = 0; i < 0x80; ++i )
   {
      if( mBeatIndex[i][0] )
      {
         for( int b = 0; b < PsmkConfig::BeatsInPattern; ++b )
         {
            if( i < 0x40 )
            {
               patternArrayH.append( QString("beat%1").arg( mBeatIndex[i][1+b]) );
            }
            else
            {
               patternArrayL.append( QString("beat%1").arg( mBeatIndex[i][1+b]) );
            }
         }
      }
   }

   index = 0;
   foreach( const QByteArray &entry, mBeatStore )
   {
      beats.append( QString("\nbeat%1:").arg(index) );
      beats.append( toList( toBin(entry), opcode, 9 ) );
      index++;
   }

   tmplString.replace( "%TEMPODELAY%", QString::number(main->mpValueDelay->value()) );
   tmplString.replace( "%SOUNDTYPEARRAY%", toList( soundTypeAttenArray.mid(0,soundTypeAttenArraySize), opcode, 8 ) );
   tmplString.replace( "%SOUNDATTENARRAY%", toList( soundTypeAttenArray.mid(soundTypeAttenArraySize,soundTypeAttenArraySize), opcode, 8 ) );
   tmplString.replace( "%HATTYPE%", toHex(hiHatConfig.at(0)) );
   tmplString.replace( "%HATPITCH%", toHex(hiHatConfig.at(1)) );
   tmplString.replace( "%HATVOLUME%", toHex(hiHatConfig.at(2)) );
   tmplString.replace( "%HATPATTERN%", toList( toBin(hiHatConfig.mid(3)), opcode, 4 ) );
   tmplString.replace( "%VOICE0%", toList( toDec(mVoice[0]), opcode, 8 ) );
   tmplString.replace( "%VOICE1%", toList( toDec(mVoice[1]), opcode, 8 ) );
   tmplString.replace( "%PATTERNARRAYHLSB%", toList( patternArrayH, opcode, 4, "<" ) );
   tmplString.replace( "%PATTERNARRAYHMSB%", toList( patternArrayH, opcode, 4, ">" ) );
   tmplString.replace( "%PATTERNARRAYLLSB%", toList( patternArrayL, opcode, 4, "<" ) );
   tmplString.replace( "%PATTERNARRAYLMSB%", toList( patternArrayL, opcode, 4, ">" ) );
   tmplString.replace( "%BEATS%", beats.join("\n") );

   QApplication::clipboard()->setText( tmplString );

   return tmplString.toUtf8();
}


void PsmkPacker::toSongBinary( SongBinary *sb, const PsmkSongWidget *main )
{
   int i;
   update( main );
   PsmkPatternSelector *selector = main->mpPsmkPatternSelector;

   // clear out entire binary song data
   ::memset( sb, 0, sizeof(SongBinary) );

   sb->tempoDelay = (main->mpValueDelay->value() & 0xff);
   QByteArray instruments( main->mpPsmkInstrumentsWidget->toBinary() );
   for( i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      sb->soundTypeArray[i]  = instruments.at( i );
      sb->soundAttenArray[i] = instruments.at( i + PsmkConfig::InstrumentsInSong );
   }

   QByteArray hihat( main->mpPsmkHiHatWidget->toBinary() );
   sb->hatType   = hihat.at(0);
   sb->hatPitch  = hihat.at(1);
   sb->hatVolume = hihat.at(2);
   for( i = 0; i < (PsmkConfig::BeatsInHiHat / 8); ++i )
   {
      sb->hatPattern[i] = hihat.at(3+i);
   }

   sb->songSize = selector->mpVoice0Stack->count();
   for( i = 0; i < selector->mpVoice0Stack->count(); ++i )
   {
      PsmkPatternWidget *pattern0 = qobject_cast<PsmkPatternWidget*>( selector->mpVoice0Stack->widget(i) );
      PsmkPatternWidget *pattern1 = qobject_cast<PsmkPatternWidget*>( selector->mpVoice1Stack->widget(i) );
      QCheckBox         *pathihat = qobject_cast<QCheckBox*>( selector->mpHiHatStack->widget(i) );
      Q_ASSERT( pattern0 );
      Q_ASSERT( pattern1 );
      Q_ASSERT( pathihat );
      int index0 = indexOfPattern( pattern0 );
      int index1 = indexOfPattern( pattern1 );
      bool ihihat = pathihat->isChecked();
      sb->voice0[i] = index0 | (ihihat ? 0 : 0x80);
      sb->voice1[i] = index1;
   }

   sb->highBeatSize = 0;
   foreach( const QByteArray &pattern, mHighPatternStore )
   {
      for( int b = 0; b < PsmkConfig::BeatsInPattern; ++b )
      {
         int index = mBeatStore.indexOf( pattern.mid(1+9*b,9) );
         sb->highBeatIndex[sb->highBeatSize][b] = index;
      }
      sb->highBeatSize++;
   }

   sb->lowBeatSize = 0;
   foreach( const QByteArray &pattern, mLowPatternStore )
   {
      for( int b = 0; b < PsmkConfig::BeatsInPattern; ++b )
      {
         int index = mBeatStore.indexOf( pattern.mid(1+9*b,9) );
         sb->lowBeatIndex[sb->lowBeatSize][b] = index;
      }
      sb->lowBeatSize++;
   }

   sb->beatsSize = 0;
   foreach( const QByteArray &beat, mBeatStore )
   {
      for( int n = 0; n < 9; ++n )
      {
         sb->beats[sb->beatsSize][n] = beat.at(n);
      }
      sb->beatsSize++;
   }
}


QString PsmkPacker::toHex( quint8 value )
{
   QString retval( QString::number( value, 16 ) );
   retval = retval.rightJustified( 2, '0' );
   retval.prepend( "$" );
   return retval;
}


QStringList PsmkPacker::toHex( const QByteArray &value )
{
   QStringList retval;
   foreach( quint8 c, value )
   {
      retval.append( toHex(c) );
   }
   return retval;
}


QString PsmkPacker::toBin( quint8 value )
{
   QString retval( QString::number( value, 2 ) );
   retval = retval.rightJustified( 8, '0' );
   retval.prepend( "%" );
   return retval;
}


QStringList PsmkPacker::toBin( const QByteArray &value )
{
   QStringList retval;
   foreach( quint8 c, value )
   {
      retval.append( toBin(c) );
   }
   return retval;
}


QStringList PsmkPacker::toDec( const QByteArray &value )
{
   QStringList retval;
   foreach( quint8 c, value )
   {
      retval.append( QString::number(c).rightJustified( 3, ' ' ) );
   }
   return retval;
}


void PsmkPacker::update( const PsmkSongWidget *main )
{
   clearStorage();
   int pattern;
   int beat;
   if( main->mpPsmkPatternSelector->mpVoice0Stack->count() > 2 )
   {
      beat = 1;
   }
   QStackedWidget *voices[2] = { main->mpPsmkPatternSelector->mpVoice0Stack,
                                 main->mpPsmkPatternSelector->mpVoice1Stack };
   for( int j = 0; j < 2; ++j )
   {
      QStackedWidget *v = voices[j];
      Q_ASSERT( v->count() == main->mpPsmkPatternSelector->mpHiHatStack->count() );
      for( int i = 0; i < v->count(); ++i )
      {
         const PsmkPatternWidget *p = qobject_cast<PsmkPatternWidget*>( v->widget( i ) );
         Q_ASSERT( p );
         pattern = addPattern( p );
         bool hihat = false;
         if( !j )
         {
            const QCheckBox *c =  qobject_cast<QCheckBox*>( main->mpPsmkPatternSelector->mpHiHatStack->widget( i ) );
            Q_ASSERT( c );
            hihat = !c->isChecked();
         }
         mVoice[j].append( pattern | (hihat ? 0x80 : 0x00) );
         if( !mBeatIndex[pattern][0] )
         {
            for( int b = 0; b < PsmkConfig::BeatsInPattern; ++b )
            {
               beat = addBeat( p->beat( b ) );
               mBeatIndex[pattern][1+b] = beat;
            }
            mBeatIndex[pattern][0] = 1;
         }
      }
      mVoice[j].append( -1 ); // =255: end marker
   }
   updateMessage();
}


QString PsmkPacker::toList( const QStringList &list, const QString &opcode,
                            int entriesPerLine, const QString &prefix )
{
   QString retval;
   int pos = 0;
   foreach( const QString &entry, list )
   {
      if( pos >= entriesPerLine )
      {
         retval.append( "\n" );
         pos = 0;
      }
      if( pos == 0 )
      {
         retval.append( opcode );
         retval.append( prefix );
         retval.append( entry );
      }
      else
      {
         retval.append( "," );
         retval.append( prefix );
         retval.append( entry );
      }
      pos++;
   }
   return retval;
}
