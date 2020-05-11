/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_START.C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  June 14, 1987

:INPUT.                 maxconn  : max number of connection allowed by
                                   this system.

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 scv_on()

:ERRORS.                ISCF_ERR + SUB_ERR
                        04 : System with same name has called ISCF
                        13 : Max # of connection exceeded
                        15 : No authorization found
                        36 : Error in reading system directory

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf_i.h"
#include "isf.h"

#define SUB_ERR 100

sint isf_star(maxcon)
      sint *maxcon;

{

      sint rc = 0;


      #D_begin( isf_star, ISCF)

      rc = scv_on(maxcon);

      #D_return( rc )

}
