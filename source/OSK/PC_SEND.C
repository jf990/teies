#include "defs.h"
#include "str.h"
#include "debug.h"
#include "pcomm.h"
#include "osk.h"

sint pc_send( filnam, fillen, data, datlen)
   byte *filnam, *data;
   hword fillen, datlen;
{
   byte state = 0,            /* current state  */
        pstate = 0,           /* previous state */
        nstate = 0,           /* next state     */
        pktnum = 0;           /* current pkt # being sent */
   sint rc = 0, rrc = 0,      /* return code & real rc passing back  */
        prterr = 0,           /* counter for protocol errs           */
        oserr = 0,            /* counter for OS errs from os_conn    */
        touterr = 0,          /* counter for time-outs               */
        nakerr = 0;           /* counter for nake errs               */
   hword stmplen = 0,
        rtmplen = 0,
        tmpbeg = 0,
        inlen = 0;
   byte *stmpdat = NULL,
        *rtmpdat = NULL,
        *tmpfilnam = NULL;
   static byte nxtstate(::) = {
        PC_ST_SEND_UPL,      PC_ST_RECV_UPL_ACK,
        PC_ST_PREP_FIL,
        PC_ST_SEND_FIL,      PC_ST_RECV_FIL_ACK,
        PC_ST_SEND_SOT,      PC_ST_RECV_SOT_ACK,
        PC_ST_PKTIZE_DATA,
        PC_ST_SEND_DATA,     PC_ST_RECV_DATA_ACK, PC_ST_PKTIZE_DATA,
        PC_ST_SEND_EOT,      PC_ST_RECV_EOT_ACK,
        PC_ST_EXIT,          PC_ST_NONE,
        PC_ST_SEND_CAN,      PC_ST_EXIT,
        PC_ST_SEND_NAK,      PC_ST_ANY };
                              /* NAK state changes depending on who    */
                              /* asked to send NAK OR who received one */

                        /* Function table is setup so state machine  */
                        /* knows which character to send or receive  */
                        /* PC_NULL is there only to take up room!!   */
   static byte functab(::) = { PC_NULL,
        PC_UPL,  PC_ACK,
        PC_NULL,
        PC_FIL,  PC_ACK,
        PC_SOT,  PC_ACK,
        PC_NULL,
        PC_DATA, PC_ACK, PC_NULL,
        PC_EOT,  PC_ACK,
        PC_NULL, PC_NULL,
        PC_CAN,  PC_NULL,
        PC_NAK,  PC_NULL      };

   #D_start(pc_send, pcomm,'Filptr=%x, Fillen=%d, Datptr=%x, Datlen=%d',\
                           'filnam, fillen, data, datlen')
   #D_dump(filnam, fillen, 'File name being sent')
   #D_dump(data, datlen, 'Data being sent')

   stmplen = rtmplen = PC_MAX_PKTLEN;
   stmpdat = os_mema( stmplen );
   rtmpdat = os_mema( rtmplen );

   for(;;)
   {

      pstate = state;              /* make last state to previous */
      if( state == nstate )        /* if anyone changed the next state */
         state = nstate = nxtstate(:state:); /* next state is default */
      else
         state = nstate;          /* make it to exception state   */

      #D_show('States->Current=%d, Next=%d, Previous=%d',\
                                   'state, nstate, pstate')
      switch(state)
      {
         case PC_ST_SEND_UPL  :
         case PC_ST_SEND_FIL  :
         case PC_ST_SEND_SOT  :
         case PC_ST_SEND_EOT  :
         case PC_ST_SEND_DATA :
         case PC_ST_SEND_CAN  :
         case PC_ST_SEND_NAK  :
            if((rc = _pc_send(functab(:state:), stmpdat, stmplen)) != 0)
            {
               nstate = PC_ST_OS_ERR;
            }
         break;
         case PC_ST_RECV_EOT_ACK  :
        #if SYS_TYPE != ARCH360           /* Tweak!! Till I figure out */
            break;                        /* how to release console    */
        #endif                            /* after IBM does last write */
         case PC_ST_RECV_UPL_ACK  :
         case PC_ST_RECV_FIL_ACK  :
         case PC_ST_RECV_SOT_ACK  :
         case PC_ST_RECV_DATA_ACK :
            if( (rc = _pc_recv(functab(:state:), rtmpdat,&rtmplen)) != 0)
            {
               switch( rc )
               {
                  case PC_ERR_OS:
                     nstate = PC_ST_OS_ERR;
                  break;
                  case PC_ERR_TOUT :
                     nstate = PC_ST_TOUT_ERR;
                  break;
                  case PC_ERR_CAN_RCVD:
                     nstate = PC_ST_TRANSFER_ABORT;
                  break;
                  case PC_ERR_NAK_RCVD:
                     nstate = PC_ST_REQ_NAKED;
                     #D_show('Rcvd NAK; note err and jump to %d state',\
                                                     'state-1')
                     nxtstate(:PC_ST_SEND_NAK:) = state - 1;
                              /* Jump to state whose ACK we're waiting */
                              /* for. Store info where;after NAK state */
                  break;
                  case PC_ERR_UPL_RCVD:
                     if ( state == PC_ST_RECV_UPL_ACK )
                     {                /* if recving ACK for UPL */
                        #D_say('Upl requested by remote')
                        nstate = PC_ST_SEND_CAN;
                        rrc = PC_ERR_UPL_REQ;
                        break;
                                      /* Sorry, we're not ready to     */
                     }                /* receive yet.                  */
                  default:
                     if( rc > OSK_ERR )
                        nstate = PC_ST_OS_ERR;
                     else
                     {
                        nxtstate(:PC_ST_SEND_NAK:) = state;
                        nstate = PC_ST_PROTO_ERR_SEND_NAK;
                     }             /* Remember the state we were in so */
                  break;           /* we can return after sending NAK  */
               }
            }  /* if */
            else
            {                     /* We received what we were waiting */
               touterr = 0;       /* expecting. Things must be going  */
               prterr = 0;        /* right, so lets clear any previous*/
               nakerr = 0;        /* errs we accumlated.              */
               #D_dump(rtmpdat, rtmplen, 'Expected pkt RCVD!!')
            }
         break;
         case PC_ST_PREP_FIL :
            rc = pc_prsnd(filnam, fillen, &tmpbeg, \
                          pktnum, stmpdat, &stmplen );
            tmpbeg = 0;                /* reset begining ptr for data */
            #D_dump(stmpdat, stmplen, 'Filename after protoized')
           #if 0
            rc = os_tran( EBC2ASC, stmpdat, stmplen );
            #D_dump(stmpdat, stmplen, 'File name after trans into ASC')
           #endif
         break;
         case PC_ST_PKTIZE_DATA :
            pktnum++;                          /* increment pktnum     */
            if(pc_prsnd(data, datlen, &tmpbeg, \
                        pktnum, stmpdat, &stmplen ) != 0)
               nstate = PC_ST_SEND_EOT;        /* no more data to send */

            #D_dump(stmpdat, stmplen, 'Data after being protoized')
            #D_show('Pktnum=%d, Data encoded=%d, Data left=%d',\
                                'pktnum, tmpbeg, datlen-tmpbeg')
           #if 0
            rc = os_tran( EBC2ASC, stmpdat, stmplen);
            #D_dump(stmpdat,stmplen,'Data after being translated in ASC')
           #endif
         break;

         case PC_ST_OS_ERR:
            if( oserr >= PC_ERR_MAX_OS )
            {
               #D_say('Too many OS errs; exiting recv mode')

               nstate = PC_ST_EXIT;
               rrc = PC_OS_ERR_EXCEEDED;
            }
            else
            {
               oserr++;
               nstate = pstate;

               #D_show('OS err. count is %d, jumping to state = %d',\
                                            'oserr, nstate');
            }
         break;
         case PC_ST_TOUT_ERR:
            if( touterr >= PC_ERR_MAX_TOUT )
            {
               #D_say('Too many Time-out errs; exiting recv mode')

               nstate = PC_ST_SEND_CAN;
               rrc = PC_TOUT_ERR_EXCEEDED;
            }
            else
            {
               touterr++;
               nstate = PC_ST_SEND_NAK;
               nxtstate(:PC_ST_SEND_NAK:) = pstate - 1;
                                       /* Send NAK and then execute   */
                                       /* the state whose ACK we were */
                                       /* waiting for.                */
               #D_show('TOUT err. count is %d, jumping to state = %d',\
                                               'touterr, nstate')
            }
         break;
         case PC_ST_REQ_NAKED :
            if( nakerr >= PC_ERR_MAX_NAK )
            {
               #D_say('Too many NAKed packets; exiting recv mode')

               nstate = PC_ST_SEND_CAN;
               rrc = PC_NAK_ERR_EXCEEDED;
            }
            else
            {
               nakerr++;
               nstate = nxtstate(:PC_ST_SEND_NAK:);

               #D_show('NAK err. count is %d, jumping to state = %d',\
                                               'nakerr, nstate')
            }      /* jump to state which was naked */
         break;
         case PC_ST_PROTO_ERR:
         case PC_ST_PROTO_ERR_SEND_NAK:
            if( prterr >= PC_ERR_MAX_PROTO )
            {
               #D_say('Too many PROTO errs; exiting recv mode')

               nstate = PC_ST_SEND_CAN;
               rrc = PC_PROTO_ERR_EXCEEDED;
            }
            else
            {
               prterr++;
               if( state == PC_ST_PROTO_ERR_SEND_NAK )
                  nstate = PC_ST_SEND_NAK;
               else
                  nstate = pstate;

               #D_show('PROTO err. count is %d, jumping to state = %d',\
                                               'prterr, nstate')
            }
         break;
         case PC_ST_TRANSFER_ABORT :
            rrc = PC_ERR_ABORT;
         case PC_ST_EXIT :
            os_memd( stmpdat );
            os_memd( rtmpdat );
            #D_return( rrc );
         break;
         default:
            #D_show('State %d not doing hell of a lot!!!\n', state)
      }  /* switch(state) */

   }  /* for */
} /* pc_send() */


