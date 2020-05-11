/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_RECV.C

:LEVEL.                OSK _ SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.            vmid  : Server to receive data from
                   buffer: Pointer to input buffer

:OUTPUT.           rc    : 0 if received, else error code.

:INPUT_GLOBALS.    rqhead  :  to get msgid, trgcls of the msg to recv
                   sysid   :  to determine if scv_on was ever called

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
 *
 *  purpose:  to receive message from the required vmid.
 *
 *  algorithm:
 *            CALLS IUCV TO RECEIVE MESSAGE INTO THE GIVE BUFFER.
 *
:CALLS.

:ERRORS.      SCV_ERR + SUB_ERR +
             01  : Invalid Path-id
             05  : Receive buffer too short to accept message
             06  : See pg 213 of Sys prog manual SC19-6203
             07  :   "   "
             08  : Invalid class-id or Path-id
             09  : Message was purged
             62  :  No msg to receive
             65  :  Server not initilized
             66  :  No path exists to the requested server.
             67  :  No message waiting to be received

:LIMITS.

:MODS.
             02/14/89     s_just() ----> s_pad()


 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 600

sint scv_recv( vmid, buf )

string         vmid,                      /* msg expecting from vmid */
                    *buf;                 /* buffer ptr              */
{
   struct scv_recq *rqprev = NULL,        /* temp ptr to receive que */
                   *rqnext = NULL,
                   *rqtemp = NULL;
   sint   rc           =   SCV_NO_ERR,
          pid          =  -1,
          msglen       =   0;
   string tmpvmid = S_NULL;


   if ( SCVExtrn != NULL )
   {

      /*************   MAJOR TWEAK   ***********/

             os_wait(1);     /* wait for other user to send it properly*/

      /****************************************/
      s_pad(&tmpvmid, vmid, IDLEN, ' ', 'R');

      if( (rc = scv_qml( &tmpvmid, &msglen, &rqprev )) == SCV_NO_ERR)
      {
         s_alloc( buf, msglen+1 );     /* allocate atleast one byte */
         rqtemp = rqprev->next;
         rqnext = rqtemp->next;

         rc = Inter(SCVRECV, rqtemp, *buf);

         if( rc == SCV_NO_ERR)
         {
            s_smp_ul( *buf ) = (hword ) msglen;
                                       /* rqtemp was freed in assem  */
            if( rqtemp == SCVExtrn->rqtail )  /* is it tail for RECQ */
            {                                 /* yup,                */
               SCVExtrn->rqtail = rqprev;     /* make previous tail  */
               SCVExtrn->rqtail->next = NULL; /* and mark it as end  */
            }                                 /* of the list         */
            else
            {
               rqprev->next = rqnext;         /* fix up chain        */
            }
         }
         else
         {
            rc = SCV_ERR + SUB_ERR - rc;
         }
      }
      else
      {
         rc = SCV_ERR + SUB_ERR + rc;        /* format rc from scv_qpth*/
      }
      s_drop( &tmpvmid );
   }
   else
   {
      rc = SCV_ERR + SUB_ERR + ERR_NEVER_INIT;
   }

   return(rc);
}
