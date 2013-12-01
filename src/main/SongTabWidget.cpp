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
#include <QScrollArea>

/* local library headers */
#include <json.h>

/* local headers */
#include "BarWidget.hpp"
#include "HiHatWidget.hpp"
#include "SoundWidget.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


SongTabWidget::SongTabWidget( SlocumSong *slocumSong, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mpHiHatWidget( new HiHatWidget( mpSlocumSong, this ) )
, mpSoundWidget( new SoundWidget( mpSlocumSong, this ) )
, mpChannels( new QScrollArea( this ) )
{
   QBoxLayout *songLayout = new QVBoxLayout( this );

   songLayout->setContentsMargins( 0, 0, 0, 0 );
   songLayout->addWidget( mpHiHatWidget );
   songLayout->addWidget( mpSoundWidget );
   songLayout->addWidget( mpChannels );

   setTexts();
}


SongTabWidget::~SongTabWidget()
{
}


void SongTabWidget::setTexts()
{
   mpHiHatWidget->setTexts();
   mpSoundWidget->setTexts();
}


void SongTabWidget::setFromSong( const SlocumSong *slocumSong )
{
   mpHiHatWidget->setFromSong( mpSlocumSong );
   mpSoundWidget->setFromSong( mpSlocumSong );
}
