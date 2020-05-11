#include "defs.h"
#include "xmodem.h"

sint con_uchr(outbuf, inbuf, inlen, inptr)
   byte      *outbuf,
                     *inbuf;
   sint                      inlen;
   sint                            *inptr;
{
   char ch;
   sint inoffset,
        rc = 0;

   switch(*inbuf)
   {
      case CON_HIBIT_PFX:
         switch(*(inbuf+1))
         {
            case CON_HIBIT_PFX:
            case CON_RPT_PFX:
               rc = CON_ERR + CON_ERR_PROTOCOL;
               break;
            case CON_CTL_PFX:
               ch = *(inbuf+2);
               if ((ch <= CON_CTL_CHRS) || (ch == CON_DEL_CHR))
               {
                  ch ^= CON_MOD64;          /* complement 7th bit */
               }
               ch |= CON_x80;               /* turn 8th bit on    */
               inoffset = 3;
               break;
            default:
               ch = *(inbuf+1) | CON_x80;    /* 8th bit on        */
               inoffset = 2;
               break;
         }
         break;                  /* CON_HIBIT_PFX              */
      case CON_CTL_PFX:
         ch = *(inbuf+1);
         if(   ( ch <= (CON_CTL_CHRS ^ CON_MOD64))\
            || ( ch == (CON_DEL_CHR  ^ CON_MOD64)))
         {
            ch ^= CON_MOD64;
         }
         inoffset = 2;
         break;
      case CON_RPT_PFX:
         rc = CON_ERR + CON_ERR_PROTOCOL;
         break;
      default:
         ch = *inbuf;
         inoffset = 1;
         break;
   }
   if ( rc == 0 )             /* if there is no error             */
   {
      *inptr = *inptr + inoffset;
      *outbuf = ch;
   }

   return( rc );
}
