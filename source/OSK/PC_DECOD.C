#include "defs.h"
#include "debug.h"
#include "pcomm.h"

sint pc_decod(inbuf, outch, chrnum)
   byte      *inbuf,
                      *outch;
   hword                     *chrnum;
{
   byte tmpch, ch;
   sint inoffset,
        rc = 0;

   #D_start(pc_decod, pcomm, 'inptr=%x', 'inbuf')

   switch(*inbuf)
   {
      case PC_HIBIT_PFX:
         switch(*(inbuf+1))
         {
            case PC_HIBIT_PFX:
            case PC_RPT_PFX:
               rc = PC_ERR_PROTOCOL;
               break;
            case PC_CTL_PFX:
               ch = *(inbuf+2);
               tmpch = ch ^ PC_MOD64; /* complement 7th bit by XOR */
               if ((tmpch <= PC_CTL_CHRS) || (tmpch == PC_DEL_CHR))
               {
                  ch = tmpch;              /* complement 7th bit */
               }
               ch |= PC_x80;               /* turn 8th bit on    */
               inoffset = 3;
               break;
            default:
               ch = *(inbuf+1) | PC_x80;    /* 8th bit on        */
               inoffset = 2;
               break;
         }
         break;                  /* PC_HIBIT_PFX              */
      case PC_CTL_PFX:
         ch = *(inbuf+1);
         if( !((ch == PC_CTL_PFX) || (ch == PC_HIBIT_PFX) \
                                || (ch == PC_RPT_PFX)) )
         {
            if(   ( ch <= (PC_CTL_CHRS ^ PC_MOD64))\
               || ( ch == (PC_DEL_CHR  ^ PC_MOD64)))
            {
               ch ^= PC_MOD64;
            }
         }
         inoffset = 2;
         break;
      case PC_RPT_PFX:
         rc = PC_ERR_PROTOCOL;
         break;
      default:
         ch = *inbuf;
         inoffset = 1;
         break;
   }
   if ( rc == 0 )             /* if there is no error             */
   {
      *chrnum = inoffset;
      *outch = ch;
   }

   #D_return( rc );
}
