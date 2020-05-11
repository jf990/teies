/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV__OFF.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 Nov 11, 1987

:INPUT.            - none -

:OUTPUT.           rc   :   0 if IUCV shutdown, else error code

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   sysid, conhead, contail, acchead, acctail,
                   rqhead, rqtail

:PURPOSE.          Shutdown the IUCV, clear the interrupts

:CALLS.            SCVCLR  :  via Inter()

:ERRORS.           SCV_ERR + SUB_ERR +
                   00 : General Error
                   02 : problem freeing up space


:LIMITS.

:MODS.             Feb 14, 1989  Replaced s_just() with s_pad()



 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 200

sint     scv__off(  )
{
   sint rc = 0;
   string sysid = S_NULL;



      if ( SCVExtrn == NULL)    /* chk if scv_on() was ever called */
      {

         s_alloc( &sysid, IDLEN);
         os_uid( &sysid);
         s_pad( &sysid, sysid, IDLEN, ' ', 'R');

         rc = Inter( SCVOFFF, sysid);

         s_drop( &sysid );
         if ( rc != 0)
         {
            rc = SCV_ERR + SUB_ERR - rc;
         }
      }
      else
      {

         rc = SCV_ERR + SUB_ERR + ERR_ALREADY_INIT + 1; /* fake it!! */

      }

   return(rc);

}
