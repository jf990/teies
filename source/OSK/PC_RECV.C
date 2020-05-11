/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  PC_RECV.C

:LEVEL.                 PCOMM

:AUTHOR.                Sal Johar

:DATE.                  4/27/88

:INPUT.                 filname : byte ptr, where to copy file name
                                  being recvd. already alloced.
                        fillen  : sint ptr, len allocated for filname.
                        data    : byte ptr, place where to recv data.
                        datalen : sint ptr, len allocated for data.

:OUTPUT.                rc:   0 if all OK, else error
                        filname : byte ptr, file name.
                        fillen  : sint ptr, filename length.
                        data    : byte ptr, data recvd.
                        datalen : sint ptr, data length.

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 -none-

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "debug.h"
#include "osk.h"
#include "pcomm.h"

sint pc_recv( filname, fillen, data, datalen)
   byte *filname,
        *data;
  hword *fillen,
        *datalen;
{
   byte  state = 0,
         pstate = 0,
         nstate = 0,
         pktnum = -1;           /* pktnum recvd */
   sint  rc = 0, rrc = 0,
         prterr = 0,
         oserr = 0,
         touterr = 0,
         nakerr = 0;
   hword rcvlen = 0,
         stmplen = 0,
         rtmplen = 0,
         outlen = 0;
   byte  *outdat = NULL,
         *stmpdat = NULL,
         *rtmpdat = NULL;
static byte nxtstate(::) = {
         PC_ST_RECV_UPL,  PC_ST_SEND_UPL_ACK,
         PC_ST_RECV_FIL,  PC_ST_COPY_FIL,      PC_ST_SEND_FIL_ACK,
         PC_ST_RECV_SOT,  PC_ST_SEND_SOT_ACK,
         PC_ST_RECV_EOT,  PC_ST_SEND_EOT_ACK,
         PC_ST_COPY_DATA, PC_ST_EXIT,
         PC_ST_CHK_PKT,   PC_ST_SEND_DATA_ACK,
         PC_ST_RECV_EOT,  PC_ST_NONE,
         PC_ST_SEND_CAN,  PC_ST_EXIT,
         PC_ST_SEND_NAK, PC_ST_ANY };
               /* Ptr in nxtstate table to jump after sending nak */
static byte functab(::) = { 0,
         PC_UPL,  PC_ACK,
         PC_FIL,  PC_NULL, PC_ACK,
         PC_SOT,  PC_ACK,
         PC_EOT,  PC_ACK,
         PC_NULL, PC_NULL,
         PC_NULL, PC_ACK,
         PC_EOT,  PC_NULL,
         PC_CAN,  PC_NULL,
         PC_NAK,  PC_NULL  };

   #D_start(pc_recv,pcomm,'filptr=%x, fillen=%x, datptr=%x, datlen=%x',\
                          'filname, *fillen, data, *datalen')

   stmpdat = os_mema( PC_MAX_PKTLEN );
   rtmpdat = os_mema( PC_MAX_PKTLEN );
   outdat = os_mema( PC_MAX_PKTLEN );

   *datalen = 0;           /* No data received yet    */

   for(;;)
   {
      pstate = state;
      if( state == nstate )
         state = nstate = nxtstate(:state:);
      else
         state = nstate;

      #D_show('States->Current=%d, Next=%d, Previous=%d','state,nstate,\
                                                               pstate')
      switch(state)
      {
         case PC_ST_RECV_UPL :
         case PC_ST_RECV_FIL :
         case PC_ST_RECV_SOT :
         case PC_ST_RECV_EOT :
            if( (rc = _pc_recv(functab(:state:), rtmpdat,&rtmplen)) != 0)
            {
               switch( rc )
               {
                  case PC_ERR_OS :
                     nstate = PC_ST_OS_ERR;
                  break;
                  case PC_ERR_TOUT :
                     nstate = PC_ST_TOUT_ERR;
                  break;
                  case PC_ERR_ACK_RCVD :
                  case PC_ERR_NAK_RCVD :
                     nstate = PC_ST_PROTO_ERR;
                  break;
                  case PC_ERR_CAN_RCVD :
                     nstate = PC_ST_TRANSFER_ABORT;
                  break;
                  case PC_ERR_DATA_RCVD :
                     if( state == PC_ST_RECV_EOT )
                     {
                        #D_say('Data rcvd!!!')
                        nstate = PC_ST_CHK_PKT;
                        break;
                     }
                  default :
                     nstate = PC_ST_PROTO_ERR_SEND_NAK;
                     nxtstate(:PC_ST_SEND_NAK:) = state;
                     #D_show('Send nak and jump to %d state',state)
                  break;
               }     /* switch */
            }
            else                           /* Since we received the   */
            {                              /* expected pkt, clear any */
               touterr = 0;                /* error counts.           */
               prterr  = 0;
               oserr   = 0;
               #D_dump(rtmpdat, rtmplen,'Data rcvd')
            }
         break;
         case PC_ST_SEND_EOT_ACK :        /* Tweak!! Till I figure out*/
        #if SYS_TYPE == ARCH360           /* how to unlock console    */
            break;                        /* after the last write.    */
        #endif                            /* Don't do anything if IBM */
         case PC_ST_SEND_UPL_ACK :
         case PC_ST_SEND_FIL_ACK :
         case PC_ST_SEND_SOT_ACK :
         case PC_ST_SEND_DATA_ACK :
         case PC_ST_SEND_NAK :
         case PC_ST_SEND_CAN :
            if( (rc = _pc_send(functab(:state:), stmpdat, stmplen)) !=0)
            {
               nstate = PC_ST_OS_ERR;
            }
         break;
         case PC_ST_COPY_FIL :
           #if 0
            rc = os_tran( ASC2EBC, rtmpdat, rtmplen );
            #D_dump(rtmpdat, rtmplen,'Filename after translated in EBC')
           #endif
            if(pc_prrcv(rtmpdat,rtmplen, &pktnum, filname, fillen ) != 0)
            {
               *fillen = 0;      /* no, file name */
               nstate = PC_ST_PROTO_ERR_SEND_NAK;
               nxtstate(:PC_ST_SEND_NAK:) = PC_ST_RECV_FIL;
                        /* jump to recvfile state after sending nak */
            }
            else
            {
               #D_dump(filname, *fillen, 'Filename after de-protoized')
            }
         break;
         case PC_ST_CHK_PKT :
           #if 0
            rc = os_tran( ASC2EBC, rtmpdat, rtmplen );
            #D_dump(rtmpdat, rtmplen,'Filename after translated in EBC')
           #endif
            if(pc_prrcv(rtmpdat,rtmplen, &pktnum, outdat, &outlen ) != 0)
            {
               nstate = PC_ST_PROTO_ERR_SEND_NAK;
               nxtstate(:PC_ST_SEND_NAK:) = PC_ST_RECV_EOT;
            }
            else
            {
               #D_dump(outdat, outlen, 'Data after de-protoized')
               #D_show('Data already rcvd=%d', rcvlen)
               rc = os_memc( outdat, (data + rcvlen), outlen );
                   /* copy data into recv buffer sent in by the caller */
                   /* offset already received data */
               rcvlen += outlen;
            }
         break;
         case PC_ST_COPY_DATA:
            #if 0
            #endif
         break;
         case PC_ST_TOUT_ERR:
            if( touterr >= PC_ERR_MAX_TOUT )
            {
               #D_say('Too many TOUT errs; exiting sending mode')
               nstate = PC_ST_EXIT;
               rrc = PC_TOUT_ERR_EXCEEDED;
            }
            else
            {
               touterr++;
               nxtstate(:PC_ST_SEND_NAK:) = pstate - 1;
               nstate = PC_ST_SEND_NAK;
               #D_show('TOUT err; Count is %d, next state = %d',\
                                               'touterr, nstate')
            }
         break;
         case PC_ST_OS_ERR:
            if( oserr >= PC_ERR_MAX_OS )
            {
               #D_say('Too many OS errs; exiting sending mode')
               nstate = PC_ST_EXIT;
               rrc = PC_OS_ERR_EXCEEDED;
            }
            else
            {
               oserr++;
               nstate = pstate;
               #D_show('OS err; Count is %d, next state = %d',\
                                               'oserr, nstate')
            }
         break;
         case PC_ST_PROTO_ERR_SEND_NAK :
         case PC_ST_PROTO_ERR:
            if( prterr >= PC_ERR_MAX_PROTO )
            {
               #D_say('Too many PROTO errs; sending CAN')
               nstate = PC_ST_SEND_CAN;
               rrc = PC_PROTO_ERR_EXCEEDED;
            }
            else
            {
               prterr++;
               if( state == PC_ST_PROTO_ERR_SEND_NAK )
               {
                  #D_show('Sending NAk and jumping to %d state',\
                           'nxtstate(:PC_ST_SEND_NAK:)')
                  nstate = PC_ST_SEND_NAK;
               }
               else
                  nstate = pstate;
               #D_show('PROTO err; Count is %d, next state = %d',\
                                               'prterr, nstate')
            }
         break;

         case PC_ST_TRANSFER_ABORT :
            rrc = PC_ERR_ABORT;
         case PC_ST_EXIT :          /* if no err, then set data length */
            if( rrc == 0 )
               *datalen = rcvlen;   /* before leaving, translate data */
            os_memd( stmpdat );
            os_memd( rtmpdat );
            os_memd( outdat );
            #D_return( rrc );
         break;
         default:
            #D_show('State %d not doing hell of a lot!!\n', state);
         break;
      }  /* switch(state) */

   }  /* for */
} /* pc_recv() */
