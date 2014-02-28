/**
 * PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PLAYERWIDGET_HPP
#define PLAYERWIDGET_HPP PLAYERWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QPushButton;

/* forward declaration of local classes */
class BarWidget;
class SlocumBar;
class SlocumSong;
class PlayerEmulation;

/*!
 \brief representing a tab for a bar

 This widget is used to show a tab for a bar with all the buttons for moving,
 etc.

*/
class PlayerWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param parent
   */
   explicit PlayerWidget( QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PlayerWidget();

public slots:
   /*!
    \brief set translatable texts

   */
   void setTexts();
   /*!
    \brief re-load configuration data from song

    \param slocumSong
   */
   void setFromSong( SlocumSong *slocumSong );

   void startStop( bool play );

protected:

signals:

private:
   Q_DISABLE_COPY( PlayerWidget )

   QPushButton          *mpPlayButton; /*!< \brief button to start playing current bar */
   PlayerEmulation      *mpPlayerEmulation; /*!< \brief player */
};

#endif // PLAYERWIDGET_HPP
