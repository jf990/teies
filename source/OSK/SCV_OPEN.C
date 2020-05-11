/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_OPEN.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.            vmid    :  id to establish connection with
                   pathid  :  ptr where to store path id for connection

:OUTPUT.           rc      :  0 if connection ok, else error
                   pathid  :  return pathid in this ptr

:INPUT_GLOBALS.    sysid   :  system name, to determine if SCV is active.
                   contail

:OUTPUT_GLOBALS.   contail  :  IUCV connection list tail pointer

:PURPOSE.          Open an IUCV communications path between this
                   server and the server requested.

:CALLS.            SCVOPEN  :  via Inter() C to asm interface

:ERRORS.           SCV_ERR + SUB_ERR +
                   00    :  General error
                   11    :  Other user not on-line
                   12    :  Other user has not intilialized IUCV
                   13    :  Maximum # of connection exceeded
                   14    :  Maximum # of connection exceeded of other sys
                   15    :  No authorization found to conn to other user
                   64    :  Connection already exist with calling sys
                   65    :  IUCV not initialized
                   68    :  Connecting to self

:LIMITS.

:MODS.
         11/30/87    when copying into table, justify with spaces
         02/14/89    s_just->s_pad



 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 300

sint   scv_open(  vmid  , pathid)
string           vmid;
sint                     *pathid;
{

sint pid = 0,
     rc = SCV_NO_ERR,
     tcount = 0;
string tmpvmid = S_NULL;
struct scv_conq *tmpcontail = NULL,
                *junkptr = NULL;           /* scv_qpth() requires it   */


   if( SCVExtrn != NULL )
   {
      s_pad( &tmpvmid, vmid, IDLEN, ' ', 'R');
      if( (rc = scv_qpth(tmpvmid, &junkptr)) == ERR_NO_PATH )
      {
         tmpcontail=(struct scv_conq *) os_mema(sizeof(struct scv_conq));
         tmpcontail->vmid = tmpvmid;
         tmpcontail->msghead = tmpcontail->msgtail = NULL;
         tmpcontail->flags = (SCV_CON_CALLER + SCV_CON_SEEN);
                             /* caller and application knows about it */
         tmpcontail->next = NULL;

         pid = Inter(SCVOPEN, tmpcontail);

         if( pid >= 0 )
         {
            tmpcontail->pathid = pid;
            rc = SCV_NO_ERR;
            for(;((tmpcontail->flags & SCV_CON_ACCEPT) == 0 ) &&   \
                 (tcount < SCV_CON_TIME_OUT); tcount++)
            {
               os_wait(1);
            }

            if( tcount == SCV_CON_TIME_OUT)     /* did we time out?? */
            {
               rc = Inter(SCVCLS, tmpcontail);  /* yup, close conn   */
               rc = SCV_ERR + SUB_ERR + ERR_CON_TIME_OUT;
            }
            else
            {                           /* everything went well      */
               SCVExtrn->contail->next = tmpcontail;
               SCVExtrn->contail = tmpcontail;
            }
         }
         else
         {
            rc = SCV_ERR + SUB_ERR - pid;    /* err while opening conn */
         }

         if( rc != SCV_NO_ERR )
         {                                   /* de-alloc things if err */
            s_drop( &(tmpcontail->vmid) );
            os_memd( tmpcontail );
         }
      }
      else
      {
         rc = SCV_ERR + SUB_ERR + rc;       /* format rc from scv_qpth */
      }
   }
   else
   {
      rc = SCV_ERR + SUB_ERR + ERR_NEVER_INIT;
   }

   return(rc);
}
