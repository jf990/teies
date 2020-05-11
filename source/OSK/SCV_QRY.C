/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_QRY.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 Oct 26, 1987

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

sint scv_qry  ( vmid, flags, intdata, strdata)

string           vmid;                        /* vmid to inquire about */
sint                  flags;                  /* things to inquire     */
sint                        *intdata;         /* data to send back     */
string                               *strdata;/* str data to send back */


{
   sint   rc = SCV_NO_ERR;
   struct scv_conq *conprev = NULL;
   struct scv_recq *rqprev = NULL;


  if (SCVExtrn != NULL)
  {
   switch( flags )
   {
      case NEW_SOMETHING :
               rc = scv_qdnw( strdata, intdata, &conprev );
               if( rc == SCV_NO_ERR )
               {
                  *intdata = NEW_DISCONNECTION;
               }
               else                           /* nope, chk for new msg */
               {

                  rc = scv_qcnw( strdata, intdata, &conprev );
                  if(( rc == SCV_NO_ERR) || (rc ==ERR_NEW_CONN_BUT_DISC))
                  {

                     *intdata = NEW_CONNECTION;  /* type new_conn  */

                  }
                  rc = scv_qml( strdata, intdata, &rqprev);
                  if( (rc == SCV_NO_ERR) || (rc == ERR_NEW_MSG_BUT_DISC))
                  {

                     *intdata = NEW_MESSAGE;  /* new message           */

                  }
                  else              /* nope, chk for new disconnection */
                  {

                  *strdata = S_NULL;

                  }

               }
               break;

      case MESSAGE_LEN :                     /* len of 1st waiting msg */
               rc = scv_qml(  &vmid, intdata, &rqprev );
               break;

      case PATH_ID :
               rc = scv_qpth (  vmid, &conprev);
               if( rc == SCV_NO_ERR )
                  *intdata = conprev->next->pathid;
               break;

      case NEW_CONNECTION :                  /* return new connection */
               rc = scv_qcnw( strdata, intdata, &conprev );
               break;

      case NEW_DISCONNECTION :               /* any new dis-connection */
               rc = scv_qdnw( strdata, intdata, &conprev );
               break;                        /* return vmid & pathid  */

      default : rc = ERR_FUNC_NOT_DEF;
                break;

   }                                      /* switch           */

   if( rc != SCV_NO_ERR )
      rc = SCV_ERR + SUB_ERR + rc;

  }
  else
  {
      rc = SCV_ERR + SUB_ERR + ERR_NEVER_INIT;
  }


   return(rc);
}
