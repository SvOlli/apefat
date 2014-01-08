/**
 * SongTabWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SongTabWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QScrollBar>
#include <QSpinBox>

#include <QStringListModel>

/* local library headers */
#include <json.h>

/* local headers */
#include "BarWidget.hpp"
#include "HiHatWidget.hpp"
#include "SoundWidget.hpp"
#include "SlocumSong.hpp"
#include "VoiceWidget.hpp"


#include <QtDebug>


SongTabWidget::SongTabWidget( SlocumSong *slocumSong, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mpHiHatWidget( new HiHatWidget( mpSlocumSong, this ) )
, mpSoundWidget( new SoundWidget( mpSlocumSong, this ) )
, mpDelayName( new QLabel( this ) )
, mpDelayValue( new QSpinBox( this ) )
, mpLinkButton( new QPushButton( this ) )
, mpUniqueInfo( new QLabel( this ) )
, mpVoice0( new VoiceWidget( mpSlocumSong, 0, this ) )
, mpVoice1( new VoiceWidget( mpSlocumSong, 1, this ) )
{
   mpDelayValue->setRange( 1, 16 );
   mpDelayValue->setAlignment( Qt::AlignRight );
   mpLinkButton->setCheckable( true );
   mpLinkButton->setChecked( true );

   QBoxLayout *songLayout = new QVBoxLayout( this );
   QBoxLayout *voiceLayout = new QHBoxLayout();
   QBoxLayout *speedLayout = new QHBoxLayout();

#if 0
   mpVoice0->setVerticalScrollBar( mpVoice1->verticalScrollBar() );
   mpVoice1->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   mpVoice0->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   QScrollBar *voiceScrollBar = mpVoice0->verticalScrollBar();
#else
   mpVoice0->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
   mpVoice1->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
#endif

   songLayout->setContentsMargins( 0, 0, 0, 0 );
   voiceLayout->setContentsMargins( 0, 0, 0, 0 );
   voiceLayout->setSpacing( 0 );
   songLayout->addWidget( mpHiHatWidget );
   songLayout->addWidget( mpSoundWidget );

   speedLayout->addWidget( mpDelayName );
   speedLayout->addWidget( mpDelayValue );
   speedLayout->addWidget( mpLinkButton );
   speedLayout->addWidget( mpUniqueInfo );
   speedLayout->addStretch( 1 );
   songLayout->addLayout( speedLayout );

   voiceLayout->addWidget( mpVoice0 );
   //voiceLayout->addWidget( voiceScrollBar );
   voiceLayout->addWidget( mpVoice1 );
   songLayout->addLayout( voiceLayout );

#if 0
   mpVoice0->addLinked( mpVoice1 );
   mpVoice1->addLinked( mpVoice0 );
#endif

#if 0
   connect( mpVoice0, SIGNAL(sizeChanged()),
            this, SLOT(setFromSong()) );
   connect( mpVoice1, SIGNAL(sizeChanged()),
            this, SLOT(setFromSong()) );
#endif
   connect( mpDelayValue, SIGNAL(valueChanged(int)),
            this, SLOT(setSongDelay(int)) );
   connect( mpVoice0->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(changeSliders(int)) );
   connect( mpVoice1->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(changeSliders(int)) );
   connect( mpVoice0, SIGNAL(sizeChanged()),
            this, SLOT(cleanup()) );
   connect( mpVoice1, SIGNAL(sizeChanged()),
            this, SLOT(cleanup()) );
   connect( mpLinkButton, SIGNAL(clicked()),
            this, SLOT(cleanup()) );
   setTexts();
}


SongTabWidget::~SongTabWidget()
{
}


void SongTabWidget::setTexts()
{
   mpDelayName->setText( tr("Delay:") );
   mpLinkButton->setText( tr("Link") );
   mpHiHatWidget->setTexts();
   mpSoundWidget->setTexts();
   mpUniqueInfo->setText( tr("Unique Patterns: normal %1 / low %2")
                          .arg( QString::number(mpSlocumSong->countUniqueBars(false)))
                          .arg( QString::number(mpSlocumSong->countUniqueBars(true))) );
}


void SongTabWidget::setFromSong( SlocumSong *slocumSong )
{
   if( slocumSong )
   {
      mpSlocumSong = slocumSong;
   }
   mpDelayValue->setValue( mpSlocumSong->delay() );
   mpHiHatWidget->setFromSong( mpSlocumSong );
   mpSoundWidget->setFromSong( mpSlocumSong );
   mpVoice0->setFromSong( mpSlocumSong );
   mpVoice1->setFromSong( mpSlocumSong );
}


void SongTabWidget::cleanup()
{
   if( mpLinkButton->isChecked() )
   {
      VoiceWidget *shortVoice = 0;
      VoiceWidget *longVoice  = 0;
      if( mpVoice0->count() == mpVoice1->count() )
      {
         return;
      }
      else if( mpVoice0->count() < mpVoice1->count() )
      {
         shortVoice = mpVoice0;
         longVoice  = mpVoice1;
      }
      else
      {
         shortVoice = mpVoice1;
         longVoice  = mpVoice0;
      }
      longVoice->setCount( shortVoice->count() );
      shortVoice->setCount( longVoice->count() );
   }
}


void SongTabWidget::changeSliders( int value )
{
   if( mpLinkButton->isChecked() )
   {
      mpVoice0->verticalScrollBar()->setValue( value );
      mpVoice1->verticalScrollBar()->setValue( value );
   }
}


void SongTabWidget::setSongDelay( int value )
{
   mpSlocumSong->setDelay( value );
}
