/**
 * PlayerWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QCommonStyle>
#include <QDragEnterEvent>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>

/* local library headers */
#include <json.h>

/* local headers */
#include "BarWidget.hpp"
#include "PlayerEmulation.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


PlayerWidget::PlayerWidget( QWidget *parent )
: QWidget( parent )
, mpPlayerEmulation( new PlayerEmulation( this ) )
, mpPlayButton( new QPushButton( this ) )
, mpLoopButton( new QPushButton( this ) )
, mpCurrentBar( new QSpinBox( this ) )
, mpState( new QLabel( this ) )
{
   mpCurrentBar->setRange( 0, 255 );
   mpCurrentBar->setDisabled( true );
   mpCurrentBar->setAlignment( Qt::AlignRight );
   mpPlayButton->setCheckable( true );
   mpPlayerEmulation->loadPlayer( ":/player.bin" );

   QBoxLayout *layout = new QHBoxLayout( this );
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->addWidget( mpPlayButton );
   layout->addWidget( mpLoopButton );
   layout->addWidget( mpCurrentBar );
   layout->addWidget( mpState );

   connect( mpPlayerEmulation, SIGNAL(currentBar(int)),
            this, SIGNAL(currentBar(int)) );
   connect( mpPlayerEmulation, SIGNAL(state(QString)),
            this, SLOT(state(QString)) );
   connect( mpPlayerEmulation, SIGNAL(currentBar(int)),
            mpCurrentBar, SLOT(setValue(int)) );
   connect( this, SIGNAL(setLoopState(bool)),
            mpCurrentBar, SLOT(setEnabled(bool)) );
   connect( this, SIGNAL(setCurrentBar(int)),
            mpPlayerEmulation, SLOT(setCurrentBar(int)) );

   registerPlayButton( mpPlayButton );
   registerLoopButton( mpLoopButton );
   setTexts();
}


PlayerWidget::~PlayerWidget()
{
}


void PlayerWidget::registerPlayButton( QAbstractButton *button )
{
   connect( button, SIGNAL(clicked(bool)),
            this, SLOT(startStop(bool)) );
   connect( this, SIGNAL(setPlayState(bool)),
            button, SLOT(setChecked(bool)) );
   button->setCheckable( true );
}


void PlayerWidget::registerLoopButton( QAbstractButton *button )
{
   connect( button, SIGNAL(clicked(bool)),
            this, SLOT(setLoop(bool)) );
   connect( this, SIGNAL(setLoopState(bool)),
            button, SLOT(setChecked(bool)) );
   button->setCheckable( true );
}


void PlayerWidget::setTexts()
{
   mpPlayButton->setText( tr("Play") );
   mpLoopButton->setText( tr("Loop") );
}


void PlayerWidget::setFromSong( SlocumSong *slocumSong )
{
   mpPlayerEmulation->setSong( slocumSong );
}


void PlayerWidget::startStop( bool play )
{
   if( play )
   {
      mpPlayerEmulation->start();
   }
   else
   {
      emit setLoopState( false );
      mpPlayerEmulation->stop();
   }
   emit setPlayState( play );
}


void PlayerWidget::setLoop( bool enabled )
{
   mpPlayerEmulation->setLooping( enabled );
   if( !mpPlayButton->isChecked() )
   {
      startStop( true );
   }
   emit setLoopState( enabled );
}


void PlayerWidget::state( const QString &msg )
{
   mpState->setText( msg );
}
