/***************************************************************

 : NAME.          PRV_TEST.C
 : LEVEL.
 : AUTHOR.        PAMELA CHAM, SAMIR CHOPRA
 : DATE           NOVEMBER 29, 1988

 : PARAM.         *PRV     pointer     INPUT

 : PARAM.         PRV_KEY  hword       INPUT

 : OUTPUT.        RC       boolean     TRUE

                  RC       boolean     FALSE

 : PURPOSE.       routine returns TRUE if all bits in the mask
                  PRV_KEY are set on in the privilege structure
                  pointed to by the privilege pointer *PRV. If any
                  bits indicated in the PRV_KEY are not set on, return
                  FALSE.

************************************************************************/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "debug.h"

boolean prv_test (prv, prv_key)

   PRIVS *prv;
   hword prv_key;

{

   byte offset=0;
   byte val=0;
   byte and_val=0;
   byte *target = NIL;
   hword divisor = 256;
   boolean rc = TRUE;


   offset = prv_key/ divisor;
   val = prv_key - (offset * divisor);


   target = ((byte *) prv) + offset;
   and_val = *target & (byte) val;


   if (val != and_val)

   {

      rc = FALSE;

   }

   return(rc);

}
