/*
 * cpu6502.hpp based on:
 *
 * $Id: cpu.h,v 1.4 1996/02/19 17:36:37 alex Exp alex $
 * $Id: macro.h,v 1.4 1996/02/26 17:41:48 alex Exp alex $
 *
 * This file was part of Commodore 64 emulator.
 * See COPYING for copyright notice
 *
 * Written by
 *   Vesa-Matti Puro (vmp@lut.fi)
 *   Jarkko Sonninen (sonninen@lut.fi)
 *   Jouko Valta (jopi@stekt.oulu.fi)
 *
 * Modifications by
 *   Alex Hornby
 *   Sven Oliver ("SvOlli") Moll (svolli@svolli.de)
 *
 */


#ifndef CPU6502_HPP
#define CPU6502_HPP CPU6502_HPP

typedef signed char     SIGNED_CHAR;
typedef unsigned char 	BYTE;
typedef unsigned short 	ADDRESS;
typedef unsigned long	CLOCK;

class Cpu6502Cfg
{
public:
   virtual ~Cpu6502Cfg() {}
   virtual void invalid() = 0;
   virtual void reset() = 0;
   virtual void crash() = 0;
   virtual BYTE peek( ADDRESS addr ) = 0;
   virtual void poke( ADDRESS addr, BYTE value ) = 0;
};


class Cpu6502
{
public:
   Cpu6502( Cpu6502Cfg *_cfg );
   virtual ~Cpu6502();
   void dump();
   void step();
   void reset();
   void crash();
   void nmi();
   void irq();
   void invalid();
   void u_stoshr( unsigned int val, ADDRESS address, BYTE index );

private:
   Cpu6502Cfg *cfg;
   ADDRESS     pc;
   BYTE        status;
   BYTE        stack;
   BYTE        a;
   BYTE        x;
   BYTE        y;
   BYTE        zero_flag;
   BYTE        sign_flag;
   BYTE        overflow_flag;
   BYTE        break_flag;
   BYTE        decimal_flag;
   BYTE        interrupt_flag;
   BYTE        carry_flag;
   CLOCK       clk;
   CLOCK       clkcount;
};


/* 6510 Registers. */
#define AC        (this->a)
#define XR        (this->x)
#define YR        (this->y)
#define SP        (this->stack)
#define PC        (this->pc)
#define PCH       ((PC>>8)&0xff)
#define PCL       (PC&0xff)

#define ZF        (this->zero_flag)
#define SF        (this->sign_flag)
#define OF        (this->overflow_flag)
#define BF        (this->break_flag)
#define DF        (this->decimal_flag)
#define IF        (this->interrupt_flag)
#define CF        (this->carry_flag)

/* Masks which indicate location of status flags in PSW. */
#define S_SIGN        0x80
#define S_OVERFLOW    0x40
#define S_NOTUSED     0x20
#define S_BREAK       0x10
#define S_DECIMAL     0x08
#define S_INTERRUPT   0x04
#define S_ZERO        0x02
#define S_CARRY       0x01

#define LOAD(a)               (cfg->peek(a))
#define LOADEXEC(a)           LOAD(a)
#define DLOAD(a)              LOAD(a)
#define LOAD_ZERO(a)          LOAD((ADDRESS)a)
#define LOAD_ADDR(a)          ((LOADEXEC(a+1)<<8)+LOADEXEC(a))
#define LOAD_ZERO_ADDR(a)     LOAD_ADDR(a)

#define STORE(a,b)            (cfg->poke((a),(b)))
#define STORE_ZERO(a,b)       STORE(((ADDRESS)a),(b))

#define PUSH(b)               STORE(SP+0x100,(b));SP--
#define PULL()                LOAD((++SP)+0x100)

#define UPPER(ad)             (((ad)>>8)&0xff)
#define LOWER(ad)             ((ad)&0xff)
#define LOHI(lo,hi)           ((lo)|((hi)<<8))

#define REL_ADDR(pc,src)      (pc+((SIGNED_CHAR)src))

#define SET_SIGN(a)           (SF=(a)&S_SIGN)
#define SET_ZERO(a)           (ZF=!(a))
#define SET_CARRY(a)          (CF=(a))

#define SET_INTERRUPT(a)      (IF=(a))
#define SET_DECIMAL(a)        (DF=(a))
#define SET_OVERFLOW(a)       (OF=(a))
#define SET_BREAK(a)          (BF=(a))

#define SET_SR(a) \
   (SF=(a) & S_SIGN,\
   ZF=(a) & S_ZERO,\
   CF=(a) & S_CARRY,\
   IF=(a) & S_INTERRUPT,\
   DF=(a) & S_DECIMAL,\
   OF=(a) & S_OVERFLOW,\
   BF=(a) & S_BREAK)

#define GET_SR() \
   ((SF ? S_SIGN : 0) |\
   (ZF ? S_ZERO : 0) |\
   (CF ? S_CARRY : 0) |\
   (IF ? S_INTERRUPT : 0) |\
   (DF ? S_DECIMAL : 0) |\
   (OF ? S_OVERFLOW : 0) |\
   (BF ? S_BREAK : 0) | S_NOTUSED)

#define IF_SIGN()      SF
#define IF_ZERO()      ZF
#define IF_CARRY()     CF
#define IF_INTERRUPT() IF
#define IF_DECIMAL()   DF
#define IF_OVERFLOW()  OF
#define IF_BREAK()     BF

#define sprint_status()     sprint_binary(GET_SR())

/* moved from exmacro */
/* returns TRUE if an address increment will cross a page boundary */
#define pagetest(a,b)   (LOWER(a) + b > 0xff)
#define load_abs_addr   (LOADEXEC(PC+1)|(LOADEXEC(PC+2)<<8))
#define brtest(a)       (UPPER(PC)==(a))
#define toBCD(a)        ((((a % 100) / 10) << 4) | (a % 10))
#define fromBCD(a)      (((a >> 4) & 0xf) * 10 + (a & 0xf))

#endif  /* CPU6502_HPP */
