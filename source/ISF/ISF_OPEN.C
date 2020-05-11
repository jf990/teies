/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_OPEN.C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 id:   System name to connect to
                        junk: To be used in future

:OUTPUT.                rc:   Less then 55,000 then connection id,
                              else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               To open a connection with 'id'

:CALLS.                 scv_open()

:ERRORS.                55308 : isf_start() was never called
                        55311 : calling system not logged on
                        55312 : calling system has not called isf_start()
                        55313 : Max # of connections exceeded
                        55314 : Calling system busy(max entries exceeded)
                        55315 : No authorization found to establish conn

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"

isf_open( id, level, pid)

      string id;
      sint level;
      sint *pid;
{


      sint pathid = 0;
      sint rc = 0;

      #D_begin( isf_open, ISCF)

      rc = scv_open( id, pid);

      #D_return( rc )

}
