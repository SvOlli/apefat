/**
 * SlocumBar.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumBar.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


SlocumBar::SlocumBar( const QString &setname )
: name( setname )
, isLow( false )
, beat()
{
}


SlocumBar::SlocumBar( const QVariantMap &variantMap )
: name()
, isLow()
, beat()
{
   fromVariantMap( variantMap );
}


SlocumBar::~SlocumBar()
{
}


SlocumBar &SlocumBar::operator=( const SlocumBar &that )
{
   for( int i = 0; i < numEntries; ++i )
   {
      beat[i] = that.beat[i];
   }
   isLow = that.isLow;
   name = that.name;

   return *this;
}


bool SlocumBar::operator==( const SlocumBar &that ) const
{
   for( int i = 0; i < numEntries; ++i )
   {
      if( beat[i] != that.beat[i] )
      {
         return false;
      }
   }
   return (isLow == that.isLow) /*&& (name == that.name)*/;
   // not a full compare to optimize removal of redundance
}


bool SlocumBar::operator!=( const SlocumBar &that ) const
{
   return !(*this == that);
}


bool SlocumBar::isEmpty()
{
   bool empty = true;
   for( int i = 0; i < numEntries; ++i )
   {
      empty &= beat[i].isEmpty();
   }
   return empty;
}


QVariantMap SlocumBar::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList beats;

   for( int i = 0; i < numEntries; ++i )
   {
      beats.append( beat[i].toVariantMap() );
   }
   variantMap.insert( "name",  name );
   variantMap.insert( "islow", isLow );
   variantMap.insert( "beats", beats );

   return variantMap;
}


void SlocumBar::fromVariantMap( const QVariantMap &variantMap )
{
   Q_ASSERT( variantMap.contains( "name" ) );
   name = variantMap.value( "name" ).toString();

   Q_ASSERT( variantMap.contains("islow") );
   isLow = variantMap.value( "islow" ).toBool();

   Q_ASSERT( variantMap.contains("beats") );
   QVariantList beats( variantMap.value("beats").toList() );
   Q_ASSERT( beats.size() == numEntries );
   for( int i = 0; i < numEntries; ++i )
   {
      beat[i].fromVariantMap( beats.at(i).toMap() );
   }
}


int SlocumBar::size()
{
   return numEntries;
}

