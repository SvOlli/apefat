/**
 * SoundComboBox.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SoundComboBox.hpp"

/* system headers */

/* Qt headers */
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSpinBox>

/* local library headers */
#include <Tones.h>

/* local headers */
#include "SlocumSong.hpp"

#include <QtDebug>


SoundComboBox::SoundComboBox( QWidget *parent )
: QComboBox( parent )
, mpSlocumSong( 0 )
{
   populate();
   connect( this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(indexToValue(int)) );
}


SoundComboBox::SoundComboBox( const SlocumSong *slocumSong, QWidget *parent )
: QComboBox( parent )
, mpSlocumSong( slocumSong )
{
   populate();
   connect( this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(indexToValue(int)) );
}


SoundComboBox::~SoundComboBox()
{
}


void SoundComboBox::populate()
{
   QComboBox::setMaxVisibleItems( TONES_SIZE );
   int savedIndex = QComboBox::currentIndex();
   QComboBox::clear();
   if( !mpSlocumSong )
   {
      // no song connected
      for( int i = 0; i < TONES_SIZE; ++i )
      {
         QString name( "%1:%2" );
         QString num( QString::number(i).rightJustified(2,'0') );
         name = name.arg( num, Tones_names[i] );
         if( name.endsWith( '*' ) )
         {
            continue;
         }
         QComboBox::addItem( name, i );
      }
   }
   else
   {
      const SlocumSound *sound = mpSlocumSong->sound();
      QComboBox::addItem( "--:no sound", 255 );
      QString name( "%1:%2" );
      for( int i = 0; i < SlocumSound::size(); ++i )
      {
         QComboBox::addItem( name.arg( QString::number(i),
                                       Tones_names[sound->type[i]] ),
                             i );
      }
   }
   QComboBox::setCurrentIndex( savedIndex );
}


quint8 SoundComboBox::value()
{
   bool ok;
   quint8 value = itemData( currentIndex() ).toInt( &ok );
   Q_ASSERT( ok );
   return value;
}


void SoundComboBox::indexToValue( int index )
{
   quint8 value = 255;
   if( index >= 0 )
   {
      bool ok;
      value = itemData( index ).toUInt( &ok );
      Q_ASSERT( ok );
   }
   emit valueChanged( value );
}


void SoundComboBox::setValue( quint8 value )
{
   bool ok;
   for( int i = 0; i < QComboBox::count(); ++i )
   {
      //! \todo find segfault because of missing/broken indirection
      if( itemData( i ).toUInt( &ok ) == value )
      {
         QComboBox::setCurrentIndex( i );
         return;
      }
      Q_ASSERT( ok );
   }
}


void SoundComboBox::setFromSong( const SlocumSong *slocumSong )
{
   mpSlocumSong = slocumSong;
   populate();
}
