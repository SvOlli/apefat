/**
 * PsmkInstrumentComboBox.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKINSTRUMENTCOMBOBOX_HPP
#define PSMKINSTRUMENTCOMBOBOX_HPP PSMKINSTRUMENTCOMBOBOX_HPP

/* base class */
#include <QtGlobal>
#if QT_VERSION < 0x050000
#include <QComboBox>
#else
#include <QtWidgets/QComboBox>
#endif

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
 \brief specialized drop down box for selecting AUDCx

 Typically used together with \ref PitchComboBox to describe a note.
 Can be configured to show either all options for AUDCx, or only the ones
 supported by the current song. Used in \ref NoteWidget, \ref SoundWidget
 \ref HiHatWidget.

*/
class PsmkInstrumentComboBox : public QComboBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    When constructed with a SlocumSong-pointer, the widget uses indirect mode:
    filling the selection box with all possible voices, it just uses the 8 ones
    defined in slocumSong->sound()->type[] .

    \param index
    \param parent
   */
   explicit PsmkInstrumentComboBox( QWidget *parent = 0 );
   /*!
    \brief constructor

   */
   virtual ~PsmkInstrumentComboBox();
   /*!
    \brief value of selection

    \return AUDCx or 255, if disabled
   */
   quint8 value();
   /*!
    \brief \todo

    \param instruments
   */
   void setInstruments( quint8 *instruments );

public slots:
   /*!
    \brief \todo

    \param index
    \param tone
   */
   void setTone( int index, quint8 tone );
   /*!
    \brief \todo

    \param tones[]
   */
   void setTones( quint8 tones[8] );
   /*!
    \brief internally used: convert index of selection to value

    \param index
   */
   void indexToValue( int index );
   /*!
    \brief set the index according to the passed value

    \param value
   */
   void setValue( quint8 value );
   /*!
    \brief \todo

   */
   void checkIndex( int index );

signals:
   /*!
    \brief notify that the value of the combo box has been changed

    \param value
   */
   void valueChanged( quint8 value );
   /*!
    \brief \todo

    \param type
   */
   void typeChanged( quint8 type );
   /*!
    \brief \todo

    \param mute
   */
   void isMute( bool mute );

private:
   Q_DISABLE_COPY( PsmkInstrumentComboBox )

   /*!
    \brief internally used for updating list

   */
   void populate();

   quint8   mInstruments[8];
};

#endif // PSMKINSTRUMENTCOMBOBOX_HPP
