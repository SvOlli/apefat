/**
 * PsmkHiHatWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkHiHatWidget.hpp"

/* system headers */

/* Qt headers */
#include <QCheckBox>
#include <QContextMenuEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSpinBox>

/* local library headers */
#include "json.h"

/* local headers */
#include "PsmkPitchComboBox.hpp"
#include "PsmkToneComboBox.hpp"
#include "PsmkContextMenu.hpp"


#include <QtDebug>


const char PsmkHiHatWidget::cMimeType[] = "x-apefat-hihat";


PsmkHiHatWidget::PsmkHiHatWidget( QWidget *parent )
: QGroupBox( parent )
, mpLabelName( new QLabel( this ) )
, mpLabelTypePitch( new QLabel( this ) )
, mpLabelVolume( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpValueTone( new PsmkToneComboBox( this ) )
, mpValuePitch( new PsmkPitchComboBox( this ) )
, mpValueVolume( new QSpinBox( this ) )
, mpPattern()
{
   setup();
}


PsmkHiHatWidget::PsmkHiHatWidget( const QVariantMap &variantMap, QWidget *parent )
: QGroupBox( parent )
, mpLabelName( new QLabel( this ) )
, mpLabelTypePitch( new QLabel( this ) )
, mpLabelVolume( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpValueTone( new PsmkToneComboBox( this ) )
, mpValuePitch( new PsmkPitchComboBox( this ) )
, mpValueVolume( new QSpinBox( this ) )
, mpPattern()
{
   setup();
   fromVariantMap( variantMap );
}


PsmkHiHatWidget::~PsmkHiHatWidget()
{
}


const char *PsmkHiHatWidget::mimeType()
{
   return &cMimeType[0];
}


void PsmkHiHatWidget::setup()
{
   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QBoxLayout *volumeLayout  = new QHBoxLayout();

   volumeLayout->setContentsMargins( 0, 0, 0, 0 );
   volumeLayout->setSpacing( 2 );
   volumeLayout->addWidget( mpLabelVolume );
   volumeLayout->addWidget( mpValueVolume, 1 );

   mainLayout->addWidget( mpLabelName );
   mainLayout->addWidget( mpValueName );
   mainLayout->addSpacing( 8 );
   mainLayout->addWidget( mpLabelTypePitch );
   mainLayout->addWidget( mpValueTone );
   mainLayout->addWidget( mpValuePitch );
   mainLayout->addSpacing( 8 );
   mainLayout->addLayout( volumeLayout );
   mainLayout->setSpacing( 1 );

   mpValueVolume->setRange( 0, 15 );
   mpValueVolume->setAlignment( Qt::AlignRight );

   for( int i = 0; i < PsmkConfig::BeatsInHiHat; ++i )
   {
      //if( (i > 0) && !(i % 8) )
      if( !(i % 8) )
      {
         mainLayout->addSpacing( 8 );
      }
      mpPattern[i] = new QCheckBox( this );
      mpPattern[i]->setContentsMargins( 0, 0, 0, 0 );
      mainLayout->addWidget( mpPattern[i] );
      connect( mpPattern[i], SIGNAL(toggled(bool)),
               this, SIGNAL(changed()) );
   }
   mainLayout->addStretch( 1 );
   connect( mpValueTone, SIGNAL(valueChanged(quint8)),
            mpValuePitch, SLOT(setSound(quint8)) );
   connect( mpValueTone, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(changed()) );
   connect( mpValuePitch, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(changed()) );
   connect( mpValueVolume, SIGNAL(valueChanged(int)),
            this, SIGNAL(changed()) );

   setTexts();
}


void PsmkHiHatWidget::setTexts()
{
   setTitle( tr("HiHat:") );
   mpLabelName->setText( tr("Name:") );
   mpLabelTypePitch->setText( tr("Type/Pitch:") );
   mpLabelVolume->setText( tr("Volume:") );
   for( int i = 0; i < PsmkConfig::BeatsInHiHat; ++i )
   {
      mpPattern[i]->setText( tr("Beat %1 Note %2").arg(
                                QString::number((i / PsmkConfig::NotesInBeat) + 1),
                                QString::number((i % PsmkConfig::NotesInBeat) + 1) ));
   }
}


QVariantMap PsmkHiHatWidget::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList patterns;

   for( int i = 0; i < PsmkConfig::BeatsInHiHat; ++i )
   {
      patterns.append( mpPattern[i]->isChecked() );
   }
   variantMap.insert( "name", mpValueName->text() );
   variantMap.insert( "volume", mpValueVolume->value() );
   variantMap.insert( "tone", mpValueTone->value() );
   variantMap.insert( "pitch", mpValuePitch->value() );
   variantMap.insert( "patterns", patterns );

   return variantMap;
}


bool PsmkHiHatWidget::fromVariantMap( const QVariantMap &variantMap )
{
   bool ok;
   Q_ASSERT( variantMap.contains( "name" ) );
   mpValueName->setText( variantMap.value( "name" ).toString() );

   Q_ASSERT( variantMap.contains( "volume" ) );
   mpValueVolume->setValue( variantMap.value( "volume" ).toUInt( &ok ) );
   Q_ASSERT( ok );

   // for backward compatiblity
   if( variantMap.contains( "sound" ) )
   {
      mpValueTone->setValue( variantMap.value( "sound" ).toUInt( &ok ) );
   }
   else
   {
      Q_ASSERT( variantMap.contains( "tone" ) );
      mpValueTone->setValue( variantMap.value( "tone" ).toUInt( &ok ) );
      Q_ASSERT( ok );
   }

   Q_ASSERT( variantMap.contains( "pitch" ) );
   mpValuePitch->setValue( variantMap.value( "pitch" ).toUInt( &ok ) );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains( "patterns" ) );
   QVariantList patterns( variantMap.value("patterns").toList() );
   Q_ASSERT( patterns.size() == PsmkConfig::BeatsInHiHat );

   for( int i = 0; i < PsmkConfig::BeatsInHiHat; ++i )
   {
      mpPattern[i]->setChecked( patterns.at(i).toBool() );
   }
   return true;
}


QByteArray PsmkHiHatWidget::toBinary()
{
   QByteArray retval;

   retval.append( mpValueTone->value() );
   retval.append( mpValuePitch->value() );
   retval.append( mpValueVolume->value() );

   quint8 byte = 0;
   for( int i = 0; i < PsmkConfig::BeatsInHiHat; ++i )
   {
      byte <<= 1;
      byte |= mpPattern[i]->isChecked() ? 1 : 0;
      if( (i & 7) == 7 )
      {
         retval.append( byte );
         byte = 0;
      }
   }

   return retval;
}


void  PsmkHiHatWidget::contextMenuEvent(QContextMenuEvent *event)
{
   PsmkContextMenu menu( tr("HiHat"), cMimeType, toVariantMap(), this );

   if( menu.runSelect( event->globalPos() - QPoint( 5, 5 ) ) == PsmkContextMenu::Paste )
   {
      fromVariantMap( menu.mimeData().toMap() );
   }
}
