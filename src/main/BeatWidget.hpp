/**
 * BeatWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BEATWIDGET_HPP
#define BEATWIDGET_HPP BEATWIDGET_HPP

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
class QPoint;
class QSignalMapper;
class QTimer;

/* forward declaration of local classes */
class NoteWidget;
class SlocumBeat;
class SlocumSong;

/*!
 \brief widget for editing one \ref SlocumBeat

*/
class BeatWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param slocumBeat
    \param index
    \param parent
   */
   explicit BeatWidget( const SlocumSong *slocumSong, SlocumBeat *slocumBeat, int index, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~BeatWidget();
   /*!
    \brief return the defined mime type

    replacement for a #define MIMETYPE_BEATWIDGET

    \return const char
   */
   static const char *mimeType();
   /*!
    \brief index for drag'n'drop purposes

    \return int
   */
   int index() const;
   /*!
    \brief set the values of the beat

    \param beat
   */
   void setValues( const SlocumBeat &beat );
   /*!
    \brief set if widget image for drag'n'drop should be created

    \param on
   */
   static void setCreateDragImage( bool on );

public slots:
   /*!
    \brief set translatable texts

   */
   void setTexts();
   /*!
    \brief set the informal name for this beat

    \param setname
   */
   void setName( const QString &setname );
   /*!
    \brief set the data from the song

    \param slocumSong pointer to global song data used for reading configuration
    \param slocumBeat pointer to beat to edit
   */
   void setFromSong( const SlocumSong *slocumSong, SlocumBeat *slocumBeat );

protected:
   /*!
    \brief handler reimplemented for dropping notes

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief handler reimplemented for dropping notes

    \param event
   */
   void dragMoveEvent( QDragMoveEvent *event );
   /*!
    \brief handler reimplemented for dropping notes

    \param event
   */
   void dragLeaveEvent( QDragLeaveEvent *event );
   /*!
    \brief handler reimplemented for dropping notes

    \param event
   */
   void dropEvent( QDropEvent *event );
   /*!
    \brief handler reimplemented for painting black border

    \param event
   */
   void paintEvent( QPaintEvent *event );
   /*!
    \brief handler reimplemented for dragging beats

    \param event
   */
   void mousePressEvent( QMouseEvent *event );

private:
   Q_DISABLE_COPY( BeatWidget )

   static const char       cMimeType[]; /*!< \brief mime type for dragging */
   static bool             cDragImage; /*!< \brief setting for generating image for drag */

   /*!
    \brief used internally for moving a note inside a beat

    \param from
    \param to
   */
   void moveNote( int from, int to );
   /*!
    \brief used internally for finding a \ref NoteWidget by its position inside \ref BeatWidget

    \param pos
    \return NoteWidget
   */
   NoteWidget *noteWidgetAt( const QPoint &pos );

   const SlocumSong        *mpSlocumSong; /*!< \brief song data to read config from */
   SlocumBeat              *mpSlocumBeat; /*!< \brief pointer to beat to edit */
   SlocumBeat              *mpSaveBeat; /*!< \brief beat saved for undoing drag'n'drop */
   const int               mIndex; /*!< \brief index of beat (0-3) */
   QBoxLayout              *mpLayout; /*!< \brief main layout */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLineEdit               *mpValueName; /*!< \brief value for name */
   QList<NoteWidget*>      mNotes; /*!< \brief list of note widget used for editing */
};

#endif // BEATWIDGET_HPP
