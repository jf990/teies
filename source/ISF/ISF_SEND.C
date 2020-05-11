/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_SEND.C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 pid:  Connection id number
                        level: Calling level(TM, DMS, OBJ)
                        buf:  Data to be sent to other system

:OUTPUT.                rc:   If less then 55,000 then number of bytes
                              sent, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               Send 'buf' to other system

:CALLS.                 scv_send()

:ERRORS.                55500 : General Error
                        55501 : isf_open() was never called for this id.
                        55503 : other system busy(too many msgs on que)

:LIMITS.                max 32K bytes allowed to send.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"


isf_send( id, level, buf)

      string id;
      sint level;
      string buf;

{


      sint rc = 0;

      #D_begin( isf_send, ISCF)

      rc = scv_send( id, buf);

      #D_return( rc )

}
