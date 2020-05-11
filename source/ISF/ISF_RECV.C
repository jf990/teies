/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_RECV.C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 pid:  connection number to receive data from
                        level: system level expecting data(TM, OBJ, DMS)
                        buf:   buffer to receive data in

:OUTPUT.                rc:   if less then 55,000 then bytes receives
                              else error.

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               to recieve data from 'id'

:CALLS.                 scv_recv()
                        isf_qry() to find length of waiting msg

:ERRORS.                55600 : General error
                        55601 : isf_open never called for this connection
                        55605 : receive buffer too short to contain msg

:LIMITS.                max 32K bytes allowed to receive, in ONE shot

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"

isf_recv(id, level, buf)

      string id;
      sint level;
      string *buf;

{

      sint rc = 0;

      #D_begin( isf_recv, ISCF)

          rc = scv_recv( id, buf);

      #D_return( rc )

}
