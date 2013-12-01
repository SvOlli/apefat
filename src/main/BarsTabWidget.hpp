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

public slots:
   /*!
    \brief set translatable texts

   */
   void setTexts();
   /*!
    \brief re-load configuration data from song

    \param slocumSong
   */
   void setFromSong( const SlocumSong *slocumSong );
   /*!
    \brief move to a specific bar for both channels

    \param bar
   */
   void setBar( int bar );
   /*!
    \brief move to first bar

    Handler for \ref mpFirstButton

   */
   void moveFirst();
   /*!
    \brief move to previous bar

    Handler for \ref mpPreviousButton

   */
   void movePrevious();
   /*!
    \brief move to next bar

    Handler for \ref mpNextButton

   */
   void moveNext();
   /*!
    \brief move to last bar

    Handler for \ref mpLastButton

   */
   void moveLast();
   /*!
    \brief insert a bar before current one

    Handler for \ref mpAddBeforeButton

   */
   void insertBefore();
   /*!
    \brief insert a bar after current one

    Handler for \ref mpAddAfterButton

   */
   void insertAfter();

protected:

signals:
   /*!
    \brief signalizes that a bar has been added

   */
   void barAdded();
   /*!
    \brief the current bar has changed

    \param bar
   */
   void barChanged( int bar );

private:
   Q_DISABLE_COPY( BarsTabWidget )

   SlocumSong           *mpSlocumSong; /*!< \brief song data to edit */
   int                  mCurrentBar; /*!< \brief the current bar */
   BarWidget            *mpChannel0; /*!< \brief channel 0 */
   BarWidget            *mpChannel1; /*!< \brief channel 1 */
   QLabel               *mpLeadInLabel; /*!< \brief label for left text */
   QLabel               *mpLeadOutLabel; /*!< \brief label for right text */
   QPushButton          *mpFirstButton; /*!< \brief button for going to first bar */
   QPushButton          *mpPreviousButton; /*!< \brief button for going to last bar */
   QPushButton          *mpNextButton; /*!< \brief button for going to next bar */
   QPushButton          *mpLastButton; /*!< \brief button for going to last bar */
   QPushButton          *mpAddBeforeButton; /*!< \brief button for adding before current bar */
   QPushButton          *mpAddAfterButton; /*!< \brief button for adding after current bar */
};

#endif // BARSTABWIDGET_HPP
