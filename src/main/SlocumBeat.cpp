/**
 * SlocumBeat.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumBeat.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


SlocumBeat::SlocumBeat( const QString &setname )
: name( setname )
, note()
{
}


SlocumBeat::SlocumBeat( const QVariantMap &variantMap )
: name()
, note()
{
   fromVariantMap( variantMap );
}


SlocumBeat::~SlocumBeat()
{
}


SlocumBeat &SlocumBeat::operator=( const SlocumBeat &that )
{
   for( int i = 0; i < numEntries; ++i )
   {
      note[i] = that.note[i];
   }
   name = that.name;

   return *this;
}


bool SlocumBeat::operator==( const SlocumBeat &that ) const
{
   for( int i = 0; i < numEntries; ++i )
   {
      if( note[i] != that.note[i] )
      {
         return false;
      }
   }
   return true; // should be: (name == that.name), but name is only informal
}


bool SlocumBeat::operator!=( const SlocumBeat &that ) const
{
   return !(*this == that);
}


bool SlocumBeat::isEmpty()
{
   bool empty = true;
   for( int i = 0; i < numEntries; ++i )
   {
      empty &= note[i].isEmpty();
   }
   return empty;
}


QVariantMap SlocumBeat::toVariantMap() const
{
  QVariantMap variantMap;
  QVariantList notes;

  for( int i = 0; i < numEntries; ++i )
  {
     notes.append( note[i].toVariantMap() );
  }
  variantMap.insert( "name", name );
  variantMap.insert( "notes", notes );

  return variantMap;
}


void SlocumBeat::fromVariantMap( const QVariantMap &variantMap )
{
   Q_ASSERT( variantMap.contains( "name" ) );
   name = variantMap.value( "name" ).toString();

   Q_ASSERT( variantMap.contains( "notes" ) );
   QVariantList notes( variantMap.value("notes").toList() );
   Q_ASSERT( notes.size() == numEntries );

   for( int i = 0; i < numEntries; ++i )
   {
      note[i].fromVariantMap( notes.at(i).toMap() );
   }
}


int SlocumBeat::size()
{
   return numEntries;
}

