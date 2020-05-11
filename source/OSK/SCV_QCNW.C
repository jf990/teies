/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_QCNW.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 Nov 29, 1987

:INPUT.            vmid : TEIES string to return new conn
                          should be NULL.
                   pid  : Pathid assigned to the new connection

:OUTPUT.           rc   : 0 if new connection was established
                          else error code.

:INPUT_GLOBALS.    SCVExtrn : To chk for new conn in list

:OUTPUT_GLOBALS.   - none -

:PURPOSE.          Return any new connection accepted by this system

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

sint scv_qcnw  ( vmid, pid, conprev )

string          *vmid;                        /* vmid to inquire about */
sint                  *pid;                   /* pid on new conn       */
struct scv_conq            **conprev;
{
   sint   rc = SCV_NO_ERR;
   struct scv_conq *contemp = NULL;

   #D_begin( scv_qcnw, OSK_SCV)

   *conprev = SCVExtrn->conhead;
   contemp = SCVExtrn->conhead->next;

   for(; contemp ;)
   {
      #D_show('Found connection pid = 0x%x, flags = 0x%x',\
              'contemp->pathid, contemp->flags')
      #D_str('contemp->vmid','VMID...')

      if( (contemp->flags & SCV_CON_SEEN) == 0 )
         break;
      *conprev = contemp;
      contemp = contemp->next;
   }

   #D_show('Found entry = 0x%x','contemp')

   if( contemp != NULL )                  /* if there was a system     */
   {                                      /* contemp won't be NULL     */
      *vmid = contemp->vmid;              /* make copy of new id       */
      *pid = contemp->pathid;             /* copy pid also             */
      contemp->flags |= SCV_CON_SEEN;     /*or to set bit              */
      if( contemp->flags & SCV_CON_DISCONN )
      {
         contemp->flags |= SCV_DSC_SEEN;  /* Now application knows conn*/
         rc = ERR_NEW_CONN_BUT_DISC;      /* already dis-connected     */
      }
   }
   else
   {
      rc = ERR_NO_NEW_CONN;
   }

#D_return( rc )
}
