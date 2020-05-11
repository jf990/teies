/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_CLS.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    sysid    :  System name, determine if scv_on was calle

:OUTPUT_GLOBALS.   contail  :  IUCV connection list tail pointer

:PURPOSE.          Find the IUCV path to the requested server and
                   close the connection.

:CALLS.            OSIUCV   :  via Inter() C to asm interface

:ERRORS.           SCV_ERR + SUB_ERR +
                   00    :  General error
                   03    :  No connection exists
                   55    :  scv_on() was never called


:LIMITS.

:MODS.

JF       05/23/87            Programming and documentation standards,
                             P*3, SCV specification level updates. Added
                             constant for function request. UPdated OSIUCV.
         02/14/89            s_just() ---> s_pad()

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 400

sint   scv_cls(  invmid  )
   string        invmid;
{


struct scv_conq *connext, *contemp, *conprev;
sint rc = SCV_NO_ERR;
string tmpvmid = S_NULL;
boolean tmpflag;


   if ( SCVExtrn != NULL )
   {
      rc = s_pad(&tmpvmid, invmid, IDLEN, ' ', 'R');

      rc = scv_qpth(tmpvmid, &conprev);
      if( (rc == ERR_ALREADY_CONN) || (rc == ERR_CONN_BUT_SEVER) )
      {
         contemp = conprev->next;
         connext = contemp->next;
         tmpflag = contemp->flags & SCV_CON_CALLER;

         rc = Inter(SCVCLS, contemp);

         if( rc == SCV_NO_ERR )
         {
            if( contemp == SCVExtrn->contail )   /* chk if ptr rlsed   */
            {                                    /* is not last element*/
               SCVExtrn->contail = conprev;
               SCVExtrn->contail->next = NULL;
            }
            else
            {
               conprev->next = connext;
            }

            if( tmpflag )                   /* If we establised connect*/
            {                               /* then we have to de-alloc*/
               s_drop( &(contemp->vmid) );  /* otherwise we freed mem  */
               os_memd( contemp );          /* already in assembly     */
            }
         }
         else
         {
            rc = SCV_ERR + SUB_ERR - rc;
         }
      }
      else
      {
         rc = SCV_ERR + SUB_ERR + rc;       /* format rc from scv_qpth */
      }
      s_drop(&tmpvmid);
   }
   else
   {
      rc = SCV_ERR + SUB_ERR + ERR_NEVER_INIT;
   }

   return(rc);
}
