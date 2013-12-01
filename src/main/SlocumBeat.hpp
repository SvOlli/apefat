/**
 * SlocumBeat.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMBEAT_HPP
#define SLOCUMBEAT_HPP SLOCUMBEAT_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SlocumNote.hpp"

/* forward declaration of Qt classes */
#include <QString>
#include <QVariant>

/* forward declaration of local classes */

/*!
 \brief data representation of a beat (8 notes)

 4 of these are used in a \ref SlocumBar. User interaction is done using
 \ref BeatWidget.

*/
class SlocumBeat
{
private:
   static const int numEntries = 8; /*!< \brief number of notes in a beat */

public:
   /*!
    \brief constructor

    \param setname
   */
   SlocumBeat( const QString &setname = QString() );
   /*!
    \brief constructor for deserializing on creation

    \param setname
   */
   SlocumBeat( const QVariantMap &variantMap );
   /*!
    \brief destructor

   */
   virtual ~SlocumBeat();

   /*!
    \brief assignment operator

    \param that
    \return SlocumBeat &operator
   */
   SlocumBeat &operator=( const SlocumBeat &that );
   /*!
    \brief check equal operator

    Note: does not check for name, as it is informal only, and ease implemetation
    of checking for repetitions.

    \param that
    \return bool result of check
   */
   bool operator==( const SlocumBeat &that ) const;
   /*!
    \brief check difference operator

    Note: does not check for name, as it is informal only, and ease implemetation
    of checking for repetitions.

    \param that
    \return bool result of check
   */
   bool operator!=( const SlocumBeat &that ) const;
   /*!
    \brief check is this beat does not contain any data

    (all notes are mute)

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

   /*!
    \brief get the number of notes in a beat

    \return int
   */
   static int size();

   QString     name; /*!< \brief informal name of this beat */
   SlocumNote  note[numEntries]; /*!< \brief array of notes */
};

#endif
