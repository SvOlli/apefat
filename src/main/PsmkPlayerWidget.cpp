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
#include <QApplication>
#include <QBoxLayout>
#include <QCommonStyle>
#include <QSpinBox>
#include <QThread>
#include <QToolButton>

/* local library headers */

/* local headers */
#include "PlayerEmulation.hpp"



PsmkPlayerWidget::PsmkPlayerWidget( QWidget *parent )
: QGroupBox( parent )
, mpSongBinary( new SongBinary() )
, mpPlayerEmulation( new PlayerEmulation( this ) )
, mpPlayButton( new QToolButton( this ) )
, mpLoopButton( new QToolButton( this ) )
, mpCurrentPattern( new QSpinBox( this ) )
, mpCurrentNote( new QSpinBox( this ) )
{
   setup();
}


PsmkPlayerWidget::~PsmkPlayerWidget()
{
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

   mpPlayerEmulation->start();
   setText();
}


void PsmkPlayerWidget::setText()
{
   QCommonStyle style;
   mpPlayButton->setIcon( style.standardIcon( mpPlayButton->isChecked() ? QStyle::SP_MediaStop : QStyle::SP_MediaPlay ) );

   setTitle( tr("Player:") );
}


void PsmkPlayerWidget::startStop( bool play )
{
   if( play )
   {
      updateSong();
      mpPlayerEmulation->start();
   }
   else
   {
      mpPlayerEmulation->stop();
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
