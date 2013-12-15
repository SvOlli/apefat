/**
 * SlocumBarList.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumBarList.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


SlocumBarList::SlocumBarList()
: QList<SlocumBar>()
{
}


SlocumBarList::SlocumBarList( const QVariantList &variantList )
: QList<SlocumBar>()
{
   fromVariantList( variantList );
}


SlocumBarList::~SlocumBarList()
{
}


SlocumBar *SlocumBarList::bar( quint8 bar )
{
   return &(this->operator[]( bar ));
}


QVariantList SlocumBarList::toVariantList() const
{
   QVariantList variantList;

   foreach( const SlocumBar &bar, *this )
   {
      variantList.append( bar.toVariantMap() );
   }

   return variantList;
}


void SlocumBarList::fromVariantList( const QVariantList &variantList )
{
   clear();
   foreach( const QVariant &variant, variantList )
   {
      append( variant.toMap() );
   }
}

