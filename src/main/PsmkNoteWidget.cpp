/**
 * PsmkNoteWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkNoteWidget.hpp"

/* system headers */
#include <cstring>

/* Qt headers */
#include <QBoxLayout>
#include <QCheckBox>
#include <QDrag>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPicture>

/* local library headers */
#include <json.h>

/* local headers */
#include "PsmkInstrumentComboBox.hpp"
#include "PsmkPitchComboBox.hpp"
#include "PsmkContextMenu.hpp"


#include <QtDebug>
#include <QApplication>
#include <QClipboard>


const char PsmkNoteWidget::cMimeType[] = "x-apefat-note";
bool       PsmkNoteWidget::cDragImage  = false;


PsmkNoteWidget::PsmkNoteWidget( int index, QWidget *parent )
: QWidget( parent )
, mIndex( index )
, mpLayout( new QHBoxLayout( this ) )
, mpInstrument( new PsmkInstrumentComboBox( this ) )
, mpPitch( new PsmkPitchComboBox( this ) )
, mpAccent( new QCheckBox( this ) )
, mpTypeLabel( new QLabel( this ) )
{
   setup();
}


PsmkNoteWidget::PsmkNoteWidget( int index, quint8 *instruments, QWidget *parent )
: QWidget( parent )
, mIndex( index )
, mpLayout( new QHBoxLayout( this ) )
, mpInstrument( new PsmkInstrumentComboBox( this ) )
, mpPitch( new PsmkPitchComboBox( this ) )
, mpAccent( new QCheckBox( this ) )
, mpTypeLabel( new QLabel( this ) )
{
   setup( instruments );
}


PsmkNoteWidget::~PsmkNoteWidget()
{
}


void PsmkNoteWidget::setup( quint8 *instruments )
{
   mpTypeLabel->setFrameStyle( QFrame::Box );
   mpTypeLabel->setLineWidth( 1 );
   mpTypeLabel->setMargin( 1 );
   mpAccent->setDisabled( true );
   mpLayout->setContentsMargins( 0, 0, 0, 0 );
   mpLayout->addWidget( mpInstrument );
   mpLayout->addWidget( mpPitch );
   mpLayout->addWidget( mpAccent );
   mpLayout->addWidget( mpTypeLabel ); //! \todo fix this
   mpLayout->setStretch( 0, 4 );
   mpLayout->setStretch( 1, 5 );
   mpLayout->setStretch( 2, 0 );
   //mpLayout->addStretch();
   connect( mpInstrument, SIGNAL(typeChanged(quint8)),
            mpPitch, SLOT(setSound(quint8)) );
   connect( mpInstrument, SIGNAL(isMute(bool)),
            mpPitch, SLOT(setDisabled(bool)) );
   connect( mpInstrument, SIGNAL(isMute(bool)),
            mpAccent, SLOT(setDisabled(bool)) );
   connect( mpInstrument, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkInvalid()) );
   connect( mpPitch, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkInvalid()) );
   connect( mpAccent, SIGNAL(clicked()),
            this, SIGNAL(changed()) );
   // workaround for setting notelist in pitch correctly
   mpInstrument->setInstruments( instruments );
   mpInstrument->setValue( 255 );
   setTexts();
}


const char *PsmkNoteWidget::mimeType()
{
   return &cMimeType[0];
}


int PsmkNoteWidget::index() const
{
   return mIndex;
}


void PsmkNoteWidget::setCreateDragImage( bool on )
{
   cDragImage = on;
}


QVariantMap PsmkNoteWidget::toVariantMap() const
{
   QVariantMap variantMap;

   variantMap.insert( "instrument", mpInstrument->value() );
   variantMap.insert( "pitch",      mpPitch->value() );
   variantMap.insert( "accent",     mpAccent->isChecked() );

   return variantMap;
}


bool PsmkNoteWidget::fromVariantMap( const QVariantMap &variantMap )
{
   bool ok;

   // for backward compability
   if( variantMap.contains( "sound" ) )
   {
      mpInstrument->setValue( variantMap.value( "sound" ).toUInt( &ok )  );
   }
   else
   {
      Q_ASSERT( variantMap.contains( "instrument" ) );
      mpInstrument->setValue( variantMap.value( "instrument" ).toUInt( &ok )  );
      Q_ASSERT( ok );
   }

   Q_ASSERT( variantMap.contains("pitch") );
   mpPitch->setValue( variantMap.value( "pitch" ).toUInt( &ok ) );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains("accent") );
   mpAccent->setChecked( variantMap.value( "accent" ).toBool() );

   return true;
}


QByteArray PsmkNoteWidget::toBinary() const
{
   QByteArray retval;

   if( mpInstrument->value() == 255 )
   {
      retval.append( (char)255 );
   }
   else
   {
      retval.append( (mpInstrument->value() << 5)
                     | mpPitch->value() );
   }
   if( mpAccent->isChecked() )
   {
      retval.append( 1 << (7-mIndex) );
   }
   else
   {
      retval.append( (char)0 );
   }
   return retval;
}


bool PsmkNoteWidget::accent() const
{
   return mpAccent->isChecked();
}


void PsmkNoteWidget::setInstrument( int index, quint8 tone )
{
   mpInstrument->setTone( index, tone );
}


void PsmkNoteWidget::setInstruments( quint8 tones[] )
{
   mpInstrument->setTones( tones );
}


void PsmkNoteWidget::setTexts()
{
   mpTypeLabel->setText( tr("Note") );
}


void PsmkNoteWidget::checkInvalid()
{
   if( (mpInstrument->value() == 7) && (mpPitch->value() == 31) )
   {
      mpInstrument->setValue( 255 );
   }
   emit changed();
}


void PsmkNoteWidget::mousePressEvent( QMouseEvent *event )
{
   QMimeData *mimeData = new QMimeData();
   mimeData->setData( PsmkNoteWidget::mimeType(), QtJson::serialize( toVariantMap() ) );
   mimeData->setData( "index", QByteArray::number(mIndex) );
   // for debugging only:
   //mimeData->setText( QString::fromUtf8(mimeData->data( PsmkNoteWidget::mimeType() )) );

   QWidget *w = focusWidget();
   if( w )
   {
      w->clearFocus();
   }
   QDrag *drag = new QDrag( this );
   drag->setMimeData( mimeData );
   if( cDragImage )
   {
      QPixmap pixmap( width()+2, height()+2 );
      pixmap.fill( QColor(0,0,0) );
      QPainter painter;
      painter.begin( &pixmap );
      painter.drawImage( 1, 1, QPixmap::grabWidget( this ).toImage() );
      painter.end();
      drag->setPixmap( pixmap );
      drag->setHotSpot( QPoint( event->x()+1, event->y()+1 ) );
   }
   // Qt::DropAction action =
   drag->exec( Qt::CopyAction | Qt::MoveAction, Qt::MoveAction );
   event->accept();
}


void PsmkNoteWidget::contextMenuEvent( QContextMenuEvent *event )
{
   PsmkContextMenu menu( tr("Note"), cMimeType, toVariantMap(), this );
   menu.removeStore();

   if( menu.runSelect( event->globalPos() - QPoint( 5, 5 ) ) == PsmkContextMenu::Paste )
   {
      fromVariantMap( menu.mimeData().toMap() );
   }
}
