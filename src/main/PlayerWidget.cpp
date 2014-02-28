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

/* local library headers */
#include <json.h>

/* local headers */
#include "BarWidget.hpp"
#include "PlayerEmulation.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


PlayerWidget::PlayerWidget( QWidget *parent )
: QWidget( parent )
, mpPlayButton( new QPushButton( this ) )
, mpPlayerEmulation( new PlayerEmulation( this ) )
{
   mpPlayButton->setCheckable( true );
   mpPlayerEmulation->loadPlayer( ":/player.bin" );

   QBoxLayout *layout = new QHBoxLayout( this );
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->addWidget( mpPlayButton );

   connect( mpPlayButton, SIGNAL(clicked(bool)),
            this, SLOT(startStop(bool)) );

   setTexts();
}


PlayerWidget::~PlayerWidget()
{
}


void PlayerWidget::setTexts()
{
   mpPlayButton->setText( tr("Play") );
}


void PlayerWidget::setFromSong( SlocumSong *slocumSong )
{
   mpPlayerEmulation->setSong( slocumSong );
}


void PlayerWidget::startStop(bool play)
{
   if( play )
   {
      mpPlayerEmulation->start();
   }
   else
   {
      mpPlayerEmulation->stop();
   }
}
