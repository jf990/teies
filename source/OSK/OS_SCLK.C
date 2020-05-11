/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  OS_SCLK.C

:LEVEL.                 OSK

:AUTHOR.                Sal Johar

:DATE.                  June 09, 1987

:INPUT.                 dt

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.               To be used with pTEIES in future

:CALLS.                 -none-

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "osk_i.h"

sint os_sclk(dt)
   datetime *dt;
{

   #D_begin(os_sclk, OSK)

   #D_return( 0 );

}
