#include "defs.h"
#include "debug.h"
#include "pcomm.h"
#include "osk.h"

sint pc_on(data, len)
   byte *data;
   hword *len;
{
   sint rc = 0;
   hword junklen = 0;

   #D_start(pc_on, pcomm, 'Struct ptr=%x, len=%d','data, *len')

   rc = os_conn(CON_ON, CON_DEVICE, data, len, CONFLAGS);
   if( rc == 0 )
      pc_crc(CRC_SETUP, NULL, junklen);

   #D_return( rc )
}
