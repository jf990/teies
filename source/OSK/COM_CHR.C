#include "xmodem.h"

sint con_pfx( ch, buf)
   byte       ch,
                 *buf;
{
   sint bufptr = 0;

   if (( ch & CON_0x80 ))      /* if hi-bit is on in ch      */
   {
      *(buf+bufptr) = CON_HIBIT_PFX;/* mark next char is as hi-bit on*/
      bufptr++;                   /* increment pointer */
      ch &= CON_x7F;              /* Turn hi bit off                  */
   }
   if((ch == CON_CTL_PFX) || (ch == CON_HIBIT_PFX) ||(ch == CON_RPT_PFX))
   {
      *(buf + bufptr) = ch;
      bufptr++;
   }

   if ( (ch <= CON_CTL_CHR) || (ch == CON_DEL_CHR) )
   {
      *(buf + bufptr) = CON_CTL_PFX;
      bufptr++;
      ch ^= CON_MOD64;     /* do XOR so, bit 7  is complemented  */
   }

   *(buf + bufptr) = ch;
   bufptr++;

   return(bufptr);
}
