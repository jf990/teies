/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_OUTPUT C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   name       type       IN   comment                     |*/
/*|                                                                   |*/
/*| :PARAM.    flag       sint       IN   Currently unused flag.      |*/
/*|                                                                   |*/
/*| :PURPOSE.  Ends output for debugging.                             |*/
/*|                                                                   |*/
/*| :OUTPUT.   RC of file open()                                      |*/
/*|                                                                   |*/
/*| :CALLS.    open(), lookup()                                       |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     25_Jul_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <string.h>
#include "debug_i.h"

void D_outend(flag,id)
   sint  flag;
   byte  *id;
{
   if ( D_io != NULL )
   {
      D_pr("----:%s:D_output(A):  Closing output file.\n",id);
      fclose(D_io);
   }
   else
   {
      #if ( NOT PROD_SYS )
         if ( D_SWITCH(D2OUTEND) || D_SWITCH(D5DEBUG) )
            D_pr("!!!!:%s:D_output(B): \
               No output file to close (D_io=%d).\n",id,D_io);
      #endif
   }
}
