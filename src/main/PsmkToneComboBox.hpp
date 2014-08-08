/**
 * PsmkToneComboBox.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKTONECOMBOBOX_HPP
#define PSMKTONECOMBOBOX_HPP PSMKTONECOMBOBOX_HPP

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
class PsmkToneComboBox : public QComboBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    Create a widget for selection of the voice (AUDCx) with all possible voices,
    just excluding the doublettes.

    \param parent
   */
   explicit PsmkToneComboBox( QWidget *parent = 0 );
   /*!
    \brief constructor

   */
   virtual ~PsmkToneComboBox();
   /*!
    \brief value of selection

    \return AUDCx or 255, if disabled
   */
   quint8 value();

public slots:
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

signals:
   /*!
    \brief notify that the value of the combo box has been changed

    \param value
   */
   void valueChanged( quint8 value );

private:
   Q_DISABLE_COPY( PsmkToneComboBox )

   /*!
    \brief internally used for updating list

   */
   void populate();
};

#endif // PSMKTONECOMBOBOX_HPP
