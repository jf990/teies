/******************************************************************

 :NAME.      PRVSET.C

 :LEVEL.     PRIV

 :AUTHOR.    PAMELA CHAM,SAMIR CHOPRA

 :DATE.      NOVEMBER 29, 1988

 :PARAM.     *PRV     pointer   INPUT

 :PARAM.     PRV_KEY  hword     INPUT

 :OUTPUT

 :PURPOSE.   The following routine turns the privilege bit(s)
             indicated by the privilege key in the privilege structure
             pointed to by the privilege pointer. Any bits that are
             already set on are left on. A bit-wise OR operation is
             carried out on the privilege byte indicated in the mask.

******************************************************************/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "debug.h"

sint  prv_set ( PRIVS *prv, hword prv_key)

{

   byte offset=0;                      /* Offset from start of struct  */
   byte val =0;                        /* Which bit to set at offset   */
   byte *target = NIL;                 /* Points to byte to be altered */

   #D_start(prv_set,priv,'*prv @ 0x%X, prv_key = 0x%X','prv,prv_key')

   offset = prv_key / 256;
   val = prv_key - (offset * 256);

   #D_show('offset = 0x%X, val = 0x%X','offset,val');

   target = ((byte *) prv) + offset;
   *target = *target | val;

   #D_show('target @ 0x%X, *target = 0x%X','target,*target');
   #D_return(0);

}
