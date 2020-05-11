/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_UNLOAD C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_htable(::) D_HTABLE IN/OUT  Hash switch table to be  |*/
/*|                                          dropped.                 |*/
/*|                                                                   |*/
/*| :PARAM.    id         *byte      IN      Identification of caller.|*/
/*|                                                                   |*/
/*| :PURPOSE.  Deallocates all elements of the hash table.            |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.    D_DEALLOCATE                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     06_Jun_88   A.K.Srivastava Implemented as "D_flush"    |*/
/*| :MODS.     19_Jul_88   K.J.Walsh      Changed into D_unload       |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include "debug_i.h"
void D_unload(id)
byte *id;
{

   struct    D_htentry   *temp1 = NULL;
   struct    D_htentry   *temp2 = NULL;
   int       count = 0;

   /*+----------------------------------------------------------------+*/
   /*| If asked for, dump the hash table.  Note that if the           |*/
   /*| compiled-in debugging is on, further switches are checked to   |*/
   /*| see if the dump should be produced.                            |*/
   /*+----------------------------------------------------------------+*/
   #if (NOT PROD_SYS)
      if
      ( D_lookup("D9HASHDUMP",id) || D_SWITCH(D5DEBUG) == TRUE )
      {
         D_hdump(id);
      }
   #endif

   /*+----------------------------------------------------------------+*/
   /*| Now strip the elements of the table and deallocate them.       |*/
   /*+----------------------------------------------------------------+*/
   for ( count = 0; count < 256; count++ )
   {
      if ( D_htable(:count:).first != NULL )
      {
         temp1 = D_htable(:count:).first ;
            while (temp1 != NULL )
            {
               temp2 = temp1;
               temp1 = temp1->next;
               D_DEALLOCATE( temp2 );
            }
      }
   }

   /*+----------------------------------------------------------------+*/
   /*| Chuck away the hash table base and the translate table.        |*/
   /*+----------------------------------------------------------------+*/
   D_DEALLOCATE(D_htable);
   D_htable = (int *) NULL;

   /*+----------------------------------------------------------------+*/
   /*| If warranted, print the stats.                                 |*/
   /*+----------------------------------------------------------------+*/
   #if (NOT PROD_SYS)
      if ( D_SWITCH(D9PSTAT1) || D_SWITCH(D2UNLOAD) || D_SWITCH(D5DEBUG))
        D_pr("!!!!:%s:D_unload(A):  queries=%d, nexts=%d, max next=%d\n",
         id,D_STATS(D_s_qrys),D_STATS(D_s_nxts),D_STATS(D_s_mnxt));
   #endif

   D_INT_SW(D_ok_use) = FALSE;                /* D_start needed now    */
}
