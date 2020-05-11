#include "defs.h"
#include "debug.h"
#include "pcomm.h"

sint pc_prsnd(inbuf, inlen, inbeg, pktnum, pkt, pktlen)
   byte *inbuf,
         pktnum,
        *pkt;
   hword inlen,
         *inbeg,
         *pktlen;

{
   byte  *orgpkt = NULL,
         *tmppkt = NULL,
         *lasinbuf = NULL,
         *orginbuf = NULL,
         *datpktbeg = NULL;
   hword len = 0,
         crc = 0;
   sint  rc = 0;

   #D_start(pc_prsnd, pcomm, 'inptr=%x, inlen=%x, inbeg=%d, pktnum=%d,\
     pktptr=%x, pktlen=%d','inbuf, inlen, *inbeg, pktnum, pkt, *pktlen')

if( *inbeg <= inlen )
{
   *pktlen = 0;
   lasinbuf = inbuf + inlen;     /* las chr to be encoded         */
   inbuf += *inbeg;              /* point where to start encoding */
   orginbuf = inbuf;             /* keep copy for later use for len */
   orgpkt = pkt;                 /* copy to figure out len          */
   pkt += 2;                     /* leave room for SOH and func */
#if 0
   pkt += 6;                     /* leave room for len(encoded) */
#endif
   *pkt++ = 0;
   *pkt++ = 0;
   *pkt++ = 0;
   *pkt++ = 0;
   *pkt++ = 0;
   *pkt++ = 0;

   *pkt++ = PC_STX;                /* text starts here; increment */

   pkt += pc_encod( pktnum, pkt);  /* now encode pktnum           */

   datpktbeg = pkt;                /* encodd data begins here      */

   for(;(inbuf <= lasinbuf) && ((pkt - orgpkt) < PC_PKTLEN);inbuf++)
   {              /* while there is more data and room in pkt buffer */
      pkt += pc_encod( *inbuf, pkt);
   }

   crc = pc_crc(CRC_CALC, datpktbeg, (pkt - datpktbeg));
                     /* calc crc from where actual encodd data starts */
   *pkt++ = PC_ETX;           /* mark end of text  */
                              /* now encod crc; left byte 1st */
   pkt += pc_encod( (byte) (crc / 0x100), pkt);
                              /* encod 2nd(right) byte of crc */
   pkt += pc_encod( (byte) (crc % 0x100), pkt); /* last 2 digits */
                    /* now encod len of actual data encodd */
   len = inbuf - orginbuf;
   tmppkt = orgpkt + 2;       /* point where len starts in pkt */
                              /* encod 1st(left) byte of pkt length */
   tmppkt += pc_encod( (byte) (len / 0x100), tmppkt);
                              /* encod 2nd(right) byte of pkt length */
   tmppkt += pc_encod( (byte) (len % 0x100), tmppkt);

                     /* now tell caller what we have done */
   *inbeg += len;    /* add data encodd this time to already done */
   *pktlen = pkt - orgpkt;    /* len of the packet */
   #D_show('Pktlen=%d',*pktlen)
}
else
   rc = -1;    /* tell caller no more data to encode */

   #D_return( rc );
}
