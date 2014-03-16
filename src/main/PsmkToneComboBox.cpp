/**
 * PsmkToneComboBox.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkToneComboBox.hpp"

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


PsmkToneComboBox::PsmkToneComboBox( QWidget *parent )
: QComboBox( parent )
{
   populate();
   connect( this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(indexToValue(int)) );
}


PsmkToneComboBox::~PsmkToneComboBox()
{
}


void PsmkToneComboBox::populate()
{
   QComboBox::setMaxVisibleItems( TONES_SIZE );
   int savedIndex = QComboBox::currentIndex();
   QComboBox::clear();

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
   if( savedIndex >= 0 )
   {
      QComboBox::setCurrentIndex( savedIndex );
   }
}


quint8 PsmkToneComboBox::value()
{
   bool ok;
   quint8 value = itemData( currentIndex() ).toInt( &ok );
   Q_ASSERT( ok );
   return value;
}


void PsmkToneComboBox::setValue( quint8 value )
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


void PsmkToneComboBox::indexToValue( int index )
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
