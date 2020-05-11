#include "defs.h"
#include "str.h"
#include "osk.h"
#include "xmodem.h"

sint con_pfx(buf, len)
   byte **buf;
   sint     *len;
{
   byte  *inbuf,  *outbuf,
         *tmpbuf;
   sint  tmplen,
         rc = 0,
         inlen,   outlen,
         inptr  = 0,
         outptr = 0;

   inlen = *len;
   inbuf = *buf;
   outbuf = malloc( inlen * 3 );  /* allocate atleast twice the mem */
   tmpbuf = malloc( 3 );  /* max # of char needed to prefix one char*/

   *(outbuf + outptr) = CON_SOH;            /* Start of header mark */
   outptr++;                               /* increment outbuf ptr */
                     /* now prefix the 2 byte len  */
   for (inptr = 0;inptr <= 1;inptr++)
   {
      tmplen = con_chr(*(inbuf+inptr), tmpbuf);
      rc = os_memc( tmpbuf, (outbuf+outptr), tmplen);
      outptr = outptr + tmplen;     /* increment outbuf ptr  */
   }
   *(outbuf + outptr) = CON_STX;   /* now mark the begging of text */
   outptr++;                       /* point to next outbuf         */
                  /* now, prefix actual data */
   for (inptr = 2;inptr < inlen;inptr++)
   {
      tmplen = con_chr(*(inbuf+inptr), tmpbuf);
      rc = os_memc( tmpbuf, (outbuf+outptr), tmplen);
      outptr = outptr + tmplen;     /* increment outbuf ptr  */
   }

   os_memd( tmpbuf );   /* don't need it any longer   */

   *len = outptr;
   *buf = outbuf;

   return( 0 );
}
