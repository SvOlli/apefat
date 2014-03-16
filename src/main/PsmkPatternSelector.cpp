/**
 * PsmkPatternSelector.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPatternSelector.hpp"

/* system headers */

/* Qt headers */
#include <QCheckBox>
#include <QCommonStyle>
#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QToolButton>

/* local library headers */

/* local headers */
#include "PsmkPatternWidget.hpp"


PsmkPatternSelector::PsmkPatternSelector( QWidget *parent )
: QWidget( parent )
, mpPositionText( new QLabel( this ) )
, mpFirstButton( new QToolButton( this ) )
, mpPreviousButton( new QToolButton( this ) )
, mpNextButton( new QToolButton( this ) )
, mpLastButton( new QToolButton( this ) )
, mpAddBeforeButton( new QToolButton( this ) )
, mpAddAfterButton( new QToolButton( this ) )
, mpVoice0Stack( new QStackedWidget( this ) )
, mpVoice1Stack( new QStackedWidget( this ) )
, mpHiHatStack( new QStackedWidget( this ) )
{
   setup();
}


PsmkPatternSelector::~PsmkPatternSelector()
{
}


void PsmkPatternSelector::setup()
{
   QCommonStyle style;
   mpFirstButton->setIcon( style.standardIcon( QStyle::SP_MediaSkipBackward ) );
   mpPreviousButton->setIcon( style.standardIcon( QStyle::SP_MediaSeekBackward ) );
   mpNextButton->setIcon( style.standardIcon( QStyle::SP_MediaSeekForward ) );
   mpLastButton->setIcon( style.standardIcon( QStyle::SP_MediaSkipForward ) );
   mpAddBeforeButton->setIcon( style.standardIcon( QStyle::SP_FileDialogNewFolder ) );
   mpAddAfterButton->setIcon( style.standardIcon( QStyle::SP_FileDialogNewFolder ) );

   QBoxLayout *mainLayout   = new QVBoxLayout( this );
   QBoxLayout *buttonLayout = new QHBoxLayout();
   QBoxLayout *voicesLayout = new QHBoxLayout();

   buttonLayout->addWidget( mpFirstButton );
   buttonLayout->addWidget( mpAddBeforeButton );
   buttonLayout->addWidget( mpPreviousButton );
   buttonLayout->addWidget( mpPositionText, 1 );
   buttonLayout->addWidget( mpHiHatStack );
   buttonLayout->addWidget( mpNextButton );
   buttonLayout->addWidget( mpAddAfterButton );
   buttonLayout->addWidget( mpLastButton );

   voicesLayout->addWidget( mpVoice0Stack );
   voicesLayout->addWidget( mpVoice1Stack );

   mainLayout->addLayout( buttonLayout );
   mainLayout->addLayout( voicesLayout );

   connect( mpFirstButton, SIGNAL(clicked()),
            this, SLOT(moveFirst()) );
   connect( mpPreviousButton, SIGNAL(clicked()),
            this, SLOT(movePrevious()) );
   connect( mpNextButton, SIGNAL(clicked()),
            this, SLOT(moveNext()) );
   connect( mpLastButton, SIGNAL(clicked()),
            this, SLOT(moveLast()) );
   connect( mpAddBeforeButton, SIGNAL(clicked()),
            this, SLOT(insertBefore()) );
   connect( mpAddAfterButton, SIGNAL(clicked()),
            this, SLOT(insertAfter()) );
   connect( mpVoice0Stack, SIGNAL(currentChanged(int)),
            mpVoice1Stack, SLOT(setCurrentIndex(int)) );
   connect( mpVoice0Stack, SIGNAL(currentChanged(int)),
            mpHiHatStack, SLOT(setCurrentIndex(int)) );

   insert( 0 );
   setTexts();
}


void PsmkPatternSelector::clear()
{
   for( int i = 0; i < mpVoice0Stack->count(); ++i )
   {
      mpVoice0Stack->widget( i )->deleteLater();
      mpVoice1Stack->widget( i )->deleteLater();
      mpHiHatStack->widget( i )->deleteLater();
   }
}


void PsmkPatternSelector::insert( int pos )
{
   PsmkPatternWidget *voice0 = new PsmkPatternWidget( this );
   PsmkPatternWidget *voice1 = new PsmkPatternWidget( this );
   QCheckBox *hihat = new QCheckBox( tr("HiHat"), this );
   mpVoice0Stack->insertWidget( pos, voice0 );
   mpVoice1Stack->insertWidget( pos, voice1 );
   mpHiHatStack->insertWidget( pos, hihat );
   connect( voice0, SIGNAL(changed()),
            this, SIGNAL(changed()) );
   connect( voice1, SIGNAL(changed()),
            this, SIGNAL(changed()) );
   connect( hihat, SIGNAL(toggled(bool)),
            this, SIGNAL(changed()) );
   setPattern( pos );
}


void PsmkPatternSelector::setTexts()
{
   mpPositionText->setText( tr("Pattern %1/%2")
                            .arg( mpVoice0Stack->currentIndex() + 1, 3, 10, QChar('0') )
                            .arg( mpVoice0Stack->count(), 3, 10, QChar('0') )
                            );
   mpFirstButton->setToolTip( tr("First Pattern") );
   mpPreviousButton->setToolTip( tr("Previous Pattern") );
   mpNextButton->setToolTip( tr("Next Pattern") );
   mpLastButton->setToolTip( tr("Last Pattern") );
   mpAddBeforeButton->setToolTip( tr("Insert Before Current") );
   mpAddAfterButton->setToolTip( tr("Insert After Current") );
   for( int i = 0; i < mpHiHatStack->count(); ++i )
   {
      QCheckBox *c = qobject_cast<QCheckBox*>( mpHiHatStack->widget(i) );
      c->setText( tr("HiHat") );
   }
}


void PsmkPatternSelector::setPattern( int pattern )
{
   int patterns = mpVoice0Stack->count();
   if( pattern >= patterns )
   {
      pattern = patterns - 1;
   }
   if( pattern < 0 )
   {
      pattern = 0;
   }
   mpFirstButton->setDisabled( pattern == 0 );
   mpPreviousButton->setDisabled( pattern == 0 );
   mpNextButton->setDisabled( pattern >= (patterns - 1) );
   mpLastButton->setDisabled( pattern >= (patterns - 1) );
   mpAddBeforeButton->setDisabled( patterns >= 256 );
   mpAddAfterButton->setDisabled( patterns >= 256 );

   mpVoice0Stack->setCurrentIndex( pattern );
   setTexts();
}


QVariantList PsmkPatternSelector::toVariantList() const
{
   QVariantList list;
   for( int i = 0; i < mpVoice0Stack->count(); ++i )
   {
      QVariantMap map;
      PsmkPatternWidget *voice0 = qobject_cast<PsmkPatternWidget*>( mpVoice0Stack->widget( i ) );
      PsmkPatternWidget *voice1 = qobject_cast<PsmkPatternWidget*>( mpVoice1Stack->widget( i ) );
      QCheckBox         *hihat  = qobject_cast<QCheckBox*>( mpHiHatStack->widget( i ) );
      Q_ASSERT( voice0 );
      Q_ASSERT( voice1 );
      Q_ASSERT( hihat  );
      map.insert( "voice0", voice0->toVariantMap() );
      map.insert( "voice1", voice1->toVariantMap() );
      map.insert( "hihat",  hihat->isChecked() );
      list.append( map );
   }
   return list;
}


bool PsmkPatternSelector::fromVariantList( const QVariantList &variantList )
{
   bool retval = true;
   clear();
   foreach( const QVariant &entry, variantList )
   {
      QVariantMap map( entry.toMap() );
      PsmkPatternWidget *voice0 = new PsmkPatternWidget( this );
      PsmkPatternWidget *voice1 = new PsmkPatternWidget( this );
      QCheckBox         *hihat  = new QCheckBox( tr("HiHat"), this );
      connect( voice0, SIGNAL(changed()),
               this, SIGNAL(changed()) );
      connect( voice1, SIGNAL(changed()),
               this, SIGNAL(changed()) );
      connect( hihat, SIGNAL(toggled(bool)),
               this, SIGNAL(changed()) );
      mpVoice0Stack->addWidget( voice0 );
      mpVoice1Stack->addWidget( voice1 );
      mpHiHatStack->addWidget( hihat  );
      if( !map.contains("voice0") ||
          !map.contains("voice1") ||
          !map.contains("hihat") )
      {
         return false;
      }
      retval &= voice0->fromVariantMap( map.value("voice0").toMap() );
      retval &= voice1->fromVariantMap( map.value("voice1").toMap() );
      hihat->setChecked( map.value("").toBool() );
   }
   setPattern( 0 );
   return retval;
}


void PsmkPatternSelector::moveFirst()
{
   setPattern( 0 );
}


void PsmkPatternSelector::movePrevious()
{
   setPattern( mpVoice0Stack->currentIndex() - 1 );
}


void PsmkPatternSelector::moveNext()
{
   setPattern( mpVoice0Stack->currentIndex() + 1 );
}


void PsmkPatternSelector::moveLast()
{
   setPattern( mpVoice0Stack->count() - 1 );
}


void PsmkPatternSelector::insertBefore()
{
   if( mpVoice0Stack->count() < 256 )
   {
      insert( mpVoice0Stack->currentIndex() );
   }
}


void PsmkPatternSelector::insertAfter()
{
   if( mpVoice0Stack->count() < 256 )
   {
      insert( mpVoice0Stack->currentIndex() + 1 );
   }
}


void PsmkPatternSelector::instrumentChanged( int index, quint8 value )
{
   Q_ASSERT( mpVoice0Stack->count() == mpVoice1Stack->count() );
   for( int i = 0; i < mpVoice0Stack->count(); ++i )
   {
      PsmkPatternWidget *v0 = qobject_cast<PsmkPatternWidget*>( mpVoice0Stack->widget( i ) );
      PsmkPatternWidget *v1 = qobject_cast<PsmkPatternWidget*>( mpVoice1Stack->widget( i ) );
      v0->setInstrument( index, value );
      v1->setInstrument( index, value );
   }
}
