/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_CLOSE.C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 pid:  Connection id # received from isf_open()

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               Close connection with another system.

:CALLS.                 scv_cls()

:ERRORS.                55408 : isf_start() was never called
                        55412 : isf_open() was never called for this
                                connection

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"

isf_clos( id, level)

      string id;
      sint level;
{
      sint rc = 0;

      #D_begin( isf_clos, ISCF)

      rc = scv_cls( id );

      #D_return( rc )

}
