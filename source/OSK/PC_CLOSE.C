#include "defs.h"
#include "pcomm.h"
#include "osk.h"
#include "debug.h"

sint pc_close()
{
   sint rc = 0;
   byte *junkdat = NULL;
   hword junklen = 0;

   #D_begin(pc_close, pcomm )

   rc = os_conn(CON_CLOSE, CON_DEVICE, junkdat, &junklen, CONFLAGS);

   #D_return( rc )
}
