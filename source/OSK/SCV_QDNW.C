/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_QDNW.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 Nov 29, 1987

:INPUT.            vmid : TEIES string to return new disconn
                          should be NULL.
                   pid  : Pathid assigned to the new dis-connection

:OUTPUT.           rc   : 0 if new dis connection
                          else error code.

:INPUT_GLOBALS.    SCVExtrn : To chk for new dis-conn in list

:OUTPUT_GLOBALS.   - none -

:PURPOSE.          Return any new disconnected vmid

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

#define SUB_ERR 700

sint scv_qdnw  ( vmid, pid, conprev )

string          *vmid;                        /* vmid to inquire about */
sint                  *pid;                   /* pid on new conn       */
struct scv_conq            **conprev;
{
   sint   rc = SCV_NO_ERR;
   struct scv_conq *contemp = NULL;

   #D_begin( scv_qdnw, OSK_SCV)

   *conprev = SCVExtrn->conhead;
   contemp = SCVExtrn->conhead->next;
   for(;contemp;*conprev = contemp, contemp = contemp->next)
   {
      #D_show('Found entry pid = 0x%x, flags = 0x%x', \
              'contemp->pathid, contemp->flags')

      if( contemp->flags & SCV_CON_DISCONN ) /* conn is disconnected   */
      {
         if( (contemp->flags & SCV_DSC_SEEN) == 0 )
         {
            break;                        /* get out, found one which  */
         }                                /* is disc & not yet known to*/
      }                                   /* upper level               */
   }

   #D_show('Entry found contemp = 0x%x','contemp')


   if ( contemp != NULL)                  /* if there was a system     */
   {                                      /* contemp won't be NULL     */
      *vmid = contemp->vmid;
      *pid = contemp->pathid;             /* copy pid also             */
      contemp->flags |= SCV_DSC_SEEN;     /*or to set bit              */
      rc = 0;
   }
   else
   {
      rc = ERR_NO_NEW_DSC;
   }

#D_return( rc )
}
