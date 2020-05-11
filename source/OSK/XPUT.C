#include "xmodem.h"

xput(fp, opts)
   FILE *fp;
   int opts;
{
   register i;
   byte  inchar,
         outchar,
         buf(:BSIZE:),
         blknum = 1,
         crclo,
         chksum;
   int   cread,
         crc;
   unsigned crcsum;

   if ( getbyte( &inchar, 60) != 0)
   {
      err("Timeout while waiting to send");
      cleanup(-1);
      return(-1);
   }
   if( inchar == CRCINIT)
      crc = 1;
   else
      if ( inchar == NAK )
         crc = 0;
      else
      {
         err("No startup %s", (crc) ?"CRCINIT": "NAK");
         cleanup(-1);
         return(-1);
      }
   cread = fillbuf(fp, buf, (opts & LF));
   while ( cread )
   {
      for (i=cread; i < BSIZE; i++)
         buf(:i:) = 0;
      putbyte( SOH );
      putbyte( blknum );
      outchar = (~blknum & MAXBLK);
      putchar( outchar );
      putbuf( buf, BSIZE);
      for( chksum = 0, crcsum = 0, i = 0;i < BSIZE; i++)
         upsum(buf(:i:), &chksum, &crcsum);
      if ( crc )
      {
         upsum(0, &chksum, &crcsum);
         upsum(0, &chksum, &crcsum);
         crclo = crcsum;
         outchar = ( crcsum >> 8);
         putbyte( outchar );
         putbyte( crclo );
      }
      else
      {
         chksum %= 256;
         putbyte( chksum );
      }
      if ( getbyte(inchar, 15) != 0)
      {
         err("Timeout after block %u", blknum);
         continue;
      }
      if ( inchar == CAN )
      {
         err("CAN after black %u", blknum);
         cleanup(-1);
         return(-1);
      }
      if ( inchar != ACK )
      {
         err("Non-ACK after block %u: %#x", blknum, inchar);
         continue;
      }
      cread = fillbuf( fp, buf, (opts & LF));
      blknum++;
      blknum %= 256;
   }

   for EVER
   {
      putbyte( EOT );
      if ( getbyte(15, inchar) != 0 )
      {
         err("Timeout during EOT");
         continue;
      }
      if ( inchar == CAN)
      {
         err("Can during EOT");
         cleanup(-1);
         return(-1);
      }
      if ( inchar != ACK)
      {
         err("Non-ACK during EOT: %#x", inchar);
         continue;
      }
      break;
   }  /* for */
   cleanup(0);
   return(0);
}
