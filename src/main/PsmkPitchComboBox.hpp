/**
 * PsmkPitchComboBox.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPITCHCOMBOBOX_HPP
#define PSMKPITCHCOMBOBOX_HPP PSMKPITCHCOMBOBOX_HPP

/* base class */
#include <QComboBox>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
 \brief specialized drop down box for selecting AUDFx

 Typically used together with \ref SoundComboBox to describe a note.
 Used by \ref NoteWidget, \ref SoundWidget, \ref HihHatWidget to allow
 selection of a pitch (AUDFx) according to the selected tone generator
 (AUDCx), defining a note.

 Also displays the note (like "C-2") and the offset (like "11%") correctly
 according to the corresponding SoundComboBox.
*/
class PsmkPitchComboBox : public QComboBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   explicit PsmkPitchComboBox( QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PsmkPitchComboBox();
   /*!
    \brief value of pitch

    \return AUDFx value or 255 if disabled
   */
   quint8 value();

public slots:
   /*!
    \brief set AUDCx value and adjust content of box entries

    \param parent
   */
   void setSound( quint8 sound );
   /*!
    \brief set AUDFx value, index of combo box

    \param parent
   */
   void setValue( quint8 value );
   /*!
    \brief for internal use: convert a selection change to the new value

   */
   void indexToValue();

signals:
   /*!
    \brief notify of the new value upon a change

    \param value
   */
   void valueChanged( quint8 value );

private:
   Q_DISABLE_COPY( PsmkPitchComboBox )

   static int     noteThreshold; /*!< \brief threshold in percent for bold text display */
   static bool    usePal; /*!< \brief adjust offsets according to PAL instead of NTSC */
};

#endif // PSMKPITCHCOMBOBOX_HPP
