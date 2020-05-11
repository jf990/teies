/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 IUCVOFF.C

:LEVEL.                MAIN

:AUTHOR.               Sal Johar

:DATE.                 Jan 13, 1988

:INPUT.            - none -

:OUTPUT.           rc   :   0 if IUCV shutdown, else error code

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.          Shutdown the IUCV, clear the interrupts
                   after abrupt crash of teies

:CALLS.            SCVCLR  :  via Inter()

:ERRORS.           SCV_ERR + SUB_ERR +
                   00 : General Error
                   02 : problem freeing up space


:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk_i.h"
#include "osk.h"
#include "debug.h"
#include "debugi.h"
#include "teies.h"

#define SPARE_ARG 0

#define SUB_ERR 200
#define ERR_FREE 2
#define ERR_ALREADY_INIT 66


main()


{

  sint rc = 0;
   string sysid = S_NULL;

#D_main(main, ISCF))


         s_alloc( &sysid, IDLEN);
         os_uid( &sysid);
         s_just( &sysid, sysid, IDLEN, ' ', 'R');

         rc = Inter( SCVOFF, sysid, 0);  /* 2nd arg is the tweek flag */
         if ( rc != 0)
         {
            rc = SCV_ERR + SUB_ERR - rc;
         }

#D_exit(rc);
}
