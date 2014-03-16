#include "QxtLabel.hpp"
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#include <QTime>
#include <QEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>

static const int Vertical_Mask = 0x02;

/*!
    \class QxtLabel
    \inmodule QxtWidgets
    \brief The QxtLabel widget is a label which is able to show elided and rotated plain text.

    QxtLabel is a label which is able to show elided and rotated plain text.
    In addition, QxtLabel provides a signal for clicking.

    \image qxtlabel.png "QxtLabel in action."

    Usage:
    \code
    QxtLabel* label = new QxtLabel(tr("Hello, World!"), this);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setElideMode(Qt::ElideRight);
    label->setRotation(QxtLabel::CounterClockwise);
    connect(label, SIGNAL(clicked()), this, SLOT(labelClicked()));
    \endcode

    \sa QLabel
 */

/*!
    \fn QxtLabel::clicked()

    This signal is emitted whenever the label has been clicked.

    \bold {Note:} A combination of mouse button press and release in shorter
    time than QApplication::doubleClickInterval is considered
    as a click.

    \sa QApplication::doubleClickInterval
 */

/*!
    \fn QxtLabel::textChanged(const QString& text)

    This signal is emitted whenever the \a text has changed.
 */

/*!
    Constructs a new QxtLabel with \a parent and \a flags.
 */
QxtLabel::QxtLabel( QWidget* parent, Qt::WindowFlags flags )
   : QFrame( parent, flags )
   , mAlign( Qt::AlignCenter )
   , mMode( Qt::ElideMiddle )
   , mRot( NoRotation )
{
}

/*!
    Constructs a new QxtLabel with \a text, \a parent and \a flags.
 */
QxtLabel::QxtLabel( const QString& text, QWidget* parent, Qt::WindowFlags flags )
   : QFrame( parent, flags )
   , mText( text )
   , mAlign( Qt::AlignCenter )
   , mMode( Qt::ElideMiddle )
   , mRot( NoRotation )
{
}

/*!
    Destructs the label.
 */
QxtLabel::~QxtLabel()
{
}

/*!
    \property QxtLabel::text
    \brief the text of the label
 */
QString QxtLabel::text() const
{
   return mText;
}

void QxtLabel::setText( const QString& text )
{
   if( mText != text )
   {
      mText = text;
      updateGeometry();
      update();

      emit textChanged( text );
   }
}

/*!
    \property QxtLabel::alignment
    \brief the alignment of the text

    The text is aligned according to this property.
    The default value is Qt::AlignCenter.

    \sa text, Qt::Alignment
 */
Qt::Alignment QxtLabel::alignment() const
{
   return mAlign;
}

void QxtLabel::setAlignment( Qt::Alignment alignment )
{
   if( mAlign != alignment )
   {
      mAlign = alignment;
      update(); // no geometry change, repaint is sufficient
   }
}

/*!
    \property QxtLabel::elideMode
    \brief the elide mode of the text

    The text is elided according to this property.
    The default value is Qt::ElideMiddle.

    \sa text, Qt::TextElideMode
 */
Qt::TextElideMode QxtLabel::elideMode() const
{
   return mMode;
}

void QxtLabel::setElideMode( Qt::TextElideMode mode )
{
   if( mMode != mode )
   {
      mMode = mode;
      updateGeometry();
      update();
   }
}

/*!
    \property QxtLabel::rotation
    \brief the rotation of the label

    The label is rotated according to this property.
    The default value is QxtLabel::NoRotation.

    \sa QxtLabel::Rotation
 */
QxtLabel::Rotation QxtLabel::rotation() const
{
   return mRot;
}

void QxtLabel::setRotation( QxtLabel::Rotation rotation )
{
   if( mRot != rotation )
   {
      QxtLabel::Rotation prev = mRot;
      mRot = rotation;
      switch (rotation)
      {
      case QxtLabel::NoRotation:
      case QxtLabel::UpsideDown:
         if (prev & Vertical_Mask)
         {
            updateGeometry();
         }
         break;

      case QxtLabel::Clockwise:
      case QxtLabel::CounterClockwise:
         if ((prev & Vertical_Mask) == 0)
         {
            updateGeometry();
         }
         break;
      default:
         // nothing to do
         break;
      }
   }
   update();
}

/*!
    \reimp
 */
QSize QxtLabel::sizeHint() const
{
   const QFontMetrics& fm = fontMetrics();
   QSize size( fm.width(mText), fm.height() );
   if( mRot & Vertical_Mask )
   {
      size.transpose();
   }
   return size;
}

/*!
    \reimp
 */
QSize QxtLabel::minimumSizeHint() const
{
   switch( mMode )
   {
   case Qt::ElideNone:
      return sizeHint();
   default:
      {
         const QFontMetrics& fm = fontMetrics();
         QSize size(fm.width("..."), fm.height());
         if (mRot & Vertical_Mask)
         {
            size.transpose();
         }
         return size;
      }
   }
}

/*!
    \reimp
 */
void QxtLabel::paintEvent(QPaintEvent* event)
{
   QFrame::paintEvent(event);
   QPainter p( this );
   p.rotate( mRot );
   QRect r = contentsRect();
#if 1
   r.setWidth( r.width() + 1 + 2 * lineWidth() );
   r.setHeight( r.height() + 1 + 2 * lineWidth() );
#else
   r.setLeft( r.left() + 2 + lineWidth() );
   r.setTop( r.top() + 2 + lineWidth() );
#endif
   switch( mRot )
   {
   case QxtLabel::UpsideDown:
      p.translate( -r.width(), -r.height() );
      break;

   case QxtLabel::Clockwise:
      p.translate( 0, -r.width() );
      break;

   case QxtLabel::CounterClockwise:
      p.translate( -r.height(), 0 );
      break;

   default:
      // nothing to do
      break;
   }

   if (mRot & Vertical_Mask)
   {
      QSize s = r.size();
      s.transpose();
      r = QRect(r.topLeft(), s);
   }

   const QString elidedText = fontMetrics().elidedText( mText, mMode, r.width() );
   p.drawText(r, mAlign, elidedText);
}

/*!
    \reimp
 */
void QxtLabel::changeEvent(QEvent* event)
{
   QFrame::changeEvent(event);
   switch (event->type())
   {
   case QEvent::FontChange:
   case QEvent::ApplicationFontChange:
      updateGeometry();
      update();
      break;
   default:
      // nothing to do
      break;
   }
}

/*!
    \reimp
 */
void QxtLabel::mousePressEvent(QMouseEvent* event)
{
   QFrame::mousePressEvent(event);
   mTime.start();
}

/*!
    \reimp
 */
void QxtLabel::mouseReleaseEvent(QMouseEvent* event)
{
   QFrame::mouseReleaseEvent(event);
   if (mTime.elapsed() < qApp->doubleClickInterval())
   {
      emit clicked();
   }
}
