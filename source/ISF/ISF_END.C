/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 - none -

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               Shutdown communication ablity of TEIES system

:CALLS.                 scv_off()

:ERRORS.                55208 : isf_start was never called

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"

sint isf_end()

{

      sint rc = 0;

      #D_begin( isf_end, ISCF)

      rc = scv_off();

      #D_return( rc )

}
