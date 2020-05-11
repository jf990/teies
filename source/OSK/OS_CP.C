/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     os_cp.c                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    osk                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    pointer     byte*    INPUT    the pointer to be checked|*/
/*|                                                                   |*/
/*| :PURPOSE.  Tests to see if a pointer might be bad.                |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns TRUE if the pointer is determined to be bad.   |*/
/*|            Returns FALSE if the pointer is determined to be OK.   |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   Actually guesses if a pointer is bad, based on it's    |*/
/*|            value as opposed to if there is a record of it being   |*/
/*|            in the range of allocated memory.                      |*/
/*|                                                                   |*/
/*|            VERY SIMILAR TO OS_CP():  MAKE SIMILAR MODIFICATIONS.  |*/
/*|                                                                   |*/
/*| :MODS.     25_Jan_88   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*| :MODS.     18_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"

bool os_cp (byte *pointer)
{

   bool  rc ;                          /* NOT initialized to save time */


   /*+----------------------------------------------------------------+*/
   /*| Null pointers are not permitted.                               |*/
   /*+----------------------------------------------------------------+*/
   if ( pointer == NIL )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| On IBM, allocations are not in the less than x'FF' range.      |*/
   /*| (Except if Null, which we have already checked for).           |*/
   /*+----------------------------------------------------------------+*/
   else if ( (byte *) pointer < (byte *) 0xFF )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| Watch for the -1 pointer, actually an error return.            |*/
   /*+----------------------------------------------------------------+*/
   else if ( pointer == (byte *) MINUS_ONE_PTR )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| Conversely, address greater than 0xFFFFFF are not possible--   |*/
   /*| can't be addressed by IBM 360/370.                             |*/
   /*+----------------------------------------------------------------+*/
   else if ( (byte *) pointer > (byte *) 0xFFFFFF )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| There does not seem to be anything wrong this this pointer.    |*/
   /*+----------------------------------------------------------------+*/
   else
   {
      rc = FALSE;
   }


   return(rc);
}
