/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_QML.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.            vmid : TEIES string of user id to check

:OUTPUT.           rc   : Length of pending IUCV message from VMID
                          -1 if no path exists.

:INPUT_GLOBALS.    rqhead: IUCV receive queue  head pointer

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
 *            return length of the message if exists.
 *  algorithm:
 *            CALL SCV_QPTH(VMID) TO GET PATHID
 *            if pathid is -1 then no path exists
 *            else using receive queue findout whether message is
 *                 pending, if so return the length of the msg.

:CALLS.       scv_qpth()

:ERRORS.

:LIMITS.

:MODS.

JF            05/23/87       Brought up to specification based on
                             new design. Documentation
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 700

sint scv_qml  ( vmid, len,  rqprev)

   string      *vmid;                 /* from whom message is expected */
   sint              *len;            /* length of the message waiting */
   struct scv_recq        **rqprev;   /* ret previous ptr to the info  */

{

   struct    scv_recq *rqtemp  = NULL;
   struct    scv_conq *conprev = NULL;
   sint      pathid            = 0,      /* path id */
             rc                = 0;

#D_start(scv_qml,OSK_SCV,'Vmid Ptr = 0x%x, LenPtr = 0x%x, Rqptr = 0x%x',\
                         'vmid, len, rqprev')
#D_str('*vmid','Looking for msgs from Vmid...')

   if( *vmid == S_NULL )
   {
      *rqprev = SCVExtrn->rqhead;
      rqtemp = SCVExtrn->rqhead->next;   /* point temp to head element */

      for(; rqtemp ;)
      {                                  /* If we are not end of RECQ  */
         #D_show('Rqtemp = 0x%x, flags = 0x%x','rqtemp,rqtemp->flags')
         if( (rqtemp->flags & SCV_MSG_SEEN) == 0 )
            break;
         *rqprev = rqtemp;               /* and there is MSG_SEEN bit  */
         rqtemp  = rqtemp->next;         /* on, then keep looping      */
      }
      if( rqtemp != NULL )               /* If we are not at the end of*/
      {                                  /* RECQ then we must have     */
                                         /* found a new msg. Lets get  */
                                         /* its vmid and set flag that */
                                         /* application knows of its   */
                                         /* existence.                 */
         if((rc = scv_qid(rqtemp->vmpath, vmid, &conprev)) == SCV_NO_ERR)
         {
            rqtemp->flags |= SCV_MSG_SEEN;
            if( conprev->next->flags & SCV_CON_DISCONN )
            {
               conprev->next->flags |= SCV_DSC_SEEN;/* Aplication knows*/
               rc = ERR_NEW_MSG_BUT_DISC; /* path is in dis-connected  */
            }
         }
         else
         {
            rc = ERR_MSG_BUT_NO_CONN;
         }
      }
      else
      {
         *rqprev = NULL;
         rc = ERR_NO_MSG;
      }
   }
   else
   {
      if( (rc = scv_qpth( *vmid, &conprev )) == ERR_ALREADY_CONN )
      {                                        /* if path exists */
         pathid = conprev->next->pathid;
         *rqprev = SCVExtrn->rqhead;
         rqtemp = SCVExtrn->rqhead->next;

         #D_show('Looking for pathid = 0x%x','pathid')

         for(;  rqtemp ;)
         {
            #D_show('Found pid = 0x%x','rqtemp->vmpath')

            if( rqtemp->vmpath == pathid )
               break;                  /* get out, we found a msg   */
            *rqprev = rqtemp;
            rqtemp  = rqtemp->next;
         }

         if( rqtemp == NULL )
         {
            *rqprev = NULL;
            rc = ERR_NO_MSG; /* no message is waiting */
         }
         else
         {
            *len = rqtemp->msglen;
            rc = SCV_NO_ERR;
         }
      }
   }

   #D_return( rc )

}
