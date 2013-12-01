/**
 * HiHatWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "HiHatWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSpinBox>

/* local library headers */
#include "json.h"

/* local headers */
#include "PitchComboBox.hpp"
#include "SlocumSong.hpp"
#include "SoundComboBox.hpp"

#include <QtDebug>


HiHatWidget::HiHatWidget( SlocumSong *slocumSong, QWidget *parent )
: QGroupBox( parent )
, mpSlocumSong( slocumSong )
, mpSlocumHiHat( 0 )
, mpPatternMapper( new QSignalMapper )
, mpMainLayout( new QVBoxLayout( this ) )
, mpNameLayout( new QHBoxLayout() )
, mpNumbersLayout( new QHBoxLayout() )
, mpPatternLayout( new QHBoxLayout() )
, mpLabelName( new QLabel( this ) )
, mpLabelStart( new QLabel( this ) )
, mpLabelSound( new QLabel( this ) )
, mpLabelPitch( new QLabel( this ) )
, mpLabelVolume( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpValueStart( new QSpinBox( this ) )
, mpValueSound( new SoundComboBox( this ) )
, mpValuePitch( new PitchComboBox( this ) )
, mpValueVolume( new QSpinBox( this ) )
, mPattern()
{
   setup();
}


HiHatWidget::HiHatWidget( SlocumHiHat *slocumHiHat, QWidget *parent )
: QGroupBox( parent )
, mpSlocumSong( 0 )
, mpSlocumHiHat( slocumHiHat )
, mpPatternMapper( new QSignalMapper )
, mpMainLayout( new QVBoxLayout( this ) )
, mpNameLayout( new QHBoxLayout() )
, mpNumbersLayout( new QHBoxLayout() )
, mpPatternLayout( new QHBoxLayout() )
, mpLabelName( new QLabel( this ) )
, mpLabelStart( new QLabel( this ) )
, mpLabelSound( new QLabel( this ) )
, mpLabelPitch( new QLabel( this ) )
, mpLabelVolume( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpValueStart( new QSpinBox( this ) )
, mpValueSound( new SoundComboBox( this ) )
, mpValuePitch( new PitchComboBox( this ) )
, mpValueVolume( new QSpinBox( this ) )
, mPattern()
{
   setup();
}


void HiHatWidget::setup()
{
   setLayout( mpMainLayout );
   mpMainLayout->addLayout( mpNameLayout );
   mpMainLayout->addLayout( mpNumbersLayout );
   mpMainLayout->addLayout( mpPatternLayout );

   mpNameLayout->addWidget( mpLabelName );
   mpNameLayout->addWidget( mpValueName );
   mpNameLayout->setStretch( 1, 1 );

   mpNumbersLayout->addWidget( mpLabelStart );
   mpNumbersLayout->addWidget( mpValueStart );
   mpNumbersLayout->addStretch( 1 );
   mpNumbersLayout->addWidget( mpLabelSound );
   mpNumbersLayout->addWidget( mpValueSound );
   mpNumbersLayout->addStretch( 1 );
   mpNumbersLayout->addWidget( mpLabelPitch );
   mpNumbersLayout->addWidget( mpValuePitch );
   mpNumbersLayout->addStretch( 1 );
   mpNumbersLayout->addWidget( mpLabelVolume );
   mpNumbersLayout->addWidget( mpValueVolume );

   mpValueStart->setRange( 0, 255 );
   mpValueVolume->setRange( 0, 15 );
   mpValueStart->setAlignment( Qt::AlignRight );
   mpValueVolume->setAlignment( Qt::AlignRight );

   mpPatternLayout->setContentsMargins( 0, 0, 0, 0 );
   mpPatternLayout->setSpacing( 0 );
   for( int i = 0; i < mpSlocumSong->hihat()->size(); ++i )
   {
      if( (i > 0) && !(i % 8) )
      {
         QLabel *l = new QLabel( "|", this );
         l->setContentsMargins( 0, 0, 0, 0 );
         l->setAlignment( Qt::AlignCenter );
         mpPatternLayout->addWidget( l );
      }
      QCheckBox *cb = new QCheckBox( this );
      cb->setContentsMargins( 0, 0, 0, 0 );
      mpPatternLayout->addWidget( cb );
      mpPatternMapper->setMapping( cb, i );
      connect( cb, SIGNAL(clicked()),
               mpPatternMapper, SLOT(map()) );
      mPattern.append( cb );
   }
   connect( mpValueName, SIGNAL(textChanged(QString)),
            this, SLOT(setName(QString)) );
   connect( mpValueStart, SIGNAL(valueChanged(int)),
            this, SLOT(setStart(int)) );
   connect( mpValueSound, SIGNAL(valueChanged(quint8)),
            this, SLOT(setSound(quint8)) );
   connect( mpValuePitch, SIGNAL(valueChanged(quint8)),
            this, SLOT(setPitch(quint8)) );
   connect( mpValueVolume, SIGNAL(valueChanged(int)),
            this, SLOT(setVolume(int)) );
   connect( mpPatternMapper, SIGNAL(mapped(int)),
            this, SLOT(patternChanged(int)) );
   connect( mpValueSound, SIGNAL(valueChanged(quint8)),
            mpValuePitch, SLOT(setSound(quint8)) );

   setTexts();
   setFromSong();
}


SlocumHiHat *HiHatWidget::hihat()
{
   if( mpSlocumSong )
   {
      return mpSlocumSong->hihat();
   }
   return mpSlocumHiHat;
}


HiHatWidget::~HiHatWidget()
{
}


void HiHatWidget::setName( const QString &setname )
{
   hihat()->name = setname;
}


void HiHatWidget::setStart( int i )
{
   hihat()->start = i;
}


void HiHatWidget::setSound( quint8 i )
{
   hihat()->sound = i;
}


void HiHatWidget::setPitch( quint8 i )
{
   hihat()->pitch = i;
}


void HiHatWidget::setVolume( int i )
{
   hihat()->volume = i;
}


void HiHatWidget::setTexts()
{
   setTitle( tr("HiHat:") );
   mpLabelName->setText( tr("Name:") );
   mpLabelStart->setText( tr("Start:") );
   mpLabelSound->setText( tr("Sound:") );
   mpLabelPitch->setText( tr("Pitch:") );
   mpLabelVolume->setText( tr("Volume:") );
}


void HiHatWidget::setFromSong( SlocumHiHat *slocumHiHat )
{
   Q_ASSERT( !mpSlocumSong );
   if( slocumHiHat && !mpSlocumSong )
   {
      mpSlocumHiHat = slocumHiHat;
   }
   setFromSong();
}


void HiHatWidget::setFromSong( SlocumSong *slocumSong )
{
   Q_ASSERT( !mpSlocumHiHat );
   if( slocumSong && !mpSlocumHiHat )
   {
      mpSlocumSong = slocumSong;
   }
   setFromSong();
}


void HiHatWidget::setFromSong()
{
   mpValueName->setText( hihat()->name );
   mpValueStart->setValue( hihat()->start );
   mpValueSound->setValue( hihat()->sound );
   mpValuePitch->setValue( hihat()->pitch );
   mpValueVolume->setValue( hihat()->volume );
   for( int i = 0; i < hihat()->size(); ++i )
   {
      mPattern.at( i )->setChecked( hihat()->pattern[i] );
   }
}


void HiHatWidget::patternChanged( int index )
{
   hihat()->pattern[index] = mPattern.at( index )->isChecked();
}
