/**
 * PsmkSongWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkSongWidget.hpp"

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
#include "PsmkContextMenu.hpp"
#include "PsmkPatternWidget.hpp"
#include "PsmkToneComboBox.hpp"
#include "PsmkPitchComboBox.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkPatternSelector.hpp"
#include "PsmkPacker.hpp"
#include "PsmkPlayerWidget.hpp"

#include <QtDebug>


const char PsmkSongWidget::cMimeType[] = "x-apefat-song";


PsmkSongWidget::PsmkSongWidget( QWidget *parent )
: QWidget( parent )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpLabelDelay( new QLabel( this ) )
, mpValueDelay( new QSpinBox( this ) )
, mpPsmkPacker( new PsmkPacker( this ) )
, mpPsmkPatternSelector( new PsmkPatternSelector( mpPsmkPacker, this ) )
, mpPsmkInstrumentsWidget( new PsmkInstrumentsWidget( this ) )
, mpPsmkHiHatWidget( new PsmkHiHatWidget( this ) )
, mpPsmkPlayerWidget( new PsmkPlayerWidget( this ) )
, mpUpdateDelay( new QTimer( this ) )
{
   setup();
}


PsmkSongWidget::~PsmkSongWidget()
{
   bool ok;
   QSettings settings;
   settings.beginGroup( "Store/Song" );
   QVariantMap variantMap( toVariantMap() );
   QString name( PsmkContextMenu::getNameFromMap( variantMap, &ok ) );
   if( ok )
   {
      if( name.isEmpty() )
      {
         name = tr("(Unnamed)");
      }
      settings.setValue( name, variantMap );
   }
}


void PsmkSongWidget::setup()
{
   mpValueDelay->setRange( 1, 50 );
   mpUpdateDelay->setSingleShot( true );
   mpUpdateDelay->setInterval( 20 );

   QBoxLayout  *layout = new QHBoxLayout( this );
   QGridLayout *buttonLayout = new QGridLayout();

   layout->setContentsMargins( 0, 0, 0, 0 );
   buttonLayout->setColumnStretch( 1, 2 );
   buttonLayout->setRowStretch( 6, 1 );

   layout->addStretch( 1 );
   buttonLayout->setRowMinimumHeight( 0, 8 );
   buttonLayout->addWidget( mpLabelName,               1, 0 );
   buttonLayout->addWidget( mpValueName,               1, 1, 1, 2 );
   buttonLayout->addWidget( mpLabelDelay,              2, 0 );
   buttonLayout->addWidget( mpValueDelay,              2, 2 );
   buttonLayout->addWidget( mpPsmkPacker,              4, 0, 1, 3 );
   buttonLayout->addWidget( mpPsmkPlayerWidget,        7, 0, 1, 3 );
   buttonLayout->addWidget( mpPsmkInstrumentsWidget,   8, 0, 1, 3 );
   //buttonLayout->addStretch( 1 );
   layout->addLayout( buttonLayout );
   layout->addWidget( mpPsmkPatternSelector );
   layout->addWidget( mpPsmkHiHatWidget );
   layout->addStretch( 1 );

   connect( mpPsmkInstrumentsWidget, SIGNAL(instrumentChanged(int,quint8)),
            mpPsmkPatternSelector, SLOT(setInstrument(int,quint8)) );
   connect( mpPsmkPlayerWidget, SIGNAL(playingPattern(int)),
            mpPsmkPatternSelector, SLOT(setPattern(int)) );
   connect( mpPsmkInstrumentsWidget, SIGNAL(instrumentChanged(int,quint8)),
            mpUpdateDelay, SLOT(start()) );
   connect( mpPsmkHiHatWidget, SIGNAL(changed()),
            mpUpdateDelay, SLOT(start()) );
   connect( mpPsmkPatternSelector, SIGNAL(changed()),
            mpUpdateDelay, SLOT(start()) );
   connect( mpValueDelay, SIGNAL(valueChanged(int)),
            mpUpdateDelay, SLOT(start()) );
   connect( mpUpdateDelay, SIGNAL(timeout()),
            this, SLOT(updateSong()) );

   mpPsmkPatternSelector->setPattern( 0 );
   setTexts();
}


void PsmkSongWidget::setTexts()
{
   mpLabelName->setText( tr("Song Name:") );
   mpLabelDelay->setText( tr("Delay:") );
}


QVariantMap PsmkSongWidget::toVariantMap()
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


bool PsmkSongWidget::fromVariantMap( const QVariantMap &variantMap )
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

   updateSong();

   return retval;
}


QByteArray PsmkSongWidget::toSourceCode()
{
   mpPsmkPacker->update( this );
   return mpPsmkPacker->toSourceCode( this );
}


void PsmkSongWidget::updateSong()
{
//   mpPsmkPacker->update( this );
   mpPsmkPacker->toSongBinary( mpPsmkPlayerWidget->songBinary(), this );
   mpPsmkPlayerWidget->updateSong(); //! \todo move to signal
}


bool PsmkSongWidget::fromVariantMapSlocum( const QVariantMap &variantMap )
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

   retval &= mpPsmkInstrumentsWidget->fromVariantMap( variantMap.value( "sound" ).toMap() );
   retval &= mpPsmkHiHatWidget->fromVariantMap( variantMap.value( "hihat" ).toMap() );
   mpPsmkPatternSelector->setInstrumentCache( mpPsmkInstrumentsWidget->toBinary() );
   retval &= mpPsmkPatternSelector->fromVariantList( newVoices );

   setTexts();

   return true;
}


bool PsmkSongWidget::fromVariantMapPsmk( const QVariantMap &variantMap )
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
   retval &= mpPsmkInstrumentsWidget->fromVariantMap( variantMap.value( "instruments" ).toMap() );
   retval &= mpPsmkHiHatWidget->fromVariantMap( variantMap.value( "hihat" ).toMap() );
   mpPsmkPatternSelector->setInstrumentCache( mpPsmkInstrumentsWidget->toBinary() );
   retval &= mpPsmkPatternSelector->fromVariantList( variantMap.value( "bars" ).toList() );

   return retval;
}


void PsmkSongWidget::updatePacker()
{
   //mpPsmkPacker->update( this );
   QApplication::clipboard()->setText( QString::fromUtf8(mpPsmkPacker->toSourceCode(this) ) );
   mpPsmkPacker->toSongBinary( mpPsmkPlayerWidget->songBinary(), this );
}


void PsmkSongWidget::contextMenuEvent( QContextMenuEvent *event )
{
   PsmkContextMenu menu( tr("Song"), cMimeType, toVariantMap(), this );

   if( menu.runSelect( event->globalPos() - QPoint( 5, 5 ) ) == PsmkContextMenu::Paste )
   {
      fromVariantMap( menu.mimeData().toMap() );
   }
}
