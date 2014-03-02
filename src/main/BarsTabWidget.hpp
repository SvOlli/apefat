/**
 * BarsTabWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BARSTABWIDGET_HPP
#define BARSTABWIDGET_HPP BARSTABWIDGET_HPP

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

/*!
 \brief representing a tab for a bar

 This widget is used to show a tab for a bar with all the buttons for moving,
 etc.

*/
class BarsTabWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param parent
   */
   explicit BarsTabWidget( SlocumSong *slocumSong, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~BarsTabWidget();
   /*!
    \brief \todo

    \return QPushButton
   */
   QPushButton *playButton() const;
   /*!
    \brief \todo

    \return QPushButton
   */
   QPushButton *loopButton() const;

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
    \brief move to a specific bar for both voices

    \param bar
   */
   void setBar( int bar );
   /*!
    \brief \todo

    \param enabled
   */
   void handleLink( bool enabled );
   /*!
    \brief \todo

   */
   void handlePlay( bool enabled );

protected:

signals:
   /*!
    \brief \todo

    \param bar
   */
   void startPlay( int bar );

private:
   Q_DISABLE_COPY( BarsTabWidget )

   SlocumSong           *mpSlocumSong; /*!< \brief song data to edit */
   int                  mCurrentBar; /*!< \brief the current bar */
   BarWidget            *mpVoice0; /*!< \brief voice 0 */
   BarWidget            *mpVoice1; /*!< \brief voice 1 */
   QLabel               *mpLeadInLabel; /*!< \brief label for left text */
   QLabel               *mpLeadOutLabel; /*!< \brief label for right text */
   QPushButton          *mpLinkButton; /*!< \brief button for going to first bar */
   QPushButton          *mpPlayButton; /*!< \brief button to start playing current bar */
   QPushButton          *mpLoopButton; /*!< \brief button to start playing current bar */
};

#endif // BARSTABWIDGET_HPP
