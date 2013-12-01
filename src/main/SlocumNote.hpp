/**
 * SlocumNote.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMNOTE_HPP
#define SLOCUMNOTE_HPP SLOCUMNOTE_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
#include <QVariant>

/* forward declaration of local classes */


/*!
 \brief data representation of smallest peace of musical information: a note

 8 of these are used in a \ref SlocumBeat. User interaction is done using
 \ref NoteWidget.

*/
struct SlocumNote
{
public:
   /*!
    \brief constructor

   */
   SlocumNote();
   /*!
    \brief constructor for deserializing on creation

   */
   SlocumNote( const QVariantMap &variantMap );
   /*!
    \brief destructor

   */
   virtual ~SlocumNote();

   /*!
    \brief assignment operator

    \param that
    \return SlocumNote &operator
   */
   SlocumNote &operator=( const SlocumNote &that );
   /*!
    \brief check if same

    \param that other element to compare to
    \return bool operator
   */
   bool operator==( const SlocumNote &that ) const;
   /*!
    \brief check if diffrent

    \param that other element to compare to
    \return bool comparison result
   */
   bool operator!=( const SlocumNote &that ) const;
   /*!
    \brief check is this note does not contain any data

    (note is mute)

    \return bool
   */
   bool isEmpty();

   /*!
    \brief serialize data

    \return QVariantMap
   */
   QVariantMap toVariantMap() const;
   /*!
    \brief deserialize data

    \param variantMap
   */
   void fromVariantMap( const QVariantMap &variantMap );

   quint8      sound; /*!< \brief indirect value for AUDCx (value of 0-7) */
   quint8      pitch; /*!< \brief value for AUDFx (value of 0-31) */
   bool        accent; /*!< \brief play note louder */
};

#endif
