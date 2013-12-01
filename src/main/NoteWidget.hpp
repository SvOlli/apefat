/**
 * NoteWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef NOTEWIDGET_HPP
#define NOTEWIDGET_HPP NOTEWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;

/* forward declaration of local classes */
class PitchComboBox;
class SlocumSong;
class SlocumNote;
class SoundComboBox;

/*!
 \brief widget for manipulating the values of a \ref SlocumNote

*/
class NoteWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong pointer to global song data
    \param slocumNote pointer to this particular note
    \param index index of the note inside the beat (used for drag'n'drop)
    \param parent
   */
   explicit NoteWidget( const SlocumSong *slocumSong, SlocumNote *slocumNote, int index, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~NoteWidget();
   /*!
    \brief return the defined mime type

    replacement for a #define MIMETYPE_NOTEWIDGET

    \return const char
   */
   static const char *mimeType();
   /*!
    \brief index for drag'n'drop purposes

    \return int
   */
   int index() const;
   /*!
    \brief set the values of the note

    \param note
   */
   void setValues( const SlocumNote &note );
   /*!
    \brief set if widget image for drag'n'drop should be created

    \param on
   */
   static void setCreateDragImage( bool on );

public slots:
   /*!
    \brief set the value of the sound part of the note

    \param sound
   */
   void setSoundValue( quint8 sound );
   /*!
    \brief set the value of the pitch part of the note

    \param pitch
   */
   void setPitchValue( quint8 pitch );
   /*!
    \brief set the value of the accent part of the note

    \param accent
   */
   void setAccent( bool accent );
   /*!
    \brief set the \ref SlocumSong and \ref SlocumNote pointers to new values

    \param slocumSong
    \param slocumNote
   */
   void setFromSong( const SlocumSong *slocumSong, SlocumNote *slocumNote );

protected:
   /*!
    \brief handle a mouse press event

    Used to create an instance for drag'n'drop.

    \param event
   */
   void mousePressEvent( QMouseEvent *event );

private:
   Q_DISABLE_COPY( NoteWidget )

   static const char       cMimeType[]; /*!< \brief mime type */
   static bool             cDragImage; /*!< \brief set by \ref setCreateDragImage */

   const SlocumSong        *mpSlocumSong; /*!< \brief pointer to whole song data */
   SlocumNote              *mpSlocumNote; /*!< \brief pointer of note to edit */
   const int               mIndex; /*!< \brief index of note (0-7) */
   QBoxLayout              *mpLayout; /*!< \brief layout */
   SoundComboBox           *mpSound; /*!< \brief value for sound */
   PitchComboBox           *mpPitch; /*!< \brief value for pitch */
   QCheckBox               *mpAccent; /*!< \brief value for accent */
};

#endif // NOTEWIDGET_HPP
