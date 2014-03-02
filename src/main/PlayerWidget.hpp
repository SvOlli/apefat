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
class QAbstractButton;
class QLabel;
class QPushButton;
class QSpinBox;
class QThread;

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
   /*!
    \brief \todo

    \param button
   */
   void registerPlayButton( QAbstractButton *button );
   /*!
    \brief \todo

    \param button
   */
   void registerLoopButton( QAbstractButton *button );

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
   /*!
    \brief \todo

    \param play
   */
   void startStop( bool play );
   /*!
    \brief \todo

    \param play
   */
   void setLoop( bool enabled );
   /*!
    \brief set translatable texts

    \param play
   */
   void state( const QString &msg );

protected:

signals:
   /*!
    \brief \todo

    \param enabled
   */
   void setPlayState( bool enabled );
   /*!
    \brief \todo

    \param enabled
   */
   void setLoopState( bool enabled );
   /*!
    \brief \todo

    \param bar
   */
   void currentBar( int bar );
   /*!
    \brief \todo

    \param bar
   */
   void setCurrentBar( int bar );

private:
   Q_DISABLE_COPY( PlayerWidget )

   PlayerEmulation      *mpPlayerEmulation; /*!< \brief player */
   QPushButton          *mpPlayButton; /*!< \brief button to start playing current bar */
   QPushButton          *mpLoopButton; /*!< \brief \todo TODO */
   QSpinBox             *mpCurrentBar; /*!< \brief \todo TODO */
   QLabel               *mpState; /*!< \brief \todo TODO */
   //QThread              *mpPlayThread; /*!< \brief \todo TODO */
};

#endif // PLAYERWIDGET_HPP
