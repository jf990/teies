#include "xmodem.h"

xget( fp, opts)
   FILE *fp;
   int  opts;
{
   byte  buf(:BSIZE:),
         blknum = 1,
         inchar,
         crchigh,
         chksum = 0;
   int crc,
       iput = BSIZE;
   unsigned crcsum;
   register i;

   sleep(10);                    /* sleep while sender gets ready */

   crc = (opts & CRC);
   if ( crc )
      putbyte( CRCINIT );
   else
      putbyte( NAK );
   for EVER
   {
      if ( getbyte(&inchar, 10) != 0)
      {
         err("Timeout during SOH");
         if ( crc )
            putbyte( CRCINIT );
         else
            putbyte( NAK );
         continue;
      }
      if ( inchar == EOT )
         break;
      if ( inchar == CAN )
      {
         err("CAN block %u", blknum);
         cleanup(-1);
         return(-1);
      }
      if ( inchar != SOH )
      {
         err("Bad SOH block%u: %#x", blknum, (inchar & MAXBLK));
         putbyte(NAK);
         continue;
      }
      if ( getbyte(&inchar, 2) != 0 )
      {
         err("Timeout block %u during blocknum", blknum);
         putbyte(NAK);
         continue;
      }
      if ( (inchar & MAXBLK) != blknum)
      {
         err("Expected blocknum %u, got %u", blknum, (inchar & MAXBLK));
         putbyte(NAK);
         continue;
      }
      if ( getbyte(&inchar, 2) != 0 )
      {
         err("Timeout block %u during -blocknum", blknum);
         putbyte(NAK);
         continue;
      }
      if ((inchar & MAXBLK) != (~blknum & MAXBLK))
      {
         err("Expected ~blocknum %u, got %u", (~blknum & MAXBLK),       \
                                              (inchar & MAXBLK));
         putbyte(NAK);
         continue;
      }

/* And finally read rest of the block! */

      for (i = 0; i < BSIZE; i++)
      {
         if ( getbyte(buf(:i:), 2) != 0 )
         {
            break;
         }
      }
      if ( i < BSIZE )
      {
         err("Timeout data recv, char #%d", i);
         putbyte( NAK );
         continue;
      }
      if( crc );
      {
         if( getbyte( crchigh, 2) != 0)
         {
            err("Timeout crc hibyte");
            putbyte(NAK);
            continue;
         }
         crchigh &= MAXBLK;
      }
      if ( getbyte( &inchar, 2) != 0 )
      {
         err("Timeout checksum or crc lowbyt");
         putbyte( NAK );
         continue;
      }

/* Now that we have the packet, lets do checksum */

      chksum = 0;
      crcsum = 0;
      for( i = 0; i < BSIZE; i++)
      {
         upsum(buf(:i:), &chksum, &crcsum);
      }
      if ( crc )
      {
         upsum(0, &chksum, &crcsum);
         upsum(0, &chksum, &crcsum);
         if ((inchar & MAXBLK) + (crchigh << 8) != crcsum )
         {
            err("Expected crc %u but got %u", crcsum,  \
                                 (inchar & MAXBLK) + ( crchigh << 8));
            putbyte( NAK );
            continue;
         }
      }
      else
      {
         chksum %= 256;
         if ( chksum != (inchar & MAXBLK) )
         {
            err("Expected checksum %u, got %u", chksum,
                                            (inchar & MAXBLK));
            putbyte( NAK );
            continue;
         }
      }
      putbyte( ACK );

      if (opts & LF)
         for(i = 0, iput = 0; i < BSIZE; i++)
         {
            if( buf(:i:) == MSDOS_EOF )
               break;
            if (buf(:i:) != '\r')
               buf(:iput++:) = buf(:i:);
         }  /* for */
      fwrite( buf, iput, 1, fp);
      blknum++;
      blknum %= 256;
   }  /* for */
   putbyte( ACK );
   cleanup( 0 );
}
