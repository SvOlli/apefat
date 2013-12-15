/**
 * SlocumBar.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMBARLIST_HPP
#define SLOCUMBARLIST_HPP SLOCUMBARLIST_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SlocumBar.hpp"

/* forward declaration of Qt classes */
#include <QStringList>
#include <QVariant>

/* forward declaration of local classes */


/*!
 \brief data representation a list of up to 256 bars

*/
class SlocumBarList : public QList<SlocumBar>
{
public:
   /*!
    \brief constructor

   */
   SlocumBarList();
   /*!
    \brief constructor for deserializing on creation

    \param name
   */
   SlocumBarList( const QVariantList &variantList );
   /*!
    \brief destructor

   */
   virtual ~SlocumBarList();

   /*!
    \brief \todo

    \param bar
    \return SlocumBar
   */
   SlocumBar *bar( quint8 bar );

   /*!
    \brief serialize data

    \return QVariantList
   */
   QVariantList toVariantList() const;
   /*!
    \brief deserialize data

    \param variantList
   */
   void fromVariantList( const QVariantList &variantList );
};

#endif
