/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  SCV_DUMP.C

:LEVEL.                 OSK_SCV

:AUTHOR.                Sal Johar

:DATE.

:INPUT.

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 -none-

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

sint scv_dump()
{
   struct scv_conq *contemp, *conhead, *contail;
   struct scv_recq *rqtemp, *rqhead, *rqtail;
   struct scv_msgq *msgtemp;

if( SCVExtrn == NULL)
   return(0);

conhead = SCVExtrn->conhead;
contail = SCVExtrn->contail;
rqhead = SCVExtrn->rqhead;
rqtail = SCVExtrn->rqtail;




      for(contemp = conhead; contemp; contemp = contemp->next)
      {

         for(msgtemp = contemp->msghead; msgtemp;msgtemp = msgtemp->next)
         {
         }
      }

      for(rqtemp = rqhead;rqtemp;rqtemp = rqtemp->next)
      {
      }

   return(0);
}
