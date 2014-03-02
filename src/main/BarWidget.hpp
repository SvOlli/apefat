/**
 * BarWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BARWIDGET_HPP
#define BARWIDGET_HPP BARWIDGET_HPP

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
class QPushButton;

/* forward declaration of local classes */
class BeatWidget;
class SlocumBar;
class SlocumSong;

/*!
 \brief widget for editing one \ref SlocumBar

*/
class BarWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param slocumBar
    \param parent
   */
   explicit BarWidget( SlocumSong *slocumSong, int voice, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~BarWidget();
   /*!
    \brief \todo

    \return int
   */
   int bar() const;

public slots:
   /*!
    \brief set translatable texts

   */
   void setTexts();
   /*!
    \brief set if this is a low volume bar

    \param isLow
   */
   void setLowVolume( bool isLow );
   /*!
    \brief set the informal name

    \param setname
   */
   void setName( const QString &setname );
   /*!
    \brief set the data from the song

    \param slocumSong pointer to global song data used for reading configuration
    \param slocumBar pointer to bar to edit
   */
   void setFromSong( SlocumSong *slocumSong, quint8 bar );
   /*!
    \brief set the values of the bar

    \param bar
   */
   void setValues( const SlocumBar &bar );
   /*!
    \brief move to a specific bar

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

signals:
   /*!
    \brief the current bar has changed

    \param bar
   */
   void barChanged( int bar );

protected:
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dragMoveEvent( QDragMoveEvent *event );
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dragLeaveEvent( QDragLeaveEvent *event );
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dropEvent( QDropEvent *event );

private:
   Q_DISABLE_COPY( BarWidget )

   /*!
    \brief used internally for moving a beat inside a bar

    \param from
    \param to
   */
   void moveBeat( int from, int to );
   /*!
    \brief used internally for finding a \ref BeatWidget by its position inside \ref BarWidget

    \param pos
    \return BeatWidget
   */
   BeatWidget *beatWidgetAt( const QPoint &pos );

   SlocumSong              *mpSlocumSong; /*!< \brief pointer to song for reading configuration */
   SlocumBar               *mpSlocumBar; /*!< \brief pointer of bar to edit */
   SlocumBar               *mpSaveBar; /*!< \brief bar saved for undoing drag'n'drop */
   QBoxLayout              *mpLayout; /*!< \brief main layout */
   QLabel                  *mpPositionText; /*!< \brief info about current voice and bar */
   QPushButton             *mpFirstButton; /*!< \brief button for going to first bar */
   QPushButton             *mpPreviousButton; /*!< \brief button for going to last bar */
   QPushButton             *mpNextButton; /*!< \brief button for going to next bar */
   QPushButton             *mpLastButton; /*!< \brief button for going to last bar */
   QPushButton             *mpAddBeforeButton; /*!< \brief button for adding before current bar */
   QPushButton             *mpAddAfterButton; /*!< \brief button for adding after current bar */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLineEdit               *mpValueName; /*!< \brief value for name */
   QCheckBox               *mpLowVolume; /*!< \brief value for low volume bar */
   const quint8            mVoice; /*!< \brief current voice */
   quint8                  mBar;  /*!< \brief current bar */
   QList<BeatWidget*>      mBeats; /*!< \brief list of beats in this bar */
};

#endif // BARWIDGET_HPP
