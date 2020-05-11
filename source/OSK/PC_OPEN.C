#include "defs.h"
#include "debug.h"
#include "pcomm.h"
#include "osk.h"

sint pc_open( bufptr, buflen )
byte *bufptr;
hword *buflen;
{
   sint rc = 0;

   #D_start(pc_open, pcomm, 'Struct ptr=%x, len=%d','bufptr, *buflen')

   #if( SYS_TYPE == ARCH360 )
      rc = os_conn(CON_OPEN, CON_DEVICE, bufptr, buflen, CONFLAGS);
   #else
      #if( SYS_TYPE == ARCH8086 )
         rc = os_conn(CON_OPEN, CON_DEVICE, bufptr, buflen, \
                                B9600|BITS8|PARITYN|STOP1);
      #endif
   #endif

   #D_return( rc )
}
