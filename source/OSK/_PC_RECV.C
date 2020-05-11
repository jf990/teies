#include "defs.h"
#include "osk.h"
#include "pcomm.h"
#include "debug.h"

sint _pc_recv(func, rcvdata, rcvlen)
   byte func,
        *rcvdata;
   hword *rcvlen;
{
   sint  rc = 0, rrc = 0,
         readtry = 0;
   hword junklen = 0;
   byte  *lasrcvdata = NULL,
         *orgrcvdata = NULL,
         *pktstart   = NULL,
          pkthead    = PC_SOH;

   #D_start(_pc_recv,pcomm,'Func=%x, rcvptr=%x', 'func, rcvdata')

   *rcvlen = PC_MAX_PKTLEN;

   #if SYS_TYPE == ARCH8086
      for(; readtry < PC_MAX_READ_TRY; readtry++)
      {             /* wait here till there is something in read buf */
        #if 0

         os_conn(CON_QUERY, CON_DEVICE, NULL, &junklen, Q_RBUF);

         if( junklen >= PC_MIN_PKTLEN )
            break;      /* at lease there should be header info       */

         os_wait(1);    /* does not make any sense to have to wait    */
                        /* there is something in read buffer          */
        #else

         os_conn(CON_QUERY, CON_DEVICE, &pkthead, &junklen, Q_SRCH_F );
         #D_show('SOH at %d',junklen)

         if( junklen != 0 )   /* if there is SOH in buffer then exit  */
            break;            /* loop and read in the biffer.         */

         os_wait(1);          /* or else wait                         */

        #endif
      }

      if( PC_MAX_READ_TRY == readtry ) /* if we exceeded the limit of */
         #D_return( PC_ERR_TOUT )       /* readtry, then time out err  */
   #endif

   if((rc = os_conn(CON_READ, CON_DEVICE, rcvdata,rcvlen,CONFLAGS)) == 0)
   {
      (*rcvlen)--;                      /* strip off PC_CR  */

      orgrcvdata = rcvdata;
      lasrcvdata = rcvdata + *rcvlen;   /* Point to end of the buffer */

      for(; rcvdata <= lasrcvdata; rcvdata++)
      {
         *rcvdata &= PC_x7F;           /* Strip high(8th) bit off     */
         if( *rcvdata == PC_SOH )      /* Start of the packet         */
            pktstart = rcvdata;        /* Yes, then remember where    */
      }                                /* Only remember last packet   */
      rcvdata = pktstart;              /* Receive buffer starts where */
                                       /* loop found last packet      */
      if ( rcvdata != NULL )           /* If we found SOH in our loop */
      {
         if( orgrcvdata != rcvdata )   /* if there is garbage infront */
         {                             /* of the pkt, then figure out */
                                       /* len of the pkt and overwrite*/
            *rcvlen = *rcvlen - (rcvdata - orgrcvdata);
            os_memc(rcvdata, orgrcvdata, (sint) *rcvlen);
         }

         #D_show('And function is %c(%x)','*(rcvdata+1),*(rcvdata+1)')

         switch( *(rcvdata+1) )     /* look at the function    */
         {
           #if 0
            case PC_ACK:
               if( func != PC_ACK )
                  rc = PC_ERR_ACK_RCVD;
               break;
            case PC_NAK:
               if( func != PC_NAK )
                  rc = PC_ERR_NAK_RCVD;
               break;
            case PC_CAN:
               if( func != PC_CAN )
                  rc = PC_ERR_CAN_RCVD;
               break;
            case PC_EOT:
               if( func != PC_EOT )
                  rc = PC_ERR_EOT_RCVD;
               break;
            case PC_SOT:
               if( func != PC_SOT )
                  rc = PC_ERR_SOT_RCVD;
               break;
            case PC_FIL:
               if( func != PC_FIL )
                  rc = PC_ERR_FIL_RCVD;
               break;
            case PC_DWN:
               if( func != PC_DWN )
                  rc = PC_ERR_DWN_RCVD;
               break;
            case PC_UPL:
               if( func != PC_UPL )
                  rc = PC_ERR_UPL_RCVD;
               break;
            case PC_DATA:
               if( func != PC_DATA )
                  rc = PC_ERR_DATA_RCVD;
               break;

           #else

            case PC_ACK:
            case PC_NAK:
            case PC_CAN:
            case PC_EOT:
            case PC_SOT:
            case PC_FIL:
            case PC_DWN:
            case PC_UPL:
            case PC_DATA:
               if( *(rcvdata+1) == func) /* If we received the function*/
                  rc = 0;                /* we expected then RC is good*/
               else                      /* else calculate the err code*/
                  rc = *(rcvdata+1) + PC_ERR_PROTOCOL;

               break;

           #endif

            default:
               rc = PC_ERR_INV_PKT_RCVD;
               *rcvlen = 0;
               break;
         }  /* function switch */
      }     /* if SOH was found */
      else
      {
          rc = PC_ERR_INV_PKT_RCVD;
          *rcvlen = 0;
      }
   }  /* if rc == 0 */

   #D_return( rc );
}
