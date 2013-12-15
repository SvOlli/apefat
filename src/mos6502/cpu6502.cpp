/*
 * Originally from x64
 * Modified by Alex Hornby for use in x2600
 * Modified by Daniel Boris for use in Virtual VCS
 * See COPYING for license terms.
 *
 * Modified by Sven Oliver ('SvOlli') Moll for use as standalone 6502 simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "cpu6502.hpp"

#if 0
int main()
{
   BYTE ram[0x2000];
   cpu6502_t cpu;
   cpu.handle = &ram[0];
   cpu.peek = peek2600;
   cpu.poke = poke2600;
   poke2600( &cpu, 0xFFFC, 0x00 );
   poke2600( &cpu, 0xFFFD, 0x10 );
   poke2600( &cpu, 0x1000, 0xC8 );
   poke2600( &cpu, 0x1001, 0x4C );
   poke2600( &cpu, 0x1002, 0x00 );
   poke2600( &cpu, 0x1003, 0x10 );
   cpu6502_reset( &cpu );
   {
      int i;
      for( i = 0; i < 135*1024*1024; i++ )
      {
         //         cpu6502_dump( &cpu );
         cpu6502_step( &cpu ); /* execute 1 command (2-8 clocks) */
      }
      printf( "%d cycles\n", cpu.clk );
   }

   return 0;
}
#endif


Cpu6502::Cpu6502( Cpu6502Cfg *cfg )
: mpCfg( cfg )
{
}


Cpu6502::~Cpu6502()
{
}


void Cpu6502::dump()
{
   printf( "pc:$%04x a:$%02x x:$%02x y:$%02x sp:$%02x st:%c%c-%c%c%c%c%c %ld\n",
           this->pc, this->a, this->x, this->y, this->stack,
           this->sign_flag ? 'N' : '-',
           this->overflow_flag ? 'V' : '-',
           this->break_flag ? 'B' : '-',
           this->decimal_flag ? 'D': '-',
           this->interrupt_flag ? 'I' : '-',
           this->zero_flag ? 'Z' : '-',
           this->carry_flag ? 'C' : '-',
           this->clk );
}

void Cpu6502::nmi()
{
   PUSH(UPPER(PC));
   PUSH(LOWER(PC));                 /* Push return address into the stack. */
   PUSH(GET_SR());                  /* Push status register into the stack. */
   SET_INTERRUPT(1);                /* Disable interrupts. */
   PC = LOAD_ADDR((ADDRESS)0xFFFA); /* Jump using NMI vector (0xfffa). */
   this->clkcount += 7;
   this->clk += this->clkcount;
}

void Cpu6502::irq()
{
   if (!IF)
   {
      PUSH(UPPER(PC));
      PUSH(LOWER(PC));                 /* Push return address into the stack. */
      PUSH(GET_SR());                  /* Push status register into the stack. */
      SET_INTERRUPT(1);                /* Disable interrupts. */
      PC = LOAD_ADDR((ADDRESS)0xFFFE); /* Jump using BRK vector (0xfffe). */
      this->clkcount += 7;
      this->clk += this->clkcount;
   }
}

void Cpu6502::reset()
{
   SET_SR(0x20);
   SP = 0xFF;
   PC = LOAD_ADDR((ADDRESS)0xFFFC);
   AC = 0;
   XR = 0;
   YR = 0;
   this->clk = 0;
   this->clkcount = 0;

   mpCfg->reset();
}


void Cpu6502::invalid()
{
   mpCfg->invalid();
}


void Cpu6502::step()
{
   register BYTE b;

   b = LOADEXEC(PC);

   switch (b)
   {
   case 0x00:
   {
      /* BRK */
      PC+=2;

      SET_BREAK(1);           /* Set break status. */
      PUSH(UPPER(PC));
      PUSH(LOWER(PC));        /* Push return address into the stack. */
      PUSH(GET_SR());                /* Push status register into the stack. */
      SET_INTERRUPT(1);       /* Disable interrupts. */
      PC = LOAD_ADDR((ADDRESS)0xFFFE); /* Jump using BRK vector (0xfffe). */
      this->clkcount = 7;
      break;
   }

   case 0x01:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src=LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 6;
      break;
   }

   case 0x02:
   {
      invalid();
      break;
   }

   case 0x03:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA */
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 8;
      break;
   }

   case 0x04:
   {
      PC += 2;
      this->clkcount = 3;
      break;
   }

   case 0x05:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 3;
      break;
   }

   case 0x06:
   {
      register BYTE p1 = LOAD(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      SET_CARRY(src & 0x80);
      src <<= 1;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x07:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA */
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x08:
   {
      register ADDRESS src = GET_SR();
      PC++;

      PUSH(src | 0x20);
      /* First push item onto stack and then decrement SP. */
      this->clkcount = 3;
      break;
   }

   case 0x09:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x0a:
   {
      register ADDRESS src = AC;
      PC++;

      SET_CARRY(src & 0x80);
      src <<= 1;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x0b:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & p1);
      PC += 2;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x0c:
   {
      PC += 3;
      this->clkcount = 4;
      break;
   }

   case 0x0d:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 4;
      break;
   }

   case 0x0e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      SET_CARRY(src & 0x80);
      src <<= 1;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x0f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA */
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x10:
      /*  BPL,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;

      PC += 2;
      /* Branch if sign flag is clear. */
      if (!IF_SIGN())    {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0x11:
      /* ORA,    INDIRECT_Y */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2 = LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      PC += 2;

      /* If low byte of address  + index reg is > 0xff then extra cycle */
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;
      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);;
      break;
   }

   case 0x12:
   {
      invalid();
      break;
   }

   case 0x13:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1) + YR);
      PC += 2;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA*/
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(LOAD_ZERO_ADDR(p1) + YR, (src));
      this->clkcount = 8;
      break;
   }

   case 0x14:
   {
      PC += 2;

      this->clkcount = 4;
      break;
   }

   case 0x15:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 4;
      break;
   }

   case 0x16:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      SET_CARRY(src & 0x80);
      src <<= 1;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x17:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA */
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x18:
   {
      PC++;

      SET_CARRY((0));
      this->clkcount = 2;
      break;
   }

   case 0x19:
      /*  ORA,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;

      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);;
      break;
   }

   case 0x1a:
   {
      PC++;

      this->clkcount = 2;
      break;
   }

   case 0x1b:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA */
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + YR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x1c:
   {
      PC += 3;

      this->clkcount = 4;
      break;
   }

   case 0x1d:
      /*   ORA,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      src |= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);;
      break;
   }

   case 0x1e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      SET_CARRY(src & 0x80);
      src <<= 1;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x1f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      /* src = asl(src); AC = ora(src); */

      SET_CARRY(src & 0x80);    /* ASL+ORA */
      src <<= 1;
      src |= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x20:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = p2;
      PC += 3;
      /* Jump to subroutine. */
      PC--;
      PUSH((PC >> 8) & 0xff);    /* Push return address onto the stack. */
      PUSH(PC & 0xff);

      PC = (src);
      this->clkcount = 6;
      break;
   }

   case 0x21:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 6;
      break;
   }

   case 0x22:
   {
      invalid();
      break;
   }

   case 0x23:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 8;
      break;
   }

   case 0x24:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      SET_SIGN(src);
      SET_OVERFLOW(0x40 & src);    /* Copy bit 6 to OVERFLOW flag. */
      SET_ZERO(src & AC);
      this->clkcount = 3;
      break;
   }

   case 0x25:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 3;
      break;
   }

   case 0x26:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src <<= 1;
      if (IF_CARRY())
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x27:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x28:
   {
      register unsigned src;
      PC++;

      /* First increment stack pointer and then pull item from stack. */
      src = PULL();

      SET_SR((src));
      this->clkcount = 4;
      break;
   }

   case 0x29:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x2a:
   {
      register ADDRESS src = AC;
      PC++;

      src <<= 1;
      if (IF_CARRY())
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x2b:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & p1);
      PC += 2;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x2c:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      SET_SIGN(src);
      SET_OVERFLOW(0x40 & src);    /* Copy bit 6 to OVERFLOW flag. */
      SET_ZERO(src & AC);
      this->clkcount = 4;
      break;
   }

   case 0x2d:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 4;
      break;
   }

   case 0x2e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src <<= 1;
      if (IF_CARRY())
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x2f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x30:
      /*   BMI,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if sign flag is set. */
      if (IF_SIGN())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0x31:
      /*  AND,    INDIRECT_Y*/
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2 = LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      PC += 2;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);;
      break;
   }

   case 0x32:
   {
      invalid();
      break;
   }

   case 0x33:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1) + YR);
      PC += 2;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE(LOAD_ZERO_ADDR(p1) + YR, (src));
      this->clkcount = 8;
      break;
   }

   case 0x34:
   {
      PC += 2;

      this->clkcount = 4;
      break;
   }

   case 0x35:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 4;
      break;
   }

   case 0x36:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src <<= 1;
      if (IF_CARRY())
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x37:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x38:
   {
      PC++;

      SET_CARRY((1));
      this->clkcount = 2;
      break;
   }

   case 0x39:
      /*  AND,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;
      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      break;
   }

   case 0x3a:
   {
      PC++;

      this->clkcount = 2;
      break;
   }

   case 0x3b:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE(p2 + YR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x3c:
   {
      PC += 3;

      this->clkcount = 4;
      break;
   }

   case 0x3d:
      /*   AND,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;
      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;
      src &= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);;
      break;
   }

   case 0x3e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      src <<= 1;
      if (IF_CARRY())
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x3f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      /* src = rol(src); AC = and(src); */

      src <<= 1;
      if (IF_CARRY())    /* ROL+AND */
         src |= 0x1;
      SET_CARRY(src > 0xff);
      src &= 0xff;

      AC &= src;
      SET_SIGN(AC);
      SET_ZERO(AC);
      /* rotated, not disturbed */
      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x40:
   {
      register unsigned src;
      PC++;
      /* Return from interrupt. */
      /* Load program status and program counter from stack. */
      src = PULL();
      SET_SR(src);
      src = PULL();
      src |= (PULL() << 8);    /* Load return address from stack. */

      PC = (src);
      this->clkcount = 6;
      break;
   }

   case 0x41:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);
      this->clkcount = 6;
      break;
   }

   case 0x42:
   {
      invalid();
      break;
   }

   case 0x43:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 8;
      break;
   }

   case 0x44:
   {
      PC += 2;

      this->clkcount = 3;
      break;
   }

   case 0x45:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);
      this->clkcount = 3;
      break;
   }

   case 0x46:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x47:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x48:
      /*   PHA,    IMPLIED */
   {
      register ADDRESS src = AC;
      PC++;

      PUSH(src);
      /* First push item onto stack and then decrement SP. */
      this->clkcount = 3;
      break;
   }

   case 0x49:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);
      this->clkcount = 2;
      break;
   }

   case 0x4a:
   {
      register ADDRESS src = AC;
      PC++;

      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x4b:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & p1);
      PC += 2;

      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x4c:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = p2;
      PC += 3;

      PC = (src);
      this->clkcount = 3;
      break;
   }

   case 0x4d:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);
      this->clkcount = 4;
      break;
   }

   case 0x4e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x4f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x50:
      /*   BVC,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if overflow flag is clear. */
      if (!IF_OVERFLOW())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0x51:
      /* EOR,    INDIRECT_Y */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2 = LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      PC += 2;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);;
      break;
   }

   case 0x52:
   {
      invalid();
      break;
   }

   case 0x53:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1) + YR);
      PC += 2;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(LOAD_ZERO_ADDR(p1) + YR, (src));
      this->clkcount = 8;
      break;
   }

   case 0x54:
   {
      PC += 2;

      this->clkcount = 4;
      break;
   }

   case 0x55:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);
      this->clkcount = 4;
      break;
   }

   case 0x56:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x57:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x58:
   {
      PC++;

      SET_INTERRUPT((0));
      this->clkcount = 2;
      break;
   }

   case 0x59:
      /*  EOR,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);
      break;
   }

   case 0x5a:
   {
      PC++;

      this->clkcount = 2;
      break;
   }

   case 0x5b:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + YR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x5c:
   {
      PC += 3;

      this->clkcount = 4;
      break;
   }

   case 0x5d:
      /*  EOR,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;
      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      src ^= AC;
      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src & 0xff);;
      break;
   }

   case 0x5e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x5f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      /* src = lsr(src); AC = eor(src); */

      SET_CARRY(src & 0x01);    /* LSR+EOR */
      src >>= 1;
      src ^= AC;
      src &= 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x60:
   {
      register unsigned src;
      PC++;
      /* Return from subroutine. */
      src = PULL();
      src += ((PULL()) << 8) + 1;    /* Load return address from stack and add 1. */

      PC = (src);
      this->clkcount = 6;
      break;
   }

   case 0x61:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 2;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      this->clkcount = 6;
      break;
   }

   case 0x62:
   {
      invalid();
      break;
   }

   case 0x63:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      register unsigned int temp;    PC += 2;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE(LOAD_ZERO_ADDR(p1 + XR), ((BYTE) temp));
      this->clkcount = 8;
      break;
   }

   case 0x64:
   {
      PC += 2;

      this->clkcount = 3;
      break;
   }

   case 0x65:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 2;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      this->clkcount = 3;
      break;
   }

   case 0x66:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      if (IF_CARRY())
         src |= 0x100;
      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0x67:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      register unsigned int temp;    PC += 2;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE_ZERO(p1, ((BYTE) temp));
      this->clkcount = 5;
      break;
   }

   case 0x68:
      /*   PLA,    IMPLIED */
   {
      register unsigned src;
      PC++;

      this->clkcount = 4;
      /* First increment stack pointer and then pull item from stack. */
      src = PULL();
      SET_SIGN(src);    /* Change sign and zero flag accordingly. */
      SET_ZERO(src);

      AC = (src);
      break;
   }

   case 0x69:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 2;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      this->clkcount = 2;
      break;
   }

   case 0x6a:
   {
      register ADDRESS src = AC;
      PC++;

      if (IF_CARRY())
         src |= 0x100;
      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x6b:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & p1);
      PC += 2;

      if (IF_CARRY())
         src |= 0x100;
      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x6c:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD_ADDR(p2);
      PC += 3;

      PC = (src);
      this->clkcount = 5;
      break;
   }

   case 0x6d:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 3;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      this->clkcount = 4;
      break;
   }

   case 0x6e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      if (IF_CARRY())
         src |= 0x100;
      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0x6f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      register unsigned int temp;    PC += 3;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE(p2, ((BYTE) temp));
      this->clkcount = 6;
      break;
   }

   case 0x70:
      /*   BVS,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if overflow flag is set. */
      if (IF_OVERFLOW())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0x71:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2=LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 2;

      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      break;
   }

   case 0x72:
   {
      invalid();
      break;
   }

   case 0x73:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1) + YR);
      register unsigned int temp;    PC += 2;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE(LOAD_ZERO_ADDR(p1) + YR, ((BYTE) temp));
      this->clkcount = 8;
      break;
   }

   case 0x74:
   {
      PC += 2;

      this->clkcount = 4;
      break;
   }

   case 0x75:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 2;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      this->clkcount = 4;
      break;
   }

   case 0x76:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      if (IF_CARRY())
         src |= 0x100;
      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x77:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      register unsigned int temp;    PC += 2;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE_ZERO(p1 + XR, ((BYTE) temp));
      this->clkcount = 6;
      break;
   }

   case 0x78:
   {
      PC++;

      SET_INTERRUPT((1));
      this->clkcount = 2;
      break;
   }

   case 0x79:
      /*   ADC,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 3;

      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      break;
   }

   case 0x7a:
   {
      PC++;

      this->clkcount = 2;
      break;
   }

   case 0x7b:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      register unsigned int temp;    PC += 3;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE(p2 + YR, ((BYTE) temp));
      this->clkcount = 7;
      break;
   }

   case 0x7c:
   {
      PC += 3;

      this->clkcount = 4;
      break;
   }

   case 0x7d:
      /*  ADC,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      register unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);    PC += 3;

      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_ZERO(temp & 0xff);    /* This is not valid in decimal mode */

      /*
     * In decimal mode this is "correct" but not exact emulation. However,
     * the probability of different result when using undefined BCD codes is
     * less than 6%
     * Sign and Overflow are set between BCD fixup of the two digits  -MSM
     */

      if (IF_DECIMAL()) {
         if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9)
            temp += 6;

         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));

         if (temp > 0x99)
            temp += 96;

         SET_CARRY(temp > 0x99);
      } else {
         SET_SIGN(temp);
         SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
         SET_CARRY(temp > 0xff);
      }

      AC = ((BYTE) temp);
      break;
   }

   case 0x7e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      if (IF_CARRY())
         src |= 0x100;
      SET_CARRY(src & 0x01);
      src >>= 1;

      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0x7f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      register unsigned int temp;    PC += 3;

      /* src = ror(src); AC = adc(src);
     * the real operation discovered by msmakela
     * ADC only does the BCD fixup */

      temp = src >>1;    /* ROR+ADC */
      if (IF_CARRY())
         temp |= 0x80;

      SET_SIGN(temp);
      SET_ZERO(temp);            /* Set flags like ROR does */

      SET_OVERFLOW((temp ^ src) & 0x40);    /* ROR causes this ? */

      if (IF_DECIMAL()) {
         if ((src & 0x0f) > 4)        /* half of the normal */
            temp = (temp & 0xf0) | ((temp + 6) & 0xf);
         if (src > 0x4f)
            temp += 96;

         SET_CARRY(src > 0x4f);
      } else {
         SET_CARRY(src & 0x80);        /* 8502 behaviour */
      }

      STORE(p2 + XR, ((BYTE) temp));
      this->clkcount = 7;
      break;
   }

   case 0x80:
   {
      PC += 2;

      this->clkcount = 2;
      break;
   }

   case 0x81:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = AC;
      PC += 2;

      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 6;
      break;
   }

   case 0x82:
   {
      PC += 2;

      this->clkcount = 2;
      break;
   }

   case 0x83:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & XR);
      PC += 2;

      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 6;
      break;
   }

   case 0x84:
      /*  STY,    ZERO_PAGE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = YR;

      this->clkcount = 3;
      PC += 2;

      STORE_ZERO(p1, (src));
      break;
   }

   case 0x85:
      /*   STA,    ZERO_PAGE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = AC;

      this->clkcount = 3;
      PC += 2;

      STORE_ZERO(p1, (src));
      break;
   }

   case 0x86:
      /*  STX,    ZERO_PAGE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = XR;

      this->clkcount = 3;
      PC += 2;

      STORE_ZERO(p1, (src));
      break;
   }

   case 0x87:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & XR);
      PC += 2;

      STORE_ZERO(p1, (src));
      this->clkcount = 3;
      break;
   }

   case 0x88:
   {
      PC++;
      YR--;
      SET_SIGN(YR);
      SET_ZERO(YR);
      this->clkcount = 2;
      break;
   }

   case 0x89:
   {
      PC += 2;

      this->clkcount = 2;
      break;
   }

   case 0x8a:
   {
      register ADDRESS src = XR;
      PC++;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x8b:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = ((AC | 0xee) & XR & p1);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x8c:
      /*  STY,    ABSOLUTE */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = YR;
      PC += 3;

      STORE(p2, (src));
      this->clkcount = 4;
      break;
   }

   case 0x8d:
      /*  STA,    ABSOLUTE */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = AC;
      PC += 3;

      STORE(p2, (src));
      this->clkcount = 4;
      break;
   }

   case 0x8e:
      /*  STA,    ABSOLUTE */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = XR;
      PC += 3;

      STORE(p2, (src));
      this->clkcount = 4;
      break;
   }

   case 0x8f:
      /* STX,    ABSOLUTE */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = (AC & XR);
      PC += 3;

      STORE(p2, (src));
      this->clkcount = 4;
      break;
   }

   case 0x90:
      /*  BCC,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if carry flag is clear. */
      if (!IF_CARRY())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0x91:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = AC;
      PC += 2;

      STORE(LOAD_ZERO_ADDR(p1) + YR, (src));
      this->clkcount = 6;
      break;
   }

   case 0x92:
   {
      invalid();
      break;
   }

   case 0x93:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & XR);
      PC += 2;

      u_stoshr(src, LOAD_ZERO_ADDR(p1), YR);
      this->clkcount = 6;
      break;
   }

   case 0x94:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = YR;
      PC += 2;

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 4;
      break;
   }

   case 0x95:
      /*  STA,    ZERO_PAGE_X */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = AC;
      this->clkcount = 4;
      PC += 2;

      STORE_ZERO(p1 + XR, (src));
      break;
   }

   case 0x96:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = XR;
      PC += 2;

      STORE_ZERO(p1 + YR, (src));
      this->clkcount = 4;
      break;
   }

   case 0x97:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & XR);
      PC += 2;

      STORE_ZERO(p1 + YR, (src));
      this->clkcount = 4;
      break;
   }

   case 0x98:
   {
      register ADDRESS src = YR;
      PC++;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 2;
      break;
   }

   case 0x99:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = AC;
      PC += 3;

      STORE(p2 + YR, (src));
      this->clkcount = 5;
      break;
   }

   case 0x9a:
   {
      register ADDRESS src = XR;
      PC++;

      SP = (src);
      this->clkcount = 2;
      break;
   }

   case 0x9b:
   {
      register ADDRESS src = (AC & XR);
      PC += 3;

      SP = src;    /* SHS */
      this->clkcount = 5;
      break;
   }

   case 0x9c:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = YR;
      PC += 3;

      u_stoshr(src, p2, XR);
      this->clkcount = 5;
      break;
   }

   case 0x9d:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = AC;
      PC += 3;

      STORE(p2 + XR, (src));
      this->clkcount = 5;
      break;
   }

   case 0x9e:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = XR;
      PC += 3;

      u_stoshr(src, p2, YR);
      this->clkcount = 5;
      break;
   }

   case 0x9f:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = (AC & XR);
      PC += 3;

      u_stoshr(src, p2, YR);
      this->clkcount = 5;
      break;
   }

   case 0xa0:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      YR = (src);
      this->clkcount = 2;
      break;
   }

   case 0xa1:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 6;
      break;
   }

   case 0xa2:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      XR = (src);
      this->clkcount = 2;
      break;
   }

   case 0xa3:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      this->clkcount = 6;
      break;
   }

   case 0xa4:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      YR = (src);
      this->clkcount = 3;
      break;
   }

   case 0xa5:
   {
      register BYTE p1 = LOADEXEC(PC+1);

      AC=LOAD_ZERO(p1);
      PC += 2;

      SET_SIGN(AC);
      SET_ZERO(AC);

      this->clkcount = 3;
      break;
   }

   case 0xa6:
      /*    LDX,   ZERO_PAGE*/
   {
      register BYTE p1 = LOADEXEC(PC+1);
      XR=LOAD_ZERO(p1);

      this->clkcount = 3;
      PC += 2;

      SET_SIGN(XR);
      SET_ZERO(XR);
      break;
   }

   case 0xa7:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      this->clkcount = 3;
      break;
   }

   case 0xa8:
   {
      PC++;

      SET_SIGN(AC);
      SET_ZERO(AC);

      YR = AC;
      this->clkcount = 2;
      break;
   }

   case 0xa9:
   {
      AC=LOADEXEC(PC+1);
      PC+=2;
      SET_SIGN(AC);
      SET_ZERO(AC);
      this->clkcount = 2;
      break;
   }

   case 0xaa:
   {
      PC++;

      SET_SIGN(AC);
      SET_ZERO(AC);

      XR = AC;
      this->clkcount = 2;
      break;
   }

   case 0xab:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = (AC & p1);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      this->clkcount = 2;
      break;
   }

   case 0xac:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      SET_SIGN(src);
      SET_ZERO(src);

      YR = (src);
      this->clkcount = 4;
      break;
   }

   case 0xad: /* LDA absolute */
   {
      register ADDRESS p2 = load_abs_addr;
      register unsigned src;
      this->clkcount = 4;
      src=LOAD(p2);
      PC += 3;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      break;
   }

   case 0xae:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      SET_SIGN(src);
      SET_ZERO(src);

      XR = (src);
      this->clkcount = 4;
      break;
   }

   case 0xaf:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      this->clkcount = 4;
      break;
   }

   case 0xb0:
      /*   BCS,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if carry flag is set. */
      if (IF_CARRY())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0xb1:
      /*   LDA,    INDIRECT_Y */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2 = LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      PC += 2;

      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      break;
   }

   case 0xb2:
   {
      invalid();
      break;
   }

   case 0xb3:
      /*   LAX,    INDIRECT_Y */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2=LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      PC += 2;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      break;
   }

   case 0xb4:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      YR = (src);
      this->clkcount = 4;
      break;
   }

   case 0xb5:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      this->clkcount = 4;
      break;
   }

   case 0xb6:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + YR);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      XR = (src);
      this->clkcount = 4;
      break;
   }

   case 0xb7:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + YR);
      PC += 2;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      this->clkcount = 4;
      break;
   }

   case 0xb8:
   {
      PC++;

      SET_OVERFLOW((0));
      this->clkcount = 2;
      break;
   }

   case 0xb9:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      break;
   }

   case 0xba:
   {
      register ADDRESS src = SP;
      PC++;

      SET_SIGN(src);
      SET_ZERO(src);

      XR = (src);
      this->clkcount = 2;
      break;
   }

   case 0xbb:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = (SP & LOAD(p2 + YR));
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = SP = (src);
      break;
   }

   case 0xbc:
      /*   LDY,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;
      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_SIGN(src);
      SET_ZERO(src);

      YR = (src);
      break;
   }

   case 0xbd:
      /*  LDA,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = (src);
      break;
   }

   case 0xbe:
      /*   LDX,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_SIGN(src);
      SET_ZERO(src);

      XR = (src);
      break;
   }

   case 0xbf:
      /*   LAX,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      SET_SIGN(src);
      SET_ZERO(src);

      AC = XR = (src);
      break;
   }

   case 0xc0:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src = YR - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 2;
      break;
   }

   case 0xc1:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 6;
      break;
   }

   case 0xc2:
   {
      PC += 2;

      this->clkcount = 2;
      break;
   }

   case 0xc3:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      PC += 2;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 8;
      break;
   }

   case 0xc4:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src = YR - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 3;
      break;
   }

   case 0xc5:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 3;
      break;
   }

   case 0xc6:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src = (src - 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0xc7:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0xc8:
   {
      PC++;

      YR++;
      SET_SIGN(YR);
      SET_ZERO(YR);
      this->clkcount = 2;
      break;
   }

   case 0xc9:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 2;
      break;
   }

   case 0xca:
   {
      PC++;
      XR--;
      SET_SIGN(XR);
      SET_ZERO(XR);
      this->clkcount = 2;
      break;
   }

   case 0xcb:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src = (AC & XR) - src;    /* Carry is ignored (CMP) */
      /* Overflow flag may be affected */
      SET_CARRY(src < 0x100);

      src &= 0xff;    /* No decimal mode */
      SET_SIGN(src);
      SET_ZERO(src);

      XR = (src);
      this->clkcount = 2;
      break;
   }

   case 0xcc:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src = YR - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 4;
      break;
   }

   case 0xcd:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 4;
      break;
   }

   case 0xce:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src = (src - 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0xcf:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0xd0:
      /*  BNE,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if  zero flag is clear. */
      if (!IF_ZERO())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0xd1:
      /*   CMP,    INDIRECT_Y */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2=LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      PC += 2;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      break;
   }

   case 0xd2:
   {
      invalid();
      break;
   }

   case 0xd3:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1) + YR);
      PC += 2;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE(LOAD_ZERO_ADDR(p1) + YR, (src));
      this->clkcount = 8;
      break;
   }

   case 0xd4:
   {
      PC += 2;

      this->clkcount = 4;
      break;
   }

   case 0xd5:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 4;
      break;
   }

   case 0xd6:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src = (src - 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0xd7:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0xd8:
   {
      PC++;

      SET_DECIMAL((0));
      this->clkcount = 2;
      break;
   }

   case 0xd9:
      /*   CMP,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;
      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      break;
   }

   case 0xda:
   {
      PC++;

      this->clkcount = 2;
      break;
   }

   case 0xdb:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      PC += 3;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE(p2 + YR, (src));
      this->clkcount = 7;
      break;
   }

   case 0xdc:
   {
      PC += 3;

      this->clkcount = 4;
      break;
   }

   case 0xdd:
      /*   CMP,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;
      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      src = AC - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      break;
   }

   case 0xde:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      src = (src - 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0xdf:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      /* cmp(--src & 0xff));  */

      src = (src - 1) & 0xff;    /* DEC+CMP */
      SET_CARRY(AC >= src);
      SET_SIGN(AC - src);
      SET_ZERO(AC != src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0xe0:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      PC += 2;

      src = XR - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 2;
      break;
   }

   case 0xe1:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 2;

      /*
     * SBC is not exact either. It has 6% different results too.
    */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      this->clkcount = 6;
      break;
   }

   case 0xe2:
   {
      PC += 2;

      this->clkcount = 2;
      break;
   }

   case 0xe3:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1 + XR));
      register unsigned int temp;    PC += 2;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE(LOAD_ZERO_ADDR(p1 + XR), (src));
      this->clkcount = 8;
      break;
   }

   case 0xe4:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src = XR - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 3;
      break;
   }

   case 0xe5:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 2;

      /*
     * SBC is not exact either. It has 6% different results too.
     */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      this->clkcount = 3;
      break;
   }

   case 0xe6:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      PC += 2;

      src = (src + 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0xe7:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1);
      register unsigned int temp;    PC += 2;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE_ZERO(p1, (src));
      this->clkcount = 5;
      break;
   }

   case 0xe8:
   {
      PC++;
      XR++;
      SET_SIGN(XR);
      SET_ZERO(XR);
      this->clkcount = 2;
      break;
   }

   case 0xe9:
      /*  SBC,    IMMEDIATE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 2;

      /*
    * SBC is not exact either. It has 6% different results too.
    */

      if (!IF_DECIMAL()) {
         SET_SIGN(temp);
         SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

         SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));
         SET_CARRY(temp < 0x100);
         AC = (temp & 0xff);

      } else {
         int   bcd1, bcd2;
         BYTE old_A;
         int C = IF_CARRY() ? 1 : 0;

         old_A = AC;

         bcd1= fromBCD(AC);
         bcd2 = fromBCD(src);

         bcd1 = bcd1 - bcd2 - !C;

         if (bcd1 < 0) bcd1 = 100 - (-bcd1);
         AC = toBCD(bcd1);

         SET_CARRY( (old_A < (src + !C)) ? 0 : 1);
         SET_OVERFLOW((old_A ^ AC) & 0x80);
      }
      this->clkcount = 2;
      break;
   }

   case 0xea:
   {
      PC++;
      this->clkcount = 2;
      break;
   }

   case 0xeb:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 2;

      /*
    * SBC is not exact either. It has 6% different results too.
     */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      this->clkcount = 2;
      break;
   }

   case 0xec:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src = XR - src;
      SET_CARRY(src < 0x100);
      SET_SIGN(src);
      SET_ZERO(src &= 0xff);
      this->clkcount = 4;
      break;
   }

   case 0xed:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 3;

      /*
    * SBC is not exact either. It has 6% different results too.
    */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      this->clkcount = 4;
      break;
   }

   case 0xee:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      PC += 3;

      src = (src + 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0xef:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2);
      register unsigned int temp;    PC += 3;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE(p2, (src));
      this->clkcount = 6;
      break;
   }

   case 0xf0:
      /*   BEQ,    RELATIVE */
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = p1;
      register BYTE hb;
      PC += 2;
      /* Branch if  zero flag is set. */
      if (IF_ZERO())
      {
         hb = UPPER(PC);
         PC = REL_ADDR(PC, src);
         if( brtest(hb))
            /* Same page */
            this->clkcount = 3;
         else
            /* Different page */
            this->clkcount = 4;
      }
      else
         this->clkcount = 2;
      break;
   }

   case 0xf1:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS p2=LOAD_ZERO_ADDR(p1);
      register ADDRESS src = LOAD(p2 + YR);
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 2;

      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 6;
      else
         /* Same page */
         this->clkcount = 5;

      /*
     * SBC is not exact either. It has 6% different results too.
    */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
   }

      break;

   case 0xf2:
   {
      invalid();
      break;
   }

   case 0xf3:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD(LOAD_ZERO_ADDR(p1) + YR);
      register unsigned int temp;    PC += 2;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE(LOAD_ZERO_ADDR(p1) + YR, (src));
      this->clkcount = 8;
      break;
   }

   case 0xf4:
   {
      PC += 2;

      this->clkcount = 4;
      break;
   }

   case 0xf5:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 2;

      /*
     * SBC is not exact either. It has 6% different results too.
     */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      this->clkcount = 4;
      break;
   }

   case 0xf6:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      PC += 2;

      src = (src + 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0xf7:
   {
      register BYTE p1 = LOADEXEC(PC+1);
      register ADDRESS src = LOAD_ZERO(p1 + XR);
      register unsigned int temp;    PC += 2;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE_ZERO(p1 + XR, (src));
      this->clkcount = 6;
      break;
   }

   case 0xf8:
      /*  SED,    IMPLIED */
   {
      PC++;
      SET_DECIMAL((1));
      this->clkcount = 2;
      break;
   }

   case 0xf9:
      /*  SBC,    ABSOLUTE_Y */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 3;

      if( pagetest(p2,YR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      /*
     * SBC is not exact either. It has 6% different results too.
    */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      break;
   }

   case 0xfa:
   {
      PC++;
      this->clkcount = 2;
      break;
   }

   case 0xfb:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + YR);
      register unsigned int temp;    PC += 3;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE(p2 + YR, (src));
      this->clkcount = 7;
      break;
   }

   case 0xfc:
   {
      PC += 3;
      this->clkcount = 4;
      break;
   }

   case 0xfd:
      /*   SBC,    ABSOLUTE_X */
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      register unsigned int temp = AC - src - (IF_CARRY() ? 0 : 1);    PC += 3;

      if( pagetest(p2,XR))
         /* Over page */
         this->clkcount = 5;
      else
         /* Same page */
         this->clkcount = 4;

      /*
     * SBC is not exact either. It has 6% different results too.
     */

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = (temp & 0xff);
      break;
   }

   case 0xfe:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD(p2 + XR);
      PC += 3;

      src = (src + 1) & 0xff;
      SET_SIGN(src);
      SET_ZERO(src);

      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   case 0xff:
   {
      register ADDRESS p2 = load_abs_addr;
      register ADDRESS src = LOAD( p2 + XR);
      register unsigned int temp;    PC += 3;

      /* src = ++src & 0xff; AC = sbc(src); */

      src = ((src + 1) & 0xff);    /* INC+SBC */

      temp = AC - src - (IF_CARRY() ? 0 : 1);

      SET_SIGN(temp);
      SET_ZERO(temp & 0xff);    /* Sign and Zero are invalid in decimal mode */

      SET_OVERFLOW(((AC ^ temp) & 0x80) && ((AC ^ src) & 0x80));

      if (IF_DECIMAL()) {
         if ( ((AC & 0xf) + (IF_CARRY() ? 1 : 0)) < (src & 0xf))
            temp -= 6;
         if (temp > 0x99)
            temp -= 0x60;
      }
      SET_CARRY(temp < 0x100);

      AC = temp;
      /* src saved */
      STORE(p2 + XR, (src));
      this->clkcount = 7;
      break;
   }

   } /* switch */

   this->clk += this->clkcount;
}


//#ifndef NO_UNDOC_CMDS
void Cpu6502::u_stoshr( unsigned int val, ADDRESS address, BYTE index )
{
   val &= (((address >> 8) + 1) & 0xff);
   if (((address & 0xff) + index) > 0xff)
      address = val;
   STORE((address + index), val);
}
//#endif   /* NO_UNDOC_CMDS */
