/**
 * BarPreviewWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "BarPreviewWidget.hpp"

/* system headers */
#include <cstring>

/* Qt headers */
#include <QBoxLayout>
#include <QCheckBox>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPicture>

/* local library headers */
#include "json.h"

/* local headers */
#include "PitchComboBox.hpp"
#include "SlocumSong.hpp"
#include "SoundComboBox.hpp"


#include <QtDebug>


const char BarPreviewWidget::cMimeType[] = "x-vcs-tracker-bar";
bool       BarPreviewWidget::cDragImage  = false;


BarPreviewWidget::BarPreviewWidget( const SlocumSong *slocumSong, int channel, int index, QWidget *parent )
: QWidget( parent )
, mChannel( channel )
, mIndex( index )
{
}


BarPreviewWidget::~BarPreviewWidget()
{
}


const char *BarPreviewWidget::mimeType()
{
   return &cMimeType[0];
}


int BarPreviewWidget::index() const
{
   return mIndex;
}


void BarPreviewWidget::mousePressEvent( QMouseEvent *event )
{
   QMimeData *mimeData = new QMimeData();
   //mimeData->setData( BarPreviewWidget::mimeType(), QtJson::serialize( mpSlocumNote->toVariantMap() ) );
   mimeData->setData( "index", QByteArray::number(mIndex) );
   // for debugging only:
   //mimeData->setText( QString::fromUtf8(mimeData->data( BarPreviewWidget::mimeType() )) );

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


void BarPreviewWidget::paintEvent( QPaintEvent *event )
{
   QWidget::paintEvent( event );

   QPainter painter;
   painter.begin( this );
   painter.setPen( QColor( 0, 0, 0 ) );
   painter.drawRect( 0, 0, width()-1, height()-1 );
   painter.end();
}


void BarPreviewWidget::setFromSong( const SlocumSong *slocumSong, int channel, int index )
{
   mpSlocumSong = slocumSong;
   mChannel   = channel;
   mIndex     = index;
}
