#include "defs.h"
#include "debug.h"
#include "pcomm.h"

hword pc_crc(func, buf, len)
   sint func;
   byte *buf;
   hword len;
{
      extern hword crc_tab(:CRC_TAB_SIZE:);
      register sint count;
      register sint i;
      byte zero = 0;
      hword crc = 0;
      register byte *data;
      hword x = 0;
      byte  y = 0, z = 0;

   #D_start(pc_crc,pcomm,'Funtion=%d,Bufptr=%x,Buflen=%d','func,buf,len')

   if( func == CRC_CALC )
   {
      #D_dump(buf, len, 'Data being CRCed')
   }
   else
   {
      #D_say('Setting up CRC table')
   }

   switch(func)
   {
   case CRC_SETUP:
      for(count = 0;count < CRC_TAB_SIZE;count++)
      {
         crc = count << 8;
         crc ^= (hword) (zero) << 8;
         for(i = 0;i < 8;i++)
         {
            if (crc & 0x8000)
               crc = (crc<<1)^0x1021;
            else
               crc <<= 1;
         }
         crc_tab(:count:) = crc;

      }
      break;   /* SET_UP */
   case CRC_CALC:
      data = buf;

      while(len--)
      {
         y = crc >> 8;
         z = y ^ (*data++);
         x = crc_tab(:z:);
         crc = x ^ (crc << 8);
      }
      break;   /* calc */
   }        /* switch */
   #D_return(crc);
}
