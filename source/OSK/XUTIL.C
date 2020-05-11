#include "xmodem.h"

fillbuf(fp, buf, lf)
   FILE *fp;
   char *buf;
   int  lf;
{
   int i = 0,
       ch = 0;
   static int cr_held;

   if ( cr_held )
   {
      buf(:i:) = '\n';
      i++;
      cr_held--;
   }
   for(; i < BSIZE; i++)
   {
      if( (ch = getc(fp)) == EOF)
         break;
      if( ch == '\n' && lf)
      {
         buf(:i:) = '\r';
         if ( i == BSIZE-1)
         {
            cr_held++;
            return( BSIZE );
         }
         buf(:i+1:) = '\n';
         i++;
      }
      else
         buf(:i:) = ch;
   }
   return( i );
}
/* ------------------------------------------------------------------- */
upsum(ch, chksum, crcsum, crc)
   char  ch,
         *chksum;
   int   *crcsum,
         crc;
{
   register unsigned i;

   if ( crc )
   {
      *crcsum ^= ( (int) ch) << 8;
      for(i = 0; i < 8; ++i)
         if (*crcsum & 0x8000)
            *crcsum = *crcsum << 1 ^ 0x1021;
         else
            *crcsum <<= 1;
   }
   else
      *chksum += ch;
}
/* ------------------------------------------------------------------- */
err(str, arg1, arg2)
   char *str;
   int arg1, arg2;
{
   extern FILE *errfile;

   fprintf( errfile, str, arg1, arg2);
   fprintf( errfile, "\n");
}
cleanup(flag)
   int flag;
{
   extern FILE *errfile;

   err("Transfer ended:%d", flag);
}
