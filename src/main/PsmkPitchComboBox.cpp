/**
 * PsmkPitchComboBox.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPitchComboBox.hpp"

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


int  PsmkPitchComboBox::noteThreshold = 11;
bool PsmkPitchComboBox::usePal = true;


PsmkPitchComboBox::PsmkPitchComboBox( QWidget *parent )
: QComboBox( parent )
{
   QComboBox::setMaxVisibleItems( TONE_SIZE );
   for( int i = 0; i < TONE_SIZE; ++i )
   {
      QComboBox::addItem( QString() );
   }
   connect( this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(indexToValue()) );
   setSound( 255 );
}


PsmkPitchComboBox::~PsmkPitchComboBox()
{
}


quint8 PsmkPitchComboBox::value()
{
   return (quint8) (0xff & QComboBox::currentIndex());
}


void PsmkPitchComboBox::setSound( quint8 sound )
{
   QFont font;
   if( sound < 255 )
   {
      QWidget::setEnabled( true );
      tia_tone_t *tone = usePal ? Tones_pal[sound] : Tones_ntsc[sound];
      for( int i = 0; i < TONE_SIZE; ++i )
      {
         QString text("%1:%2 (%3%)");
         QString num( QString::number(i).rightJustified(2,'0') );
         text = text.arg( num, tone[i].name, QString::number(tone[i].percent) );
         if( !sound && !i )
         {
            // workaround: to make widget wider on creation
            text.append( "     " );
         }
         QComboBox::setItemText( i, QString("%1").arg(text) );
         font.setBold( (tone[i].should > 16.3) && (tone[i].should < 15000) &&
                       (::abs(tone[i].percent) <= noteThreshold ) );
         QComboBox::setItemData( i, font, Qt::FontRole );
      }
   }
   else
   {
      QWidget::setEnabled( false );

      for( int i = 0; i < TONE_SIZE; ++i )
      {
         // workaround: tailing spaces necessary for width
         QComboBox::setItemText( i, "---                  " );
         QComboBox::setItemData( i, font, Qt::FontRole );
      }
   }
   QWidget::updateGeometry();
}


void PsmkPitchComboBox::setValue( quint8 value )
{
   if( value == 255 )
   {
      value = 0;
   }
   QComboBox::setCurrentIndex( value );
}


void PsmkPitchComboBox::indexToValue()
{
   emit valueChanged( value() );
}
