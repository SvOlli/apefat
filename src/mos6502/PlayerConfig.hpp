/**
 * PlayerConfig.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PLAYERCONFIG_HPP
#define PLAYERCONFIG_HPP PLAYERCONFIG_HPP

#include "cpu6502.hpp"

class TIASound;
class SlocumSong;


class PlayerConfig : public Cpu6502Cfg
{
public:
   PlayerConfig( TIASound *tia );
   virtual ~PlayerConfig();
   void invalid( BYTE opcode );
   void reset();
   BYTE peek( ADDRESS addr );
   void poke( ADDRESS addr, BYTE value );

   bool keepOnRunning();

private:
   bool        mKeepOnRunning;
   BYTE        mMemory[0x2000];
   TIASound    *mpTIA;
};

#endif
