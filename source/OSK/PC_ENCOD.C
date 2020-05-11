#include "defs.h"
#include "debug.h"
#include "pcomm.h"

sint pc_encod( ch, buf)
   byte       ch,
                 *buf;
{
   byte *orgbuf = NULL;

   #D_start(pc_encod, pcomm, 'Char being encoded=%x, bufptr=%x','ch,buf')

   orgbuf = buf;

   if (( ch & PC_x80 ))          /* if hi-bit is on in ch            */
   {
      *buf++ = PC_HIBIT_PFX;     /* mark next char is as hi-bit on   */
      ch &= PC_x7F;              /* Turn hi bit off                  */
      #D_show('Hi bit turned off. Char = %x','ch')
   }
   if((ch == PC_CTL_PFX) || (ch == PC_HIBIT_PFX) ||(ch == PC_RPT_PFX))
   {
      *buf++ = PC_CTL_PFX;       /* next char is quoted char         */
      #D_say('Char is one of the control characters')
   }

   if ( (ch <= PC_CTL_CHRS) || (ch == PC_DEL_CHR) )
   {
      *buf++ = PC_CTL_PFX;
      ch ^= PC_MOD64;            /* do XOR so, bit 7 is complemented */
      #D_show('Char is non-printable. Char = %x','ch')
   }

   *buf++ = ch;

   #D_return(buf - orgbuf);
}
