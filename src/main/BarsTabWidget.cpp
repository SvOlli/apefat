/**
 * BarsTabWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "BarsTabWidget.hpp"

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
#include "SlocumSong.hpp"


#include <QtDebug>


BarsTabWidget::BarsTabWidget( SlocumSong *slocumSong, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mCurrentBar( 0 )
, mpVoice0( new BarWidget( mpSlocumSong, 0, this ) )
, mpVoice1( new BarWidget( mpSlocumSong, 1, this ) )
, mpLeadInLabel( new QLabel( this ) )
, mpLeadOutLabel( new QLabel( this ) )
, mpLinkButton( new QPushButton( this ) )
, mpPlayButton( new QPushButton( this ) )
, mpLoopButton( new QPushButton( this ) )
{
   mpLinkButton->setCheckable( true );
   mpLinkButton->setChecked( true );
   mpPlayButton->setCheckable( true );
   mpLoopButton->setCheckable( true );

   QBoxLayout *layout = new QHBoxLayout( this );
   QBoxLayout *buttonLayout = new QVBoxLayout();
   layout->setContentsMargins( 0, 0, 0, 0 );

   layout->addWidget( mpLeadInLabel );
   layout->addWidget( mpVoice0 );
   buttonLayout->addStretch( 1 );
   buttonLayout->addWidget( mpPlayButton );
   buttonLayout->addWidget( mpLoopButton );
   buttonLayout->addWidget( mpLinkButton );
   buttonLayout->addStretch( 1 );
   layout->addLayout( buttonLayout );
   layout->addWidget( mpVoice1 );
   layout->addWidget( mpLeadOutLabel );

   layout->setStretchFactor( mpLeadInLabel, 1 );
   layout->setStretchFactor( mpLeadOutLabel, 1 );

   connect( mpVoice0, SIGNAL(barChanged(int)),
            this, SLOT(setBar(int)) );
   connect( mpVoice1, SIGNAL(barChanged(int)),
            this, SLOT(setBar(int)) );
   connect( mpLinkButton, SIGNAL(clicked(bool)),
            this, SLOT(handleLink(bool)) );
   connect( mpPlayButton, SIGNAL(clicked(bool)),
            this, SLOT(handlePlay(bool)) );

   handleLink( mpLinkButton->isChecked() );

   setTexts();
   setBar( 0 );
}


BarsTabWidget::~BarsTabWidget()
{
}


QPushButton *BarsTabWidget::playButton() const
{
   return mpPlayButton;
}


QPushButton *BarsTabWidget::loopButton() const
{
   return mpLoopButton;
}


void BarsTabWidget::setTexts()
{
   mpLinkButton->setText( tr("Link") );
   mpPlayButton->setText( tr("Play") );
   mpLoopButton->setText( tr("Loop") );
}


void BarsTabWidget::setFromSong( SlocumSong *slocumSong )
{
   mpSlocumSong = slocumSong;
   mpVoice0->setFromSong( slocumSong, 0 );
   mpVoice1->setFromSong( slocumSong, 0 );
}


void BarsTabWidget::setBar( int bar )
{
   if( mpLinkButton->isChecked() )
   {
      mpVoice0->setBar( bar );
      mpVoice1->setBar( bar );
   }
}


void BarsTabWidget::handleLink( bool enabled )
{
   mpPlayButton->setEnabled( enabled );
   mpLoopButton->setEnabled( enabled );
   if( enabled )
   {
      mpVoice1->setBar( mpVoice0->bar() );
   }
}


void BarsTabWidget::handlePlay( bool enabled )
{
   if( enabled )
   {
      emit startPlay( mpVoice0->bar() );
   }
}
