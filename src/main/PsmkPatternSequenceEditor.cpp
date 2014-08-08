/**
 * PsmkPatternSequenceEditor.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPatternSequenceEditor.hpp"

/* system headers */

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextEdit>
#endif
#include <QClipboard>
#include <QMimeData>
#include <QSignalMapper>

/* local library headers */

/* local headers */
#include "PsmkBeatWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkNoteWidget.hpp"
#include "PsmkPatternWidget.hpp"
#include "PsmkPatternProxyWidget.hpp"


#include <QtDebug>


const int PsmkPatternSequenceEditor::LABELROW      = 1;
const int PsmkPatternSequenceEditor::LABELCOL      = 0;
const int PsmkPatternSequenceEditor::SWAP0ROW      = 0;
const int PsmkPatternSequenceEditor::SWAP0COL      = 1;
const int PsmkPatternSequenceEditor::SWAPALLROW    = 0;
const int PsmkPatternSequenceEditor::SWAPALLCOL    = 3;
const int PsmkPatternSequenceEditor::SWAP1ROW      = 0;
const int PsmkPatternSequenceEditor::SWAP1COL      = 5;
const int PsmkPatternSequenceEditor::PROXY0ROW     = 1;
const int PsmkPatternSequenceEditor::PROXY0COL     = 1;
const int PsmkPatternSequenceEditor::CLONEROW      = 1;
const int PsmkPatternSequenceEditor::CLONECOL      = 2;
const int PsmkPatternSequenceEditor::SWAP01ROW     = 1;
const int PsmkPatternSequenceEditor::SWAP01COL     = 3;
const int PsmkPatternSequenceEditor::REMOVEROW     = 1;
const int PsmkPatternSequenceEditor::REMOVECOL     = 4;
const int PsmkPatternSequenceEditor::PROXY1ROW     = 1;
const int PsmkPatternSequenceEditor::PROXY1COL     = 5;
const int PsmkPatternSequenceEditor::HIHATROW      = 1;
const int PsmkPatternSequenceEditor::HIHATCOL      = 6;

PsmkPatternSequenceEditor::PsmkPatternSequenceEditor( const QList<PsmkPatternWidget *> &voice0,
                                                      const QList<PsmkPatternWidget *> &voice1,
                                                      const QList<bool> &hihat,
                                                      const PsmkPacker *psmkPacker,
                                                      QWidget *parent )
: QWidget( parent )
, mpLayout( new QGridLayout( this ) )
, mpCloneMapper( new QSignalMapper( this ) )
, mpRemoveMapper( new QSignalMapper( this ) )
, mpSwap0Mapper( new QSignalMapper( this ) )
, mpSwap1Mapper( new QSignalMapper( this ) )
, mpSwapAllMapper( new QSignalMapper( this ) )
, mpSwap01Mapper( new QSignalMapper( this ) )
, mpPsmkPacker( psmkPacker )
, mProxy0Widgets()
, mProxy1Widgets()
, mHiHatWidgets()
, mCloneButtons()
, mRemoveButtons()
, mSwap0Buttons()
, mSwap1Buttons()
, mSwap01Buttons()
, mSwapAllButtons()
{
   Q_ASSERT( mProxy0Widgets.size() == mProxy1Widgets.size() );

   mpLayout->setRowStretch( 0, 1 );
   mpLayout->setRowStretch( 4, 1 );
   for( int i = 0; i < voice0.size(); ++i )
   {
      insertLine( i, voice0.at(i), voice1.at(i), hihat.at(i) );
   }

   connect( mpCloneMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(handleClone(QWidget*)) );
   connect( mpRemoveMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(handleRemove(QWidget*)) );
   connect( mpSwap0Mapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(handleSwap0(QWidget*)) );
   connect( mpSwap1Mapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(handleSwap1(QWidget*)) );
   connect( mpSwap01Mapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(handleSwap01(QWidget*)) );
   connect( mpSwapAllMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(handleSwapAll(QWidget*)) );

   updateLayout();
   setTexts();
}


PsmkPatternSequenceEditor::~PsmkPatternSequenceEditor()
{
}


QList<PsmkPatternWidget *> PsmkPatternSequenceEditor::getVoicePatterns( int voice ) const
{
   QList<PsmkPatternWidget*> retval;
   switch( voice )
   {
   case 0:
      foreach( PsmkPatternProxyWidget *proxy, mProxy0Widgets )
      {
         retval.append( proxy->patternWidget() );
      }
      break;
   case 1:
      foreach( PsmkPatternProxyWidget *proxy, mProxy1Widgets )
      {
         retval.append( proxy->patternWidget() );
      }
      break;
   default:
      break;
   }
   return retval;
}


QList<bool> PsmkPatternSequenceEditor::getHiHat() const
{
   QList<bool> retval;
   foreach( QCheckBox *checkbox, mHiHatWidgets )
   {
      retval.append( checkbox->isChecked() );
   }
   return retval;
}


void PsmkPatternSequenceEditor::insertLine( int index,
                                            PsmkPatternWidget *voice0,
                                            PsmkPatternWidget *voice1,
                                            bool hihat )
{
   QPushButton *swap0   = new QPushButton( tr("^ Swap v"), this );
   QPushButton *swap1   = new QPushButton( tr("^ Swap v"), this );
   QPushButton *swapAll = new QPushButton( tr("^ Swap All v"), this );
   if( !index )
   {
      swap0->hide();
      swap1->hide();
      swapAll->hide();
   }
   QPushButton *clone  = new QPushButton( tr("Clone"), this );
   QPushButton *swap01 = new QPushButton( tr("<- Swap ->"), this );
   QPushButton *remove = new QPushButton( tr("Remove"), this );
   Q_ASSERT( voice0 );
   Q_ASSERT( voice1 );
   QLabel *label = new QLabel( this );
   label->setAlignment( Qt::AlignVCenter | Qt::AlignRight );
   PsmkPatternProxyWidget *proxy0 = new PsmkPatternProxyWidget( voice0, mpPsmkPacker, this );
   PsmkPatternProxyWidget *proxy1 = new PsmkPatternProxyWidget( voice1, mpPsmkPacker, this );
   QCheckBox              *phihat = new QCheckBox( tr("HiHat"), this );
   phihat->setChecked( hihat );

   mLabels.insert( index, label );
   mSwap0Buttons.insert( index, swap0 );
   mSwap1Buttons.insert( index, swap1 );
   mSwapAllButtons.insert( index, swapAll );
   mCloneButtons.insert( index, clone );
   mSwap01Buttons.insert( index, swap01 );
   mRemoveButtons.insert( index, remove );
   mProxy0Widgets.insert( index, proxy0 );
   mProxy1Widgets.insert( index, proxy1 );
   mHiHatWidgets.insert( index, phihat );

   mpCloneMapper->setMapping( clone, clone );
   connect( clone, SIGNAL(clicked()),
            mpCloneMapper, SLOT(map()) );
   mpRemoveMapper->setMapping( remove, remove );
   connect( remove, SIGNAL(clicked()),
            mpRemoveMapper, SLOT(map()) );
   mpSwap0Mapper->setMapping( swap0, swap0 );
   connect( swap0, SIGNAL(clicked()),
            mpSwap0Mapper, SLOT(map()) );
   mpSwap1Mapper->setMapping( swap1, swap1 );
   connect( swap1, SIGNAL(clicked()),
            mpSwap1Mapper, SLOT(map()) );
   mpSwap01Mapper->setMapping( swap01, swap01 );
   connect( swap01, SIGNAL(clicked()),
            mpSwap01Mapper, SLOT(map()) );
   mpSwapAllMapper->setMapping( swapAll, swapAll );
   connect( swapAll, SIGNAL(clicked()),
            mpSwapAllMapper, SLOT(map()) );
}


void PsmkPatternSequenceEditor::updateLayout()
{
   for( int i = mProxy0Widgets.size() - 1; i >= 0; --i )
   {
      mSwap0Buttons.at(i)->show();
      mSwap1Buttons.at(i)->show();
      mRemoveButtons.at(i)->setEnabled( true );
      mLabels.at(i)->setText( tr("%1:").arg( i + 1 ) );
      mpLayout->addWidget( mLabels.at(i),         LABELROW   + i * 2, LABELCOL );
      mpLayout->addWidget( mSwap0Buttons.at(i),   SWAP0ROW   + i * 2, SWAP0COL );
      mpLayout->addWidget( mSwap1Buttons.at(i),   SWAP1ROW   + i * 2, SWAP1COL );
      mpLayout->addWidget( mSwapAllButtons.at(i), SWAPALLROW + i * 2, SWAPALLCOL );
      mpLayout->addWidget( mProxy0Widgets.at(i),  PROXY0ROW  + i * 2, PROXY0COL );
      mpLayout->addWidget( mCloneButtons.at(i),   CLONEROW   + i * 2, CLONECOL );
      mpLayout->addWidget( mSwap01Buttons.at(i),  SWAP01ROW  + i * 2, SWAP01COL );
      mpLayout->addWidget( mRemoveButtons.at(i),  REMOVEROW  + i * 2, REMOVECOL );
      mpLayout->addWidget( mProxy1Widgets.at(i),  PROXY1ROW  + i * 2, PROXY1COL );
      mpLayout->addWidget( mHiHatWidgets.at(i),   HIHATROW   + i * 2, HIHATCOL );
   }
   mSwap0Buttons.at(0)->hide();
   mSwap1Buttons.at(0)->hide();
   mSwapAllButtons.at(0)->hide();
   if( mRemoveButtons.size() == 1 )
   {
      mRemoveButtons.at(0)->setEnabled( false );
   }
   setMinimumSize( sizeHint() );
}


void PsmkPatternSequenceEditor::setTexts()
{
}


PsmkPatternWidget *PsmkPatternSequenceEditor::getVoice( int row, int column )
{
   QLayoutItem *item = mpLayout->itemAtPosition( row, column );
   Q_ASSERT( item );
   QWidget *widget = item->widget();
   Q_ASSERT( widget );
   PsmkPatternProxyWidget *proxy = qobject_cast<PsmkPatternProxyWidget*>( widget );
   Q_ASSERT( proxy );
   return proxy->patternWidget();
}


void PsmkPatternSequenceEditor::handleClone( QWidget *widget )
{
   QPushButton *button = qobject_cast<QPushButton*>( widget );
   Q_ASSERT( button );
   int myline = mCloneButtons.indexOf( button );
   PsmkPatternWidget *voice0 = getVoice( PROXY0ROW + myline * 2, PROXY0COL )->getClone( this );
   PsmkPatternWidget *voice1 = getVoice( PROXY1ROW + myline * 2, PROXY1COL )->getClone( this );
   insertLine( myline, voice0, voice1, mHiHatWidgets.at( myline )->isChecked() );
   updateLayout();
}


void PsmkPatternSequenceEditor::handleRemove( QWidget *widget )
{
   QPushButton *button = qobject_cast<QPushButton*>( widget );
   Q_ASSERT( button );
   int myline = mRemoveButtons.indexOf( button );
   QPushButton *swap0   = mSwap0Buttons.takeAt(myline);
   QPushButton *swap1   = mSwap1Buttons.takeAt(myline);
   QPushButton *swap01  = mSwap01Buttons.takeAt(myline);
   QPushButton *swapAll = mSwapAllButtons.takeAt(myline);
   QPushButton *clone   = mCloneButtons.takeAt(myline);
   QPushButton *remove  = mRemoveButtons.takeAt(myline);
   PsmkPatternProxyWidget *proxy0 = mProxy0Widgets.takeAt(myline);
   PsmkPatternProxyWidget *proxy1 = mProxy1Widgets.takeAt(myline);
   QCheckBox *hihat = mHiHatWidgets.takeAt(myline);
   QLabel *label = mLabels.takeAt(myline);
   mpLayout->removeWidget( swap0 );
   mpLayout->removeWidget( swap1 );
   mpLayout->removeWidget( swapAll );
   mpLayout->removeWidget( swap01 );
   mpLayout->removeWidget( clone );
   mpLayout->removeWidget( remove );
   mpLayout->removeWidget( proxy0 );
   mpLayout->removeWidget( proxy1 );
   mpLayout->removeWidget( hihat );
   mpLayout->removeWidget( label );
   swap0->deleteLater();
   swap1->deleteLater();
   swapAll->deleteLater();
   swap01->deleteLater();
   clone->deleteLater();
   remove->deleteLater();
   proxy0->deleteLater();
   proxy1->deleteLater();
   hihat->deleteLater();
   label->deleteLater();
   updateLayout();
}


void PsmkPatternSequenceEditor::handleSwap0( QWidget *widget )
{
   QPushButton *button = qobject_cast<QPushButton*>( widget );
   Q_ASSERT( button );
   int myline = mSwap0Buttons.indexOf( button );
   mProxy0Widgets.swap( myline, myline - 1 );
   updateLayout();
}


void PsmkPatternSequenceEditor::handleSwap1( QWidget *widget )
{
   QPushButton *button = qobject_cast<QPushButton*>( widget );
   Q_ASSERT( button );
   int myline = mSwap1Buttons.indexOf( button );
   mProxy1Widgets.swap( myline, myline - 1 );
   updateLayout();
}


void PsmkPatternSequenceEditor::handleSwap01( QWidget *widget )
{
   QPushButton *button = qobject_cast<QPushButton*>( widget );
   Q_ASSERT( button );
   int myline = mSwap01Buttons.indexOf( button );
   PsmkPatternProxyWidget *proxy0 = mProxy0Widgets.takeAt( myline );
   PsmkPatternProxyWidget *proxy1 = mProxy1Widgets.takeAt( myline );
   mProxy0Widgets.insert( myline, proxy1 );
   mProxy1Widgets.insert( myline, proxy0 );
   updateLayout();
}


void PsmkPatternSequenceEditor::handleSwapAll( QWidget *widget )
{
   QPushButton *button = qobject_cast<QPushButton*>( widget );
   Q_ASSERT( button );
   int myline = mSwapAllButtons.indexOf( button );
   mProxy0Widgets.swap( myline, myline - 1 );
   mProxy1Widgets.swap( myline, myline - 1 );
   mHiHatWidgets.swap( myline, myline - 1 );
   updateLayout();
}
