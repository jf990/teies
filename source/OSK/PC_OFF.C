#include "defs.h"
#include "pcomm.h"
#include "osk.h"
#include "debug.h"

sint pc_off()
{
   sint rc = 0;
   hword junklen = 0;
   byte *junkdat = NULL;

   #D_begin(pc_off, pcomm)

   rc = os_conn(CON_OFF, CON_DEVICE, junkdat, &junklen, CONFLAGS);

   #D_return( rc )
}
