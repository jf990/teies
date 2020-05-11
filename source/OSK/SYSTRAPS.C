
/*
 *   Copyright (C) 1986, by the University of Waterloo,
 *   Computer Systems Group. All rights reserved. No part
 *   of this software may be reproduced in any form or by
 *   any means - graphic, electronic or mechanical,
 *   including photocopying, recording, taping or
 *   information storage and retrieval systems - except
 *   with the written permission of the copyright owner.
 */

/*
 * systraps -- CLIB signal handling routines
 */

#define NULL 0

/*
 * The following #if/#endif is a tweeker to make this routine
 * dependant on the OSK header file--this is so it will get properly
 * added to the OSK TXTLIB even if it has been deleted, etc.
 */
#if 0
#include "osk_i.h"
#endif
/*
 * End of abovementioned tweaker.
 */

#include <signal.h>

extern _exit();
extern char *strcpy();
extern int strlen();
extern char *itohs();
extern char *malloc();
extern int ffs();
extern bcopy();

extern _setattnexit(), _clrattnexit();
extern _errdisp();
extern _progmask();
extern _clrexttrap(), _setexttrap();
extern char *_find_lim();       /* takes apart stack */
extern int _handdbx();          /* tries to hand DBX the exception */

extern unsigned progmask();     /* forward reference */
extern int ctrcbk();            /* forward reference */

extern int (*_deftrap)();
extern int (**_trapvec)();

extern unsigned int sysmask;
extern char _inattn;

char *sys_siglist(::)
    = {
        "Unknown exception",
        "Operation exception",
        "Privileged operation exception",
        "Execute exception",
        "Protection exception",
        "Addressing exception",
        "Specification exception",
        "Data exception",
        "Fixed-point overflow",
        "Fixed-point divide exception",
        "Decimal overflow",
        "Decimal divide exception",
        "Exponent overflow",
        "Exponent underflow",
        "Significance check",
        "Floating-point divide exception",
        "Out of memory" ,
        "Out of time",
        "Attn",
        "Protection exception (probable cause: stack overflow)",
        "Disk full error occurred during a write operation",
        "User defined exception #1",
        "User defined exception #2",
        "User defined exception #3",
        "User defined exception #4",
        "External interrupt"
    };


/*
 * _inittrap -- initialize trap handling
 */

_inittrap()
  {
    char *temporary_trapvec(: NSIG+1 :);

    /* use a temporary trap vector for the first malloc, until we get */
    /*  trapping and memory allocation bootstrapped... */
    _trapvec = temporary_trapvec;
    signal( _OUT_OF_MEMORY, SIG_IGN );
    _trapvec = malloc( (NSIG+1) * sizeof( char* ) );
    if( _trapvec != NULL )
      {
        signal( _PROGRAM_CHECK, SIG_DFL );
        signal( _OUT_OF_MEMORY, SIG_IGN );
        signal( _DISK_FULL, SIG_IGN );
        signal( _USER_DEFS, SIG_IGN );
        signal( _XTRN_INT, SIG_DFL );
        _trapvec(: 0 :) = SIG_DFL;
      };
    /* hook in the default trap handler... */
    _deftrap = ctrcbk;
  }


/*
 * _finitrap -- unhook CLIB trap handling from the system
 */

_finitrap()
  {
    /* Note that attention handlers should not call this routine    */
    /*  either directly, or through a call to _exit.  A quick       */
    /*  verification of _inattn is made nevertheless; CMS currently */
    /*  abends with a very nasty message otherwise...               */
    if( _inattn == 0 )
        signal( _ATTN_TRAP, SIG_DFL );
  }


/*
 * signal -- establish the action to be taken when a specified set of
 *           "signals" (also known as "traps" or "exceptions") occur
 *           If an invalid signal code is specified, -1 is returned,
 *           otherwise a pointer to the previous handler is returned
 */

int (*signal( code, address ))()
  register char *address;
  register int code;
  {
    register char *lastaddr;
    register int bit, setmask;
    register char **trapv_ptr;

    if( ( code > _LAST_TRAP ) ||
        ( code <= _NULL_TRAP ) )
        return( -1 );
    trapv_ptr = _trapvec;
    setmask = 0;
    bit = 1;
    while( code != 0 )
      {
        /* the first entry in _trapvec is wasted... */
        ++trapv_ptr;
        if( code & 1 )
          {
            lastaddr = *trapv_ptr;
            *trapv_ptr = address;
            if( bit & _ATTN_TRAP )
              {
                if( address == SIG_DFL )
                    _clrattnexit();
                else
                    _setattnexit();
              }
            if( bit & _XTRN_INT )
              {
                if( address == SIG_DFL )
                    _clrexttrap();
                else
                    _setexttrap();
              }
            if( bit & (_DISABLED + _FLOAT_UNDER) )
              {
                if( ( address == SIG_IGN ) ||
                    ( ( address == SIG_DFL ) &&
                      ( bit & _DISABLED ) ) )
                  {
                    sysmask &= ~progmask( bit );
                    *trapv_ptr = SIG_IGN;
                  }
                else
                    sysmask |= progmask( bit );
                setmask = -1;
              }
          }
        code >>= 1;
        bit <<= 1;
      }
    if( setmask != 0 )
        _progmask( sysmask );
    return( lastaddr );
  }


int (*ssignal( code, address ))()
  register unsigned code;
  register char *address;
  {
    register retval;

    retval = signal( code, address );
    if( retval == -1 )
        retval = 0;
    return( retval );
  }


/*
 * progmask -- return the bit for program mask of the PSW in bits 4-7
 *             corresponding to bits 36-39 in the BC Mode PSW
 */

static unsigned progmask( bit )
  register unsigned bit;
  {
    register retval;

    if( bit & _INTEGER_OVERFLOW )
        retval = 0x08000000;    /* mask for FIXED POINT OVERFLOW */
    else if( bit & _DECIMAL_OVERFLOW )
        retval = 0x04000000;    /* mask for DECIMAL OVERFLOW */
    else if( bit & _FLOAT_UNDER )
        retval = 0x02000000;    /* mask for EXPONENT UNDERFLOW */
    else
        retval = 0x01000000;    /* mask for SIGNIFICANCE except */
    return( retval );
  }


int _systrap( code, regarea, psw )
  register int code;
  register char *psw, *regarea;
  {
    register status;
    register (*catchaddr)();

    catchaddr = _trapvec(: code :);
    status = 0;
    if( catchaddr == SIG_DFL )
      {
        if( code == ffs( _ATTN_TRAP ) )
            return( -1 );
        if( (status = _handdbx( code, regarea, psw )) < 0 )
            status =
                (*_deftrap)( code, regarea, psw, sys_siglist(: code :));
      }
    else if( catchaddr != SIG_IGN )
        status = (*catchaddr)( code, regarea, psw,
            sys_siglist(: code :) );
    return( status );
  }


/*
 * addrtos -- format an address as an 8-character, right-justified
 *            hexadecimal string (not unlike sprintf( "%8x"...), but
 *            sprintf is not used inside the library because of the
 *            large amount of baggage (e.g., fp formatting) that must
 *            be linked into the program by printf, even when the user
 *            does not explicitly reference it)
 */

static addrtos( str, addr )
  register char *str;
  register unsigned addr;
  {
    char fmt_buffer(:10:);

    strcpy( str, "        " );
    itohs( fmt_buffer, addr );
    strcpy( &str(: 8 - strlen( fmt_buffer ) :), fmt_buffer );
  }


_rsarea rs;
showcall ()
{ int auto1 (:0:);
   rs.sp = auto1(:1:);
   rs.ra = auto1(:2:);
   rs.br = auto1(:3:);
   rs.ar = auto1(:4:);
   traceback(&rs);
}

/* Changed deleted 'static' from following declaration line in
   an effort to make a new trackback call 6 October 1987 /ken */

traceback( rs )
  register _rsarea *rs;
  {
    register int args, *ar, *arglim;
    register int call_limit;
    register char *msg_ptr;
    char msg_buffer(:81:);

    _errdisp( "Traceback started" );
    _errdisp( "  BR(14)   RA(13)   AR(15)  PARMS-AUTOS" );
    /* output record format: (the numbers are an offset into msg_buffer)
0        9        18        28       37       46       55       64
|        |        |         |        |        |        |        |
v        v        v         v        v        v        v        v
xxbr(14) xxra(13) xxar(15)  xxxxp-a1 xxxxp-a2 xxxxp-a3 xxxxp-a4 xxxxp-a5
    */
    /* in the event of a botched stack, a maximum call depth is */
    /*  set to an arbitrary value... */
    call_limit = 40;
    do
      {
        if( rs->ar == 0 )
            break;
        arglim = _find_lim( rs->br, rs->ar );
        if( ( arglim == 0 ) ||
            ( call_limit-- <= 0 ) )
            break;
        if( rs->ar != ((_rsarea *)arglim)->sp )
          {
            _errdisp( "bad stack data" );
            break;
          };
        rs->ra = ( (_rsarea *) arglim )->ra;
        msg_ptr = msg_buffer;
        addrtos( msg_ptr, rs->br & _IA );
        msg_ptr += 8;
        *msg_ptr = ' ';
        addrtos( ++msg_ptr, rs->ra & _IA );
        msg_ptr += 8;
        *msg_ptr = ' ';
        addrtos( ++msg_ptr, rs->ar & _IA );
        msg_ptr += 8;
        *msg_ptr = ' ';
        ++msg_ptr;
        ar = rs->ar;
        args = 0;
        while( ( ar != arglim ) &&
               ( args <= 4 ) )
          {
            *msg_ptr = ' ';
            addrtos( ++msg_ptr, *ar );
            msg_ptr += 8;
            ++ar;
            ++args;
          }
        if( ar != arglim )
            strcpy( msg_ptr, " ..." );
        else
            *msg_ptr = '\0';
        _errdisp( msg_buffer );
        /* bcopy must be done or else we write to the stack... */
        bcopy( arglim, rs, sizeof( _rsarea ) );
      }
    while( rs->br != 0 );
    if( call_limit == 0 )
       _errdisp( "The maximum call depth for a traceback was exceeded");
    else if( arglim == 0 )
        _errdisp( "bad stack format" );
    _errdisp( "Traceback ended" );
  }


static int ctrcbk( code, regarea, psw, xcep_name )
  register int code;
  register _savedpsw *psw;
  register _savedreg *regarea;
  register char *xcep_name;
  {
    char msg_buffer(:30:);
    auto _rsarea rs;

    _errdisp( xcep_name );
    strcpy( msg_buffer, "IAR = " );
    itohs( &msg_buffer(:6:), psw->psw_word2 & _IA );
    _errdisp( msg_buffer );
    rs.sp = regarea->rs.sp;
    rs.ra = regarea->rs.ra;
    rs.br = regarea->rs.br;
    rs.ar = regarea->rs.ar;
    traceback( &rs );
    if( code != 0 )
        _exit( code );
    return( code );
  }
