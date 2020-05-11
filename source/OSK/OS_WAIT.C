/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_WAIT.C

:LEVEL.                OSK

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
 *  purpose: to wait the user machine for the requested period.
 *  inputs:  wait period in seconds
 *  outputs:  waits for the given period.
 *  algorithm:
 *            calls oswait assemble routine with secs as input
 *  rc      : from oswait assemble
 *          0: ok,  -1: error occurred.

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"



sint os_wait ( period )

sint           period;                  /* wait period */


{



    if (period >= 0)  /* valid only for positive wait periods */
    {

   return(Inter( OSWAIT, period ));

    }
    else
    {

   return(-1);

    }
}
