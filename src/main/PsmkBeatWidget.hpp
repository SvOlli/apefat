/**
 * PsmkBeatWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKBEATWIDGET_HPP
#define PSMKBEATWIDGET_HPP PSMKBEATWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */
#include "PsmkConfig.hpp"

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPoint;
class QSignalMapper;
class QTimer;

/* forward declaration of local classes */
class PsmkNoteWidget;
class QxtLabel;

/*!
 \brief widget for editing one beat

 a beat is a sequence of 8 \ref PsmkNoteWidget

*/
class PsmkBeatWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param index
    \param parent
   */
   explicit PsmkBeatWidget( int index, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PsmkBeatWidget();
   /*!
    \brief return the defined mime type

    replacement for a #define MIMETYPE_PSMKBEATWIDGET

    \return const char
   */
   static const char *mimeType();
   /*!
    \brief index for drag'n'drop purposes

    \return int
   */
   int index() const;
   /*!
    \brief \todo

    \return QVariantList
   */
   QVariantMap toVariantMap() const;
   /*!
    \brief \todo

    \param variantMap
   */
   bool fromVariantMap( const QVariantMap &variantMap );
   /*!
    \brief \todo

    format: 8 byte of note data vvvppppp (v=voice,p=pitch), 1 byte of accent
            (bit 2^note# is set)

    \return QByteArray
   */
   QByteArray toBinary() const;
   /*!
    \brief set if widget image for drag'n'drop should be created

    \param on
   */
   static void setCreateDragImage( bool on );
   /*!
    \brief \todo

    \return QString
   */
   QString name();

public slots:
   /*!
    \brief \todo

    \param index
    \param tone
   */
   void setInstrument( int index, quint8 tone );
   /*!
    \brief \todo

    \param tones[]
   */
   void setInstruments( quint8 tones[8] );
   /*!
    \brief set translatable texts

   */
   void setTexts();
   /*!
    \brief

   */
   void debug();

signals:
   void debugMsg( const QString &msg );
   /*!
    \brief \todo

   */
   void changed();

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
#if 0
   /*!
    \brief handler reimplemented for painting black border

    \param event
   */
   void paintEvent( QPaintEvent *event );
#endif
   /*!
    \brief handler reimplemented for dragging beats

    \param event
   */
   void mousePressEvent( QMouseEvent *event );
   /*!
    \brief handle the context menu

    \param event
   */
   void contextMenuEvent( QContextMenuEvent *event );

private:
   Q_DISABLE_COPY( PsmkBeatWidget )

   static const char       cMimeType[]; /*!< \brief mime type for dragging */
   static bool             cDragImage; /*!< \brief setting for generating image for drag */

   void setup();
   /*!
    \brief used internally for moving a note inside a beat

    \param from
    \param to
   */
   void moveNote( int from, int to );
   /*!
    \brief used internally for finding a \ref NoteWidget by its position inside \ref PsmkBeatWidget

    \param pos
    \return NoteWidget
   */
   PsmkNoteWidget *noteWidgetAt( const QPoint &pos );

   const int               mIndex; /*!< \brief index of beat (0-3) */
   QxtLabel                *mpTypeName; /*!< \brief \todo TODO */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLineEdit               *mpValueName; /*!< \brief value for name */
   PsmkNoteWidget          *mpNotes[PsmkConfig::NotesInBeat]; /*!< \brief list of note widget used for editing */
   QVariantMap             mSaveBeat; /*!< \brief \todo TODO */
};

#endif // PSMKBEATWIDGET_HPP
