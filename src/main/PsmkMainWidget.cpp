/**
 * PsmkMainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkMainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QClipboard>
#include <QBoxLayout>
#include <QCheckBox>
#include <QCommonStyle>
#include <QDragEnterEvent>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTimer>

/* local library headers */
#include <json.h>

/* local headers */
#include "PsmkPatternWidget.hpp"
#include "PsmkToneComboBox.hpp"
#include "PsmkPitchComboBox.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkClipboard.hpp"
#include "PsmkPatternSelector.hpp"
#include "PsmkPacker.hpp"
#include "PsmkPlayerWidget.hpp"

#include <QtDebug>


#include "PsmkNoteWidget.hpp"


PsmkMainWidget::PsmkMainWidget( QWidget *parent )
: QWidget( parent )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpLabelDelay( new QLabel( this ) )
, mpValueDelay( new QSpinBox( this ) )
, mpPsmkPatternSelector( new PsmkPatternSelector( this ) )
, mpPsmkPacker( new PsmkPacker( this ) )
, mpPsmkInstrumentsWidget( new PsmkInstrumentsWidget( this ) )
, mpPsmkHiHatWidget( new PsmkHiHatWidget( this ) )
, mpPsmkPlayerWidget( new PsmkPlayerWidget( this ) )
, mpLeadInLabel( new QLabel( this ) )
, mpLeadOutLabel( new QLabel( this ) )
{
   setup();
}


PsmkMainWidget::~PsmkMainWidget()
{
}


void PsmkMainWidget::setup()
{
   mpValueDelay->setRange( 1, 50 );

   QBoxLayout  *layout = new QHBoxLayout( this );
   QGridLayout *buttonLayout = new QGridLayout();

   layout->setContentsMargins( 0, 0, 0, 0 );
   buttonLayout->setColumnStretch( 1, 2 );
   buttonLayout->setRowStretch( 6, 1 );

   layout->addWidget( mpLeadInLabel );
   buttonLayout->setRowMinimumHeight( 0, 8 );
   buttonLayout->addWidget( mpLabelName,               1, 0 );
   buttonLayout->addWidget( mpValueName,               1, 1, 1, 2 );
   buttonLayout->addWidget( mpLabelDelay,              2, 0 );
   buttonLayout->addWidget( mpValueDelay,              2, 2 );
   buttonLayout->addWidget( mpPsmkPacker,              4, 0, 1, 3 );
   buttonLayout->addWidget( new PsmkClipboard( this ), 5, 0, 1, 3 );
   buttonLayout->addWidget( mpPsmkPlayerWidget,        7, 0, 1, 3 );
   buttonLayout->addWidget( mpPsmkInstrumentsWidget,   8, 0, 1, 3 );
   //buttonLayout->addStretch( 1 );
   layout->addLayout( buttonLayout );
   layout->addWidget( mpPsmkPatternSelector );
   layout->addWidget( mpPsmkHiHatWidget );
   layout->addWidget( mpLeadOutLabel );

   connect( mpPsmkInstrumentsWidget, SIGNAL(instrumentChanged(int,quint8)),
            mpPsmkPatternSelector, SLOT(instrumentChanged(int,quint8)) );
   connect( mpPsmkPlayerWidget, SIGNAL(playingPattern(int)),
            mpPsmkPatternSelector, SLOT(setPattern(int)) );
   connect( mpPsmkInstrumentsWidget, SIGNAL(instrumentChanged(int,quint8)),
            this, SLOT(updateSong()) );
   connect( mpPsmkHiHatWidget, SIGNAL(changed()),
            this, SLOT(updateSong()) );
   connect( mpPsmkPatternSelector, SIGNAL(changed()),
            this, SLOT(updateSong()) );

   QTimer *t = new QTimer( this );
   connect( t, SIGNAL(timeout()),
            this, SLOT(updatePacker()) );
   t->setSingleShot( true );
   t->setInterval( 1000 );
   t->start();
   connect( mpPsmkPatternSelector, SIGNAL(changed()),
            t, SLOT(start()) );

   mpPsmkPatternSelector->setPattern( 0 );
   setTexts();
}


void PsmkMainWidget::setTexts()
{
   mpLabelName->setText( tr("Song Name:") );
   mpLabelDelay->setText( tr("Delay:") );
}


QVariantMap PsmkMainWidget::toVariantMap()
{
   QVariantMap variantMap;

   variantMap.insert( "type", "psmk0" );
   variantMap.insert( "name", mpValueName->text() );
   variantMap.insert( "delay", mpValueDelay->value() );
   variantMap.insert( "instruments", mpPsmkInstrumentsWidget->toVariantMap() );
   variantMap.insert( "hihat", mpPsmkHiHatWidget->toVariantMap() );
   variantMap.insert( "bars", mpPsmkPatternSelector->toVariantList() );

   return variantMap;
}


bool PsmkMainWidget::fromVariantMap( const QVariantMap &variantMap )
{
   bool retval = true;
   const QString type( variantMap.value( "type" ).toString() );

   if( type.startsWith("psmk") )
   {
      retval = fromVariantMapPsmk( variantMap );
   }
   else // slocum0 or none
   {
      retval = fromVariantMapSlocum( variantMap );
   }

   for( int i = 0; i < PsmkConfig::InstrumentsInSong; ++i )
   {
      mpPsmkPatternSelector->setInstrument( i, mpPsmkInstrumentsWidget->instrument(i) );
   }
   return retval;
}


QByteArray PsmkMainWidget::toSourceCode()
{
   mpPsmkPacker->update( this );
   return mpPsmkPacker->toSourceCode( this );
}


void PsmkMainWidget::updateSong()
{
   mpPsmkPacker->update( this );
   mpPsmkPlayerWidget->updateSong(); //! \todo move to signal
}


bool PsmkMainWidget::fromVariantMapSlocum( const QVariantMap &variantMap )
{
   QVariantList newVoices;

   bool retval = true;
   if( !variantMap.contains( "name" ) ||
       !variantMap.contains( "delay" ) ||
       !variantMap.contains( "sound" ) ||
       !variantMap.contains( "hihat" ) ||
       !variantMap.contains( "voices" ) )
   {
      return false;
   }
   mpValueName->setText( variantMap.value( "name" ).toString() );
   mpValueDelay->setValue( variantMap.value( "delay" ).toInt( &retval ) );
   if( !retval )
   {
      return false;
   }
   int hiHatStart = variantMap.value( "hihat" ).toMap().value( "start" ).toInt();

   QVariantList voices( variantMap.value( "voices" ).toList() );
   if( voices.size() != 2 )
   {
      return false;
   }

   // create temporary just to get a variantMap of an empty Pattern
   PsmkPatternWidget *emptyPattern = new PsmkPatternWidget( this );
   emptyPattern->deleteLater();
   emptyPattern->hide();
   QVariantMap emptyVariantMap( emptyPattern->toVariantMap() );

   QVariantList voice0( voices.at(0).toList() );
   QVariantList voice1( voices.at(1).toList() );
   while( voice0.size() < voice1.size() )
   {
      voice0.append( emptyVariantMap );
   }
   while( voice1.size() < voice0.size() )
   {
      voice1.append( emptyVariantMap );
   }

   for( int i = 0; i < voice0.size(); ++i )
   {
      QVariantMap map;

      map.insert( "voice0", voice0.at(i) );
      map.insert( "voice1", voice1.at(i) );
      map.insert( "hihat",  i >= hiHatStart );
      newVoices.append( map );
   }

   retval &= mpPsmkHiHatWidget->fromVariantMap( variantMap.value( "hihat" ).toMap() );
   retval &= mpPsmkPatternSelector->fromVariantList( newVoices );
   retval &= mpPsmkInstrumentsWidget->fromVariantMap( variantMap.value( "sound" ).toMap() );

   setTexts();

   return true;
}


bool PsmkMainWidget::fromVariantMapPsmk( const QVariantMap &variantMap )
{
   bool retval = true;
   if( !variantMap.contains( "name" ) ||
       !variantMap.contains( "delay" ) ||
       !variantMap.contains( "instruments" ) ||
       !variantMap.contains( "hihat" ) ||
       !variantMap.contains( "bars" ) )
   {
      return false;
   }
   mpValueName->setText( variantMap.value( "name" ).toString() );
   mpValueDelay->setValue( variantMap.value( "delay" ).toInt( &retval ) );
   if( !retval )
   {
      return false;
   }
   retval &= mpPsmkHiHatWidget->fromVariantMap( variantMap.value( "hihat" ).toMap() );
   retval &= mpPsmkPatternSelector->fromVariantList( variantMap.value( "bars" ).toList() );
   retval &= mpPsmkInstrumentsWidget->fromVariantMap( variantMap.value( "instruments" ).toMap() );

   return retval;
}


void PsmkMainWidget::updatePacker()
{
   //mpPsmkPacker->update( this );
   QApplication::clipboard()->setText( QString::fromUtf8(mpPsmkPacker->toSourceCode(this) ) );
   mpPsmkPacker->toSongBinary( mpPsmkPlayerWidget->songBinary(), this );
}
