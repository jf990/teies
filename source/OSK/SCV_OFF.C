/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_OFF.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.            - none -

:OUTPUT.           rc   :   0 if IUCV shutdown, else error code

:INPUT_GLOBALS.    SCVExtrn

:OUTPUT_GLOBALS.   SCVExtrn

:PURPOSE.          Shutdown the IUCV, clear the interrupts

:CALLS.            SCVCLR  :  via Inter()
                   scv_cls :  To close all out-standing connections

:ERRORS.           SCV_ERR + SUB_ERR +
                   00 : General Error
                   02 : problem freeing up space


:LIMITS.

:MODS.
                   July 07, 1988 : Re-write to clean up things


 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 200

sint     scv_off(  )

{
   struct scv_conq *contemp,
                   *connext;
   sint rc = 0;



   if ( SCVExtrn != NULL)    /* chk if scv_on() was ever called */
   {
      for(contemp = SCVExtrn->conhead->next; contemp;contemp = connext)
      {                      /* disconnect all links to other machines */
         connext = contemp->next;
         if((rc = scv_cls(contemp->vmid)) != SCV_NO_ERR)
         {                    /* could not dis-connect for some reason */
         }
      }

      rc = Inter(SCVOFF);


      s_drop( &(SCVExtrn->conhead->vmid) );
      os_memd(SCVExtrn->conhead);

      os_memd(SCVExtrn->rqhead);
      s_drop( &(SCVExtrn->sysid));         /* de-allocate sysid global */
      os_memd(SCVExtrn);                /* finally, de-allocate extern */
      SCVExtrn = NULL;

      if ( rc < 0 )                       /* if error occured   */
      {
         rc = SCV_ERR + SUB_ERR - rc;     /* rc was negative */
      }
   }
   else
   {
      rc = SCV_ERR + SUB_ERR + ERR_NEVER_INIT;
   }

   return(rc);

}
