#include "defs.h"
#include "xmodem.h"

sint con_upfx(buf, len)
   byte     **buf;
   sint           *len;
{
   byte *inbuf, *outbuf;
   sint  inlen,  outlen = 0,
         inptr = 0,  outptr = 0,
         rc = 0;

   inbuf = *buf;
   inlen = *len;

   outbuf = malloc( inlen );

   inptr++;                               /* skip SOH char             */
   for(;(*(inbuf+inptr) != CON_STX) && (inptr <= inlen)\
                                    && (rc == 0);outptr++)
   {                                   /* process the len          */
      rc = con_uchr((outbuf+outptr), (inbuf+inptr), inlen, &inptr);
   }
   inptr++;                             /* skip STX char             */
   for(;(*(inbuf+inptr) != CON_ETX) && (inptr <= inlen)\
                                    && (rc == 0);outptr++)
   {                                 /* now process the data      */
      rc = con_uchr((outbuf+outptr), (inbuf+inptr), inlen, &inptr);
   }
   inptr++;                            /* skip ETX char  */
   for(;(inptr <= inlen) && (rc == 0);outptr++)
   {                                /* figure out the actual crc */
      rc = con_uchr((outbuf+outptr), (inbuf+inptr), inlen, &inptr);
   }

   *buf = outbuf;
   *len = outptr;

   return( rc );
}
