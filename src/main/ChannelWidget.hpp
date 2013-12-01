/**
 * ChannelWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef CHANNELWIDGET_HPP
#define CHANNELWIDGET_HPP CHANNELWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;
class QLabel;
class QLineEdit;

/* forward declaration of local classes */
class BarWidget;
class SlocumBar;
class SlocumSong;

/*!
 \brief \todo

 UNUSED RIGHT NOW

*/
class ChannelWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief \todo

    \param slocumSong
    \param parent
   */
   explicit ChannelWidget( SlocumSong *slocumSong, QWidget *parent = 0 );
   /*!
    \brief \todo

   */
   virtual ~ChannelWidget();

public slots:
   /*!
    \brief \todo

   */
   void setTexts();
   /*!
    \brief \todo

    \param slocumSong
    \param slocumBar
   */
   void setFromSong( const SlocumSong *slocumSong, SlocumBar *slocumBar );

protected:
   // for dropping notes
   /*!
    \brief \todo

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void dragMoveEvent( QDragMoveEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void dragLeaveEvent( QDragLeaveEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void dropEvent( QDropEvent *event );

private:
   /*!
    \brief \todo

    \param
   */
   Q_DISABLE_COPY( ChannelWidget )

   /*!
    \brief \todo

    \param from
    \param to
   */
   void moveBar( int from, int to );
   /*!
    \brief \todo

    \param pos
    \return BarWidget
   */
   BarWidget *barWidgetAt( const QPoint &pos );

   SlocumSong              *mpSlocumSong; /*!< \brief song data to edit */
   QBoxLayout              *mpLayout; /*!< \brief main layout */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLineEdit               *mpValueName; /*!< \brief value for name */
};

#endif // CHANNELWIDGET_HPP
