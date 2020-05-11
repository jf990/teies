/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_QID.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 Nov 29, 1987

:INPUT.            pid  :    pathid to look for
                   vmid :    teies str; id associated with pid

:OUTPUT.           rc   :    zero if vmid found for pid
                             else error

:INPUT_GLOBALS.    SCVExtrn : to search for pid in its table

:OUTPUT_GLOBALS.   - none -

:PURPOSE.          Returns vmid associated with given pid.

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

sint scv_qid ( pid, vmid, conprev )

sint             pid;               /* pid to search for               */
string               *vmid;         /* vmid to be returned             */
struct scv_conq            **conprev; /* return prev ptr to info elemen*/

{
    struct scv_conq *contemp = NULL;   /* temporary ptr to the con. q */
    sint  rc = SCV_NO_ERR;

   #D_start( scv_qid,  OSK_SCV,'Looking for Pid 0x%x, VmidPtr = 0x%x',\
                               'pid, vmid')

   *conprev = SCVExtrn->conhead;
   contemp = SCVExtrn->conhead->next;

   for(; contemp ;)
   {
      #D_show('And found pid = 0x%x','contemp->pathid')

      if( contemp->pathid == pid )
         break;
      *conprev = contemp;
      contemp = contemp->next;
   }

   #D_show('contemp:%x', contemp)
   if ( contemp == NULL )        /* reached end of null, didn't find   */
   {                             /* what we were looking for in conque */
      rc = ERR_NO_PATH;
   }
   else
   {
      *vmid = contemp->vmid;
      #D_str('*vmid','after copy')
   }
#D_return( rc );
}
