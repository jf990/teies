/*********************************************************************

 :NAME.     PRV_RSET.C

 :LEVEL.

 :AUTHOR.   PAMELA CHAM,SAMIR CHOPRA

 :DATE.     NOVEMBER 29, 1988

 :PARAM.    *PRV      pointer   INPUT

 :PARAM.    PRV_KEY   hword     INPUT

 :OUTPUT.

 :PURPOSE.  The following routine turns off the privilege bit(s)
            indicated by the privilege key in the privilege structure
            pointed to by the privilege pointer. If the bit is already
            off it is left off. Else a bit-wise AND operation is
            performed.

 *****************************************************************/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "debug.h"

prv_rset ( prv, prv_key)

PRIVS *prv;
hword prv_key;

{

   byte offset = 0;
   byte val = 0;
   byte *target = NIL;


   offset = prv_key / 256;
   val = prv_key - (offset * 256);
   target = ((byte *) prv) + offset;
   *target = *target & ~val;

   return(0);

}
