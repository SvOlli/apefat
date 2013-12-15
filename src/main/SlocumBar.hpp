/**
 * SlocumBar.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMBAR_HPP
#define SLOCUMBAR_HPP SLOCUMBAR_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SlocumBeat.hpp"

/* forward declaration of Qt classes */
#include <QString>
#include <QVariant>

/* forward declaration of local classes */


/*!
 \brief data representation of a beat (8 notes)

 Up to 256 of these are used in \ref SlocumSong for each of the two voices.
 User interaction is done using \ref BarWidget.

*/
class SlocumBar
{
private:
   static const int numEntries = 4; /*!< \brief number of beats in a bar */

public:
   /*!
    \brief constructor

    \param name
   */
   SlocumBar( const QString &name = QString() );
   /*!
    \brief constructor for deserializing on creation

    \param name
   */
   SlocumBar( const QVariantMap &variantMap );
   /*!
    \brief destructor

   */
   virtual ~SlocumBar();

   /*!
    \brief assignment operator

    \param that
    \return SlocumBar
   */
   SlocumBar &operator=( const SlocumBar &that );
   /*!
    \brief check same operator

    Note: does not check for name, as it is informal only, and ease implemetation
    of checking for repetitions.

    \param that
    \return bool result of check
   */
   bool operator==( const SlocumBar &that ) const;
   /*!
    \brief check different operator

    Note: does not check for name, as it is informal only, and ease implemetation
    of checking for repetitions.

    \param that
    \return bool result of check
   */
   bool operator!=( const SlocumBar &that ) const;
   /*!
    \brief check is this bar does not contain any data

    (all beats contain mute notes only)

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
    \brief number of beats in a bar

    \return int
   */
   static int size();

   QString     name; /*!< \brief informal name */
   bool        isLow; /*!< \brief bar is low volume */
   SlocumBeat  beat[numEntries]; /*!< \brief array of beats */
};

#endif
