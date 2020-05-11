/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     os_csp.c                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    osk                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    pointer     string*  INPUT    the pointer to be checked|*/
/*|                                                                   |*/
/*| :PURPOSE.  Tests string to see a string or it's pointer might be  |*/
/*|            corrupt.                                               |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns TRUE or FALSE based on checking.               |*/
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
#include "str_i.h"
#include "debug.h"

bool os_csp (string str_ptr)
{

   bool  rc ;                          /* NOT initialized to save time */


   /*+----------------------------------------------------------------+*/
   /*| Null string pointers are never corrupt.                        |*/
   /*+----------------------------------------------------------------+*/
   if ( str_ptr == S_NULL )
   {
      rc = FALSE;
   }

   /*+----------------------------------------------------------------+*/
   /*| On IBM, allocations are not in the less than x'FF' range.      |*/
   /*| (Except if Null, which we have already checked for).           |*/
   /*+----------------------------------------------------------------+*/
   else if ( (byte *) str_ptr < (byte *) 0xFF )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| Conversely, address greater than 0xFFFFFF are not possible--   |*/
   /*| can't be addressed by IBM 360/370.                             |*/
   /*+----------------------------------------------------------------+*/
   else if ( (byte *) str_ptr > (byte *) 0xFFFFFF )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| The high bit in the allocation size field is not supposed to   |*/
   /*| be set (reserved for "complex" strings).                       |*/
   /*+----------------------------------------------------------------+*/
   else if ( s_smp_al(str_ptr) > 32768 )
   {
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| If the usage length is greater than the usage length, then more|*/
   /*| more memory is being used than was allocated!                  |*/
   /*+----------------------------------------------------------------+*/
   else if (s_smp_ul(str_ptr) > s_smp_al(str_ptr))
   {
      if ( ( s_smp_al(str_ptr) == 0 ) && ( s_smp_ul(str_ptr) == 0xDED ) )
      {
      }
      else
      {
      }
      rc = TRUE;
   }

   /*+----------------------------------------------------------------+*/
   /*| There does not seem to be anything wrong this this string ptr. |*/
   /*+----------------------------------------------------------------+*/
   else
   {
      rc = FALSE;
   }


   return(rc);
}
