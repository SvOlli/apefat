/**
 * PsmkPlayerWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPlayerWidget.hpp"

/* system headers */

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QBoxLayout>
#include <QComboBox>
#include <QCommonStyle>
#include <QSpinBox>
#include <QToolButton>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#endif
#include <QThread>

/* local library headers */

/* local headers */
#include "PlayerEmulation.hpp"



PsmkPlayerWidget::PsmkPlayerWidget( QWidget *parent )
: QGroupBox( parent )
, mpSongBinary( new SongBinary() )
, mpPlayerEmulation( new PlayerEmulation( /*this*/ 0 ) )
, mpPlayButton( new QToolButton( this ) )
, mpLoopButton( new QToolButton( this ) )
, mpCurrentPattern( new QSpinBox( this ) )
, mpCurrentNote( new QSpinBox( this ) )
, mpChannels( new QComboBox( this ) )
{
   setup();
   mpPlayerEmulation->start();
}


PsmkPlayerWidget::~PsmkPlayerWidget()
{
   mpPlayerEmulation->quit();
   mpPlayerEmulation->wait();
   delete mpPlayerEmulation;
   delete mpSongBinary;
}


SongBinary *PsmkPlayerWidget::songBinary()
{
   return mpSongBinary;
}


void PsmkPlayerWidget::setup()
{
   QCommonStyle style;

   mpPlayerEmulation->loadPlayer( ":/player.bin" );

   mpPlayButton->setCheckable( true );
   mpLoopButton->setCheckable( true );
   mpLoopButton->setIcon( style.standardIcon( QStyle::SP_BrowserReload ) );
   mpCurrentPattern->setAlignment( Qt::AlignRight );
   mpCurrentPattern->setRange( 1, 256 );
   mpCurrentPattern->setDisabled( true );
   mpCurrentNote->setAlignment( Qt::AlignRight );
   mpCurrentNote->setRange( 1, 32 );
   mpCurrentNote->setDisabled( true );

   QBoxLayout *mainLayout = new QHBoxLayout( this );

   mainLayout->addWidget( mpPlayButton );
   mainLayout->addWidget( mpLoopButton );
   mainLayout->addWidget( mpCurrentPattern );
   mainLayout->addWidget( mpCurrentNote );
   mainLayout->addWidget( mpChannels );
   mainLayout->addStretch( 1 );

   connect( mpPlayButton, SIGNAL(toggled(bool)),
            this, SLOT(setText()) );
   connect( mpPlayButton, SIGNAL(toggled(bool)),
            this, SLOT(startStop(bool)) );
   connect( mpLoopButton, SIGNAL(toggled(bool)),
            mpCurrentPattern, SLOT(setEnabled(bool)) );
   connect( mpLoopButton, SIGNAL(toggled(bool)),
            mpPlayerEmulation, SLOT(setLooping(bool)) );
   connect( mpPlayerEmulation, SIGNAL(stateUpdate(int,int)),
            this, SIGNAL(playingPattern(int)) );
   connect( mpPlayerEmulation, SIGNAL(stateUpdate(int,int)),
            this, SLOT(updatePlayerState(int,int)) );
   connect( mpCurrentPattern, SIGNAL(valueChanged(int)),
            this, SLOT(setPatternByCurrent()) );
   connect( mpChannels, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setChannels(int)) );

   setText();
}


void PsmkPlayerWidget::setText()
{
   QCommonStyle style;
   mpPlayButton->setIcon( style.standardIcon( mpPlayButton->isChecked() ? QStyle::SP_MediaStop : QStyle::SP_MediaPlay ) );

   setTitle( tr("Player:") );
   mpChannels->clear();
   /* order is the same as in enum PlayerConfig::Channels */
   mpChannels->addItem( tr("Mono") );
   mpChannels->addItem( tr("Stereo") );
   mpChannels->addItem( tr("Left") );
   mpChannels->addItem( tr("Right") );
}


void PsmkPlayerWidget::startStop( bool play )
{
   if( play )
   {
      updateSong();
      QMetaObject::invokeMethod( mpPlayerEmulation, "startSong", Qt::QueuedConnection );
      //mpPlayerEmulation->startSong();
   }
   else
   {
      QMetaObject::invokeMethod( mpPlayerEmulation, "stopSong", Qt::QueuedConnection );
      //mpPlayerEmulation->stopSong();
   }
}


void PsmkPlayerWidget::updatePlayerState( int pattern, int note )
{
   mpCurrentPattern->setValue( pattern + 1 );
   mpCurrentNote->setValue( note + 1 );
}


void PsmkPlayerWidget::updateSong()
{
   mpPlayerEmulation->songToMemory( mpSongBinary );
}


void PsmkPlayerWidget::setPatternByCurrent()
{
   mpPlayerEmulation->setCurrentPattern( mpCurrentPattern->value() - 1 );
}


void PsmkPlayerWidget::setChannels( int index )
{
   PlayerConfig::Channels channels = PlayerConfig::Mono;
   if( (index >= 0) && (index < 4) )
   {
      channels = static_cast<PlayerConfig::Channels>( index );
   }
   mpPlayerEmulation->setChannels( channels );
}
