#include "defs.h"
#include "debug.h"
#include "osk.h"
#include "pcomm.h"

sint _pc_send(func, pkt, pktlen)
   byte func,
        *pkt;
   hword pktlen;
{
   sint   rc = 0;

   #D_start(_pc_send,pcomm,'Func=%x, pktptr=%x, pktlen=%d','func, pkt, \
                                                            pktlen')
   *(pkt)   = PC_SOH;
   *(pkt+1) = func;

   if( !((func == PC_FIL) || (func == PC_DATA)) )  /* only file & data */
      pktlen = 2;                      /* pkts have longer length      */

   *((byte *) pkt + pktlen) = PC_CR;   /* put RETURN at the end of */
   pktlen++;                           /* the packet and increment len */

   rc = os_conn(CON_WRITE, CON_DEVICE, pkt, &pktlen, CONFLAGS);

   #D_return( rc );
}
