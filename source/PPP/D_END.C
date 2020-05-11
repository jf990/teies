/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_END C                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   All debugging level globals                            |*/
/*|                                                                   |*/
/*| :PARAM.    id         *byte      IN   Identification of caller    |*/
/*|                                                                   |*/
/*| :PURPOSE.  Deallocates all structures created by calling D_init.  |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.    D_DEALLOCATE                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     19_Jul_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include "debug_i.h"

void D_end(id)
byte *id;
{

   D_unload(id);                             /* Get rid of D_htable    */

   /*+----------------------------------------------------------------+*/
   /*| If apparently not called before, do deallocations.             |*/
   /*+----------------------------------------------------------------+*/
   if ( D_SWITCH(D_ok_run) == TRUE )          /* Not set?              */
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2END) || D_SWITCH(D5DEBUG))
            D_pr("!!!!:%s:D_end(A):  Deallocating tables and buffers\n",
               id);
      #endif

      /*+-------------------------------------------------------------+*/
      /*| Deallocate the misc segements                               |*/
      /*+-------------------------------------------------------------+*/
      D_DEALLOCATE(D_cstr   );
      D_DEALLOCATE(D_buffer );
      /*+-------------------------------------------------------------+*/
      /*| Deallocate hash value lookup table.                         |*/
      /*+-------------------------------------------------------------+*/
      D_DEALLOCATE(D_htrans);

      /*+-------------------------------------------------------------+*/
      /*| Last and hardly least turn off indicators.                  |*/
      /*+-------------------------------------------------------------+*/
      D_INT_SW(D_ok_run)  = FALSE;
      D_INT_SW(D_ok_use)    = FALSE;
   }
   else
   {
      #if (NOT PROD_SYS)
         if (D_SWITCH(D2END) || D_SWITCH(D5DEBUG))
           D_pr("!!!!:%s:D_end(B):  Already ended or not started \
request ignored.\n",id);
      #endif
   }
   /*+----------------------------------------------------------------+*/
   /*| Last and hardly least turn off indicators.                     |*/
   /*+----------------------------------------------------------------+*/
   D_INT_SW(D_ok_run)  = FALSE;
   D_INT_SW(D_ok_use)    = FALSE;
}
