/********************************************************************

  :NAME.      PRV_CMP.C

  :LEVEL.

  :AUTHOR.   PAMELA CHAM,SAMIR CHOPRA

  :DATE.     NOVEMBER 29, 1988

  :PARAM.     *PRV1     pointer   INPUT
              *PRV2     pointer   INPUT

  :OUTPUT.    RC        boolean   TRUE

              RC        boolean   FALSE

  :PURPOSE    The following routines checks the equality (in bytewise
              fashion) of the privilege structures pointed to by
              *PRV1 and *PRV2.  Two temporary pointers *T_PRV1 and
              *T_PRV2 are assigned the addresses of *PRV1 and *PRV2
              respectively.  These pointers are then incremented to
              move from one byte to the next as byte equality is checked.
              If structures are equal, return TRUE, else return FALSE.


******************************************************************/

#include "defs.h"
#include "osk.h"
#include "osk_i.h"
#include "str.h"
#include "obj.h"
#include "debug.h"

boolean prv_cmp (prv1, prv2)

PRIVS *prv1, *prv2;

{

   byte  *t_prv1=NIL, *t_prv2=NIL;
   int i=0;
   boolean rc=TRUE ;


   t_prv1 = (byte*) prv1;
   t_prv2 = (byte*) prv2;
   for (i= 1; i<= sizeof(PRIVS); i++)

   {
      if (*t_prv1++ != *t_prv2++)

      {

        rc=FALSE;
        break;

      }

   }

   return(rc);

}
