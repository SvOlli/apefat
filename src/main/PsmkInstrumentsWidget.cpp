/**
 * PsmkInstrumentWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkInstrumentsWidget.hpp"

/* system headers */

/* Qt headers */
#include <QGridLayout>
#include <QCheckBox>
#include <QContextMenuEvent>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSpinBox>
#include <QTimer>

/* local library headers */

/* local headers */
#include "PsmkToneComboBox.hpp"
#include "PsmkContextMenu.hpp"


#include <QtDebug>


const char PsmkInstrumentsWidget::cMimeType[] = "x-apefat-instruments";


PsmkInstrumentsWidget::PsmkInstrumentsWidget( QWidget *parent )
: QGroupBox( parent )
, mpTypesMapper( new QSignalMapper( this ) )
, mpMainLayout( new QGridLayout( this ) )
, mpLabelName( new QLabel( this ) )
, mpLabelID( new QLabel( this ) )
, mpLabelType( new QLabel( this ) )
, mpLabelAtten( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpTypes()
, mpAttens()
{
   setup();
}


PsmkInstrumentsWidget::PsmkInstrumentsWidget( const QVariantMap &variantMap, QWidget *parent )
: QGroupBox( parent )
, mpTypesMapper( new QSignalMapper( this ) )
, mpMainLayout( new QGridLayout( this ) )
, mpLabelName( new QLabel( this ) )
, mpLabelID( new QLabel( this ) )
, mpLabelType( new QLabel( this ) )
, mpLabelAtten( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpTypes()
, mpAttens()
{
   setup();
   fromVariantMap( variantMap );
}


void PsmkInstrumentsWidget::setup()
{
   mpMainLayout->addWidget( mpLabelName,  0, 0 );
   mpMainLayout->addWidget( mpValueName,  0, 1, 1, 2 );
   mpMainLayout->addWidget( mpLabelID,    1, 0 );
   mpMainLayout->addWidget( mpLabelType,  1, 1 );
   mpMainLayout->addWidget( mpLabelAtten, 1, 2 );
   mpMainLayout->setColumnStretch( 1, 1 );

   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      QLabel *header = new QLabel( QString::number(i), this );
      mpTypes[i]     = new PsmkToneComboBox( this );
      mpAttens[i]    = new QSpinBox( this );
      header->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
      mpAttens[i]->setAlignment( Qt::AlignRight );
      mpAttens[i]->setRange( 0, 15 );
      mpMainLayout->addWidget( header,      2 + i, 0 );
      mpMainLayout->addWidget( mpTypes[i],  2 + i, 1 );
      mpMainLayout->addWidget( mpAttens[i], 2 + i, 2 );
      mpTypesMapper->setMapping( mpTypes[i], i );
      connect( mpTypes[i], SIGNAL(currentIndexChanged(int)),
               mpTypesMapper, SLOT(map()) );
   }
   connect( mpTypesMapper, SIGNAL(mapped(int)),
            this, SLOT(typeChanged(int)) );
   setTexts();
}


PsmkInstrumentsWidget::~PsmkInstrumentsWidget()
{
}


const char *PsmkInstrumentsWidget::mimeType()
{
   return &cMimeType[0];
}


QVariantMap PsmkInstrumentsWidget::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList sounds;

   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      QVariantMap sound;
      sound.insert( "type", mpTypes[i]->value() );
      sound.insert( "atten", mpAttens[i]->value() );
      sounds.append( sound );
   }
   variantMap.insert( "name", mpValueName->text() );
   variantMap.insert( "instruments", sounds );

   return variantMap;
}


bool PsmkInstrumentsWidget::fromVariantMap( const QVariantMap &variantMap )
{
   bool ok;
   Q_ASSERT( variantMap.contains( "name" ) );
   mpValueName->setText( variantMap.value( "name" ).toString() );

   QVariantList sounds;
   if( variantMap.contains( "sounds" ) )
   {
      sounds = variantMap.value( "sounds" ).toList();
   }
   else
   {
      Q_ASSERT( variantMap.contains( "instruments" ) );
      sounds = variantMap.value( "instruments" ).toList();
   }
   Q_ASSERT( sounds.size() == PsmkConfig::InstrumentsInSong );
   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      QVariantMap sound( sounds.at(i).toMap() );
      Q_ASSERT( sound.contains( "type" ) );
      mpTypes[i]->setValue( sound.value( "type" ).toUInt( &ok ) );
      Q_ASSERT( ok );

      Q_ASSERT( sound.contains( "atten" ) );
      mpAttens[i]->setValue( sound.value( "atten" ).toUInt( &ok ) );
      Q_ASSERT( ok );
   }
   return true;
}


QByteArray PsmkInstrumentsWidget::toBinary()
{
   QByteArray retval;

   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      retval.append( mpTypes[i]->value() );
   }
   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      retval.append( mpAttens[i]->value() );
   }

   return retval;
}


void PsmkInstrumentsWidget::setTexts()
{
   setTitle( tr("Instruments:") );
   mpLabelName->setText( tr("Name:") );
   mpLabelType->setText( tr("Type:") );
   mpLabelAtten->setText( tr("Atten:") );
   mpLabelAtten->setToolTip( tr("Attenuation") );
}


void PsmkInstrumentsWidget::typeChanged( int index )
{
   emit instrumentChanged( index, mpTypes[index]->value() );
}


void PsmkInstrumentsWidget::contextMenuEvent( QContextMenuEvent *event )
{
   PsmkContextMenu menu( tr("Instruments"), cMimeType, toVariantMap(), this );

   if( menu.runSelect( event->globalPos() - QPoint( 5, 5 ) ) == PsmkContextMenu::Paste )
   {
      fromVariantMap( menu.mimeData().toMap() );
   }
}
