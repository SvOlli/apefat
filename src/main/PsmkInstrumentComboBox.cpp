/**
 * PsmkInstrumentComboBox.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkInstrumentComboBox.hpp"

/* system headers */

/* Qt headers */
#include <QGridLayout>
#include <QCheckBox>
#include <QContextMenuEvent>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSpinBox>

/* local library headers */
#include <Tones.h>

/* local headers */
#include "PsmkConfig.hpp"
#include "PsmkContextMenu.hpp"

#include <QtDebug>


PsmkInstrumentComboBox::PsmkInstrumentComboBox( QWidget *parent )
: QComboBox( parent )
, mInstruments()
{
   connect( this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkIndex(int)) );
   populate();
}


PsmkInstrumentComboBox::~PsmkInstrumentComboBox()
{
}


void PsmkInstrumentComboBox::populate()
{
   QComboBox::setMaxVisibleItems( TONES_SIZE );
   int savedIndex = QComboBox::currentIndex();
   QComboBox::clear();

   QComboBox::addItem( "--:no sound", 255 );
   QString name( "%1:%2" );
   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      QComboBox::addItem( name.arg( QString::number(i),
                                    Tones_names[mInstruments[i]] ),
                          i );
   }
   QComboBox::setCurrentIndex( savedIndex );
}


quint8 PsmkInstrumentComboBox::value()
{
   int index = currentIndex();
   if( index > 0 )
   {
      return (index-1) & 0xff;
   }
   else
   {
      return 255;
   }
}


void PsmkInstrumentComboBox::setTone( int index, quint8 tone )
{
   Q_ASSERT( (index >=0 ) && (index < PsmkConfig::InstrumentsInSong) );
   mInstruments[index] = tone;
   populate();
}


void PsmkInstrumentComboBox::setTones( quint8 tones[] )
{
   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      mInstruments[i] = tones[i];
   }
   populate();
}


void PsmkInstrumentComboBox::indexToValue( int index )
{
}


void PsmkInstrumentComboBox::setValue( quint8 value )
{
   if( value < 255 )
   {
      value++;
   }
   else
   {
      value = 0;
   }

   QComboBox::setCurrentIndex( value );
}


void PsmkInstrumentComboBox::checkIndex( int index )
{
   quint8 v = value();
   emit isMute( index == 0 );
   emit valueChanged( v );
   emit typeChanged( v > PsmkConfig::InstrumentsInSong ? 255 : mInstruments[v] );
}
