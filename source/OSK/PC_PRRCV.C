#include "defs.h"
#include "debug.h"
#include "pcomm.h"

sint pc_prrcv(inbuf, inlen, pktnum, outbuf, outlen)
   byte      *inbuf,       *pktnum, *outbuf;
   hword             inlen,                *outlen;
{
   sint  inptr = 0,  outptr = 0,
         rrc = -2,
         rc = 0,     etxptr = 0;
   hword crc = 0, rcvcrc = 0,
         chrnum = 0,
         len = 0, tmpoutlen = 0;
   byte *lasinbuf = NULL,
        *orgoutbuf = NULL,
        *datinbeg = NULL,
        laspktnum,
        outch, outch2;

   #D_start(pc_prrcv, pcomm,'inptr=%x, inlen=%d, pktnum=%d, outptr=%x,\
                    outlen=%d','inbuf, inlen, *pktnum, outbuf, *outlen')
   #D_dump(inbuf, inlen, 'Data to de-protoized')

   laspktnum = *pktnum + 1;
   lasinbuf = inbuf + inlen;        /* last inbuf char position */
   orgoutbuf = outbuf;              /* copy start of output buffer */
   inbuf++;                               /* skip SOH char         */
   inbuf++;                               /* skip func char         */
                                 /* lets extract len */
   if((rc = pc_decod( inbuf, &outch, &chrnum)) == 0 )/* left byte 1st */
   {
      inbuf += chrnum;    /* increment buffer with # of chrs processed */
      #D_show('Left len->ptr=%x, byte=%x, #of chrs processed=%d',\
                                       'inbuf, outch, chrnum')
      if( pc_decod( inbuf, &outch2, &chrnum) == 0 )
      {
         inbuf += chrnum; /* increment buffer with # of chrs processed */
         len = (outch << 8) + outch2; /* calc len  */
         /* look for start of text while there is some thingin buffer */
         for(;(*inbuf != PC_STX) && (inbuf <= lasinbuf);inbuf++);
         if( *inbuf == PC_STX )
         {
            inbuf++;          /* Skip STX chr  */
                           /* lets extract pkt number */
            rc = pc_decod(inbuf, pktnum, &chrnum);
            inbuf += chrnum;     /* offset # of chrs processed */

            datinbeg = inbuf;    /* actual encodd data begins here */
            for(;(*inbuf != PC_ETX) && (inbuf <= lasinbuf) && (rc == 0);)
            {
               rc = pc_decod(inbuf, outbuf, &chrnum);
               inbuf += chrnum;     /* offset # of chrs processed */
               outbuf++;            /* increment buffer */
            }
            if( (*inbuf == PC_ETX) && (rc == 0) )
            {
                        /* calc crc from where data actually begin */
               crc = pc_crc(CRC_CALC, datinbeg, (inbuf-datinbeg));
               inbuf++;       /* skip ETX chr */
                              /* lets extract crc now */
               if( (rc = pc_decod(inbuf, &outch, &chrnum)) == 0)
               {                              /* left byte */
                  inbuf += chrnum;
                  if( (rc = pc_decod(inbuf, &outch2, &chrnum)) == 0)
                  {                           /* now right byte */
                     inbuf += chrnum;
                     rcvcrc = (outch << 8) + outch2;
                     *outlen = outbuf - orgoutbuf; /* # of chrs read */
                        /* if rcvd data crc doesn't match crc rcvd   */
                        /* or rcvd len doesn't equal sent len        */
                        /* or if pkt # is consecutive  */
                        /* or if there is something still in buffer  */
                           /* to be processed */
                     if( (crc == rcvcrc) && (len == *outlen) && \
                       (*pktnum == laspktnum))
                        rrc = 0;
                  }  /* decod right crc byte */
               }  /* decod left crc byte */
            }  /* PC_ETX */
         }  /* PC_STX */
      }  /* decod right len byte */
   }  /* decod left len byte */
   if( rrc != 0 )
   {
      *outlen = 0;
      rc = PC_ERR_PROTOCOL;
   }

   #D_return( rc );
}
