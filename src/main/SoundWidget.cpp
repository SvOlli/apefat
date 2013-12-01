/**
 * SoundWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SoundWidget.hpp"

/* system headers */

/* Qt headers */
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSpinBox>
#include <QTimer>

/* local library headers */

/* local headers */
#include "SlocumSong.hpp"
#include "SoundComboBox.hpp"


#include <QtDebug>


SoundWidget::SoundWidget( SlocumSong *slocumSong, QWidget *parent )
: QGroupBox( parent )
, mpSlocumSong( slocumSong )
, mpTypesMapper( new QSignalMapper( this ) )
, mpAttensMapper( new QSignalMapper( this ) )
, mpMainLayout( new QGridLayout( this ) )
, mpLabelName( new QLabel( this ) )
, mpLabelType( new QLabel( this ) )
, mpLabelAtten( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mTypes()
, mAttens()
{
   mpMainLayout->addWidget( mpLabelName,  0, 0 );
   mpMainLayout->addWidget( mpValueName,  0, 1, 1, mpSlocumSong->sound()->size() );
   mpMainLayout->addWidget( mpLabelType,  2, 0 );
   mpMainLayout->addWidget( mpLabelAtten, 3, 0 );

   for( int i = 0; i < mpSlocumSong->sound()->size(); ++i )
   {
      QLabel        *header = new QLabel( QString::number(i), this );
      SoundComboBox *type   = new SoundComboBox( this );
      QSpinBox      *atten  = new QSpinBox( this );
      header->setAlignment( Qt::AlignCenter );
      atten->setAlignment( Qt::AlignRight );
      atten->setRange( 0, 15 );
      mpMainLayout->setColumnStretch( 1 + i, 1 );
      mpMainLayout->addWidget( header, 1, 1 + i );
      mpMainLayout->addWidget( type,   2, 1 + i );
      mpMainLayout->addWidget( atten,  3, 1 + i );
      mTypes.append( type );
      mAttens.append( atten );
      mpTypesMapper->setMapping(  type,  i );
      mpAttensMapper->setMapping( atten, i );
      connect( type, SIGNAL(currentIndexChanged(int)),
               mpTypesMapper, SLOT(map()) );
      connect( atten, SIGNAL(valueChanged(int)),
               mpAttensMapper, SLOT(map()) );
   }
   connect( mpValueName, SIGNAL(textChanged(QString)),
            this, SLOT(setName(QString)) );
   setTexts();
   setFromSong( mpSlocumSong );
}


SoundWidget::~SoundWidget()
{
}


void SoundWidget::setTexts()
{
   setTitle( tr("Sound:") );
   mpLabelName->setText( tr("Name:") );
   mpLabelType->setText( tr("Type:") );
   mpLabelAtten->setText( tr("Atten:") );
}


void SoundWidget::setName( const QString &setname )
{
   mpSlocumSong->sound()->name = setname;
}


void SoundWidget::typeChanged( int index )
{
   mpSlocumSong->sound()->type[index] = mTypes.at(index)->value() & 0xff;
}


void SoundWidget::attensChanged( int index )
{
   mpSlocumSong->sound()->atten[index] = mAttens.at(index)->value() & 0xff;
}


void SoundWidget::setFromSong( SlocumSong *slocumSong )
{
   if( slocumSong )
   {
      mpSlocumSong = slocumSong;
   }
   SlocumSound *sound = mpSlocumSong->sound();
   mpValueName->setText( sound->name );
   for( int i = 0; i < sound->size(); ++i )
   {
      mTypes.at(i)->setValue( sound->type[i]  );
      mAttens.at(i)->setValue( sound->atten[i] );
   }
}
