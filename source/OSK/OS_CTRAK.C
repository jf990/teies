/*
 *   Copyright (C) 1985, 1988 by the University of Waterloo,
 *   Computer Systems Group. All rights reserved. No part
 *   of this software may be reproduced in any form or by
 *   any means - graphic, electronic or mechanical,
 *   including photocopying, recording, taping or
 *   information storage and retrieval systems - except
 *   with the written permission of the copyright owner.
 */
/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    os_ctrak()                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 30, 1988                                      |
  |                                                                   |
  |  :LEVEL.   OSK                                                    |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.                                                          |
  |                                                                   |
  |  :PURPOSE. Display the C stack by calling modified versions of    |
  |            Waterloo C tracebk(), ctrcbk(), and addrtos().         |
  |                                                                   |
  |  :OUTPUT.  void                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |  :ERRORS.  None known.                                            |
  |  :LIMITS.  None known.                                            |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/
#include <clibrwd.h>
#include <cmslib.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "str.h"
#include "osk.h"


extern  char    * itohs();
extern  int      _handdbg();
extern _rsarea  *_findlim();



void os_ctrak( )

{

   auto _rsarea rs;
   int     auto1[1]; /* changed at recommend of cmjc@watcsg */


   rs.sp = auto1[1];
   rs.ra = auto1[2];
   rs.br = auto1[3];
   rs.ar = auto1[4];
   _tracebk(&rs);

}


/*+-------------------------------------------------------------------+
  |  :Name.    _tracebk()                                             |
  |  :Purpose. Modified version of Waterloo C routine.                |
  |  :END.                                                            |
  +-------------------------------------------------------------------+*/


_tracebk( rs )
    _rsarea     *rs;
  {
    _rsarea     *arglim;
    int         call_limit, args, *ar;
    char        *msg_ptr;
    char        msg_buffer[81];

    _errdisp( "C stack traceback started:" );
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
        arglim = _findlim( rs->br, rs->ar );
        if( ( arglim == NULL ) ||
            ( call_limit-- <= 0 ) )
            break;
        if( rs->ar != arglim->sp )
          {
            _errdisp( "Bad stack data!" );
            break;
          };
        rs->ra = arglim->ra;
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
        ar = (int *) ( rs->ar );
        args = 0;
        while( ( ar != ((int *) arglim) ) &&
               ( args <= 4 ) )
          {
            *msg_ptr = ' ';
            addrtos( ++msg_ptr, *ar );
            msg_ptr += 8;
            ++ar;
            ++args;
          };
        if( ar != ((int *) arglim) )
            strcpy( msg_ptr, " ..." );
        else
            *msg_ptr = '\0';
        _errdisp( msg_buffer );
        memcpy( rs, arglim, sizeof( _rsarea ) );
      }
    while( rs->br != 0 );
    if( call_limit == 0 )
        _errdisp( "The maximum call depth for a traceback was exceeded");
    else if( arglim == NULL )
        _errdisp( "Bad stack format!" );
    _errdisp( "C stack traceback ended." );
  }



/*+-------------------------------------------------------------------+
  |  :Name.    os_dcall()                                             |
  |  :Purpose. Modified version of Waterloo C routine.                |
  |  :END.                                                            |
  +-------------------------------------------------------------------+*/

/* modified 'tracebk' routine.  Will print bottom 6 caller addresses
   in a buffer.  Done by Doc Kenneth "The power of the Source" Ng.
*/
unsigned char *os_dcbuf;


void os_rdcal( rs )
    _rsarea     *rs;
  {
    _rsarea     *arglim;
    int         call_limit, args, *ar;
    char        *msg_ptr;
    char        msg_buffer[81];
    int         skip_first = 1;

    /* in the event of a botched stack, a maximum call depth is */
    /*  set to an arbitrary value... */
    call_limit = 7;
    msg_ptr = os_dcbuf;
    do {
        if( rs->ar == 0 )
            break;
        arglim = _findlim( rs->br, rs->ar );
        if( ( arglim == NULL ) || ( call_limit-- <= 0 ) )
            break;
        if( rs->ar != arglim->sp ) {
            _errdisp( "Bad stack data!" );
            break;
            };
        rs->ra = arglim->ra;
        addrtos( msg_ptr, rs->br & _IA );
        if (skip_first == 1){
           skip_first = 0;
           }
        else {
           msg_ptr += 8;
           }
        *msg_ptr = ' ';
        memcpy( rs, arglim, sizeof( _rsarea ) );
        }
    while( rs->br != 0 );
  }

/* this is the interface point to the routine that prints out the
   bottom few caller addresses up the call stack, starting with
   the routine *BEFORE* the one that called this routine.
*/

void os_dcall( )

{
   auto _rsarea rs2;
   int     auto1[1];

   rs2.sp = auto1[1];
   rs2.ra = auto1[2];
   rs2.br = auto1[3];
   rs2.ar = auto1[4];
   os_rdcal(&rs2);
}



/*+-------------------------------------------------------------------+
  |  :Name.    ctrcbk()                                               |
  |  :Purpose. Modified version of Waterloo C routine.                |
  |  :END.                                                            |
  +-------------------------------------------------------------------+*/



void ctrcbk( code, regarea, psw, xcep_name )
    int code;
    _savedpsw *psw;
    _savedreg *regarea;
    char *xcep_name;
  {
    char msg_buffer[30];
    auto _rsarea rs;

    _wrterm( xcep_name, strlen( xcep_name ), WRT_EDIT_YES );
    strcpy( msg_buffer, "IAR = " );
    itohs( &msg_buffer[6], psw->psw_word2 & _IA );
    _wrterm( msg_buffer, strlen( msg_buffer ), WRT_EDIT_YES );
    rs.sp = regarea->rs.sp;
    rs.ra = regarea->rs.ra;
    rs.br = regarea->rs.br;
    rs.ar = regarea->rs.ar;
    ( *( _getcrwd()->cw_sysd.vm_rwd.cv_trcbk ) )( &rs );

  }





/*+-------------------------------------------------------------------+
  |  :Name.    ctrcbk()                                               |
  |  :Purpose. Modified version of Waterloo C routine.                |
  |           Format an address as an 8-character, right-justified    |
  |           hexadecimal string (not unlike sprintf( "%8x"...), but  |
  |           sprintf is not used inside the library because of the   |
  |           large amount of baggage (e.g., fp formatting) that must |
  |           be linked into the program by printf, even when the user|
  |           does not explicitly reference it)                       |
  |                                                                   |
  |  :END.                                                            |
  +-------------------------------------------------------------------+*/




static addrtos( str, addr )
    char        *str;
    unsigned int addr;
  {
    char        fmt_buffer[10];

    strcpy( str, "        " );
    itohs( fmt_buffer, addr );
    strcpy( &str[ 8 - strlen( fmt_buffer ) ], fmt_buffer );
  }



/*+-------------------------------------------------------------------+
  |  :Name.    showcall()                                             |
  |  :Purpose. To be removed when all references to it are changed to |
  |            os_ctrak().                                            |
  |  :END.                                                            |
  +-------------------------------------------------------------------+*/

void showcall()
{
   os_ctrak();
}

