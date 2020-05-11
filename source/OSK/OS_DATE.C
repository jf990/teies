/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_DATE.C

:LEVEL.                OSK

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

           This routine calls OSTODCLK for date.

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



sint os_date(      ptr )


date              *ptr;

{

    sint      rc = 0;




    rc = Inter( OSTODCLK,  ptr,  1  );


   return(rc);

}
