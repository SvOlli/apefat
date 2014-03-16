/**
 * PsmkNoteWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKNOTEWIDGET_HPP
#define PSMKNOTEWIDGET_HPP PSMKNOTEWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;
class QLabel;

/* forward declaration of local classes */
class PsmkInstrumentComboBox;
class PsmkPitchComboBox;

/*!
 \brief widget for manipulating the values of a 32th note

 contains three parameters
 - instrument (combination of preset tone and volume)
 - pitch (frequency divider)
 - accent (play note at maximum volume, regardless of settings)

 \todo find a way to disable the selection instrument 7 & pitch 31
*/
class PsmkNoteWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param index index of the note inside the beat (used for drag'n'drop)
    \param parent
   */
   explicit PsmkNoteWidget( int index, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PsmkNoteWidget();
   /*!
    \brief return the defined mime type

    replacement for a #define MIMETYPE_PSMKNOTEWIDGET

    \return const char
   */
   static const char *mimeType();
   /*!
    \brief index for drag'n'drop purposes

    \return int
   */
   int index() const;
   /*!
    \brief set if widget image for drag'n'drop should be created

    \param on
   */
   static void setCreateDragImage( bool on );
   /*!
    \brief \todo

    \return QVariantMap
   */
   QVariantMap toVariantMap() const;
   /*!
    \brief \todo

    \param data
   */
   bool fromVariantMap( const QVariantMap &variantMap );
   /*!
    \brief \todo

    format: 1 byte of note data vvvppppp (v=voice,p=pitch), 1 byte of accent
            (bit 2^note# is set)

    \return QByteArray
   */
   QByteArray toBinary() const;
   /*!
    \brief \todo

    \return bool
   */
   bool accent() const;

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
    \brief \todo

   */
   void setTexts();
   /*!
    \brief \todo

   */
   void checkInvalid();

signals:
   /*!
    \brief \todo

   */
   void changed();
   /*!
    \brief \todo

    \param index
   */
   void requestRemove( int index );
   /*!
    \brief \todo

    \param index
    \param data
    \param insert
   */
   void requestSetData( int index, const QVariant &data, bool insert );

protected:
   /*!
    \brief handle a mouse press event

    Used to create an instance for drag'n'drop.

    \param event
   */
   void mousePressEvent( QMouseEvent *event );
   /*!
    \brief handle the context menu

    \param event
   */
   void contextMenuEvent( QContextMenuEvent *event );

private:
   Q_DISABLE_COPY( PsmkNoteWidget )

   /*!
    \brief internally used by constuctors

   */
   void setup();

   static const char       cMimeType[]; /*!< \brief mime type */
   static bool             cDragImage; /*!< \brief set by \ref setCreateDragImage */

   const int               mIndex; /*!< \brief index of note (0-7) */
   QBoxLayout              *mpLayout; /*!< \brief layout */
   PsmkInstrumentComboBox  *mpInstrument; /*!< \brief value for sound */
   PsmkPitchComboBox       *mpPitch; /*!< \brief value for pitch */
   QCheckBox               *mpAccent; /*!< \brief value for accent */
   QLabel                  *mpTypeLabel; /*!< \brief  */
};

#endif // PSMKNOTEWIDGET_HPP
