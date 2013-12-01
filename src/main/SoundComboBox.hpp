/**
 * SoundComboBox.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SOUNDCOMBOBOX_HPP
#define SOUNDCOMBOBOX_HPP SOUNDCOMBOBOX_HPP

/* base class */
#include <QComboBox>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class SlocumSong;

/*!
 \brief specialized drop down box for selecting AUDCx

 Typically used together with \ref PitchComboBox to describe a note.
 Can be configured to show either all options for AUDCx, or only the ones
 supported by the current song. Used in \ref NoteWidget, \ref SoundWidget
 \ref HiHatWidget.

*/
class SoundComboBox : public QComboBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    Create a widget for selection of the voice (AUDCx) with all possible voices,
    just excluding the doublettes.

    \param parent
   */
   explicit SoundComboBox( QWidget *parent = 0 );
   /*!
    \brief constructor

    When constructed with a SlocumSong-pointer, the widget uses indirect mode:
    filling the selection box with all possible voices, it just uses the 8 ones
    defined in slocumSong->sound()->type[] .

    \param slocumSong pointer to song data to pull out configuration for indirection
    \param parent
   */
   explicit SoundComboBox( const SlocumSong *slocumSong, QWidget *parent = 0 );
   /*!
    \brief constructor

   */
   virtual ~SoundComboBox();
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
   /*!
    \brief re-read configuration for indirection from song data

    \param slocumSong SlocumSong to set to
   */
   void setFromSong( const SlocumSong *slocumSong );

signals:
   /*!
    \brief notify that the value of the combo box has been changed

    \param value
   */
   void valueChanged( quint8 value );

private:
   Q_DISABLE_COPY( SoundComboBox )

   /*!
    \brief internally used for updating list

   */
   void populate();

   const SlocumSong        *mpSlocumSong; /*!< \brief song data to pull configuration from, if any */
};

#endif // SOUNDCOMBOBOX_HPP
