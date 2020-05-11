/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     S_OVRALL C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.   s_keepsw   boolean    IN   Set exact if set TRUE.      |*/
/*|                                                                   |*/
/*| :PARAM.    before     sint       IN   Required minimum length.    |*/
/*|                                                                   |*/
/*| :PURPOSE.  Computes overallocation for string allocations.        |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns a sint denoting length to allocate or a        |*/
/*|            standard error RC.                                     |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.   S_BAD_PARAM                                            |*/
/*|                                                                   |*/
/*| :LIMITS.   MANY ROUTINES DEPEND ON THE ALLOCATION LENGTH BEING    |*/
/*|            EVEN!                                                  |*/
/*|                                                                   |*/
/*| :MODS.     06_Aug_87   N.Voronka   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_87   K.J.Walsh   Altered structure and updated  |*/
/*|                                    slot scheme to spec 1.42       |*/
/*|                                                                   |*/
/*| :MODS.     23_May_87   K.J.Walsh   Standardized documentation and |*/
/*|                                    return codes.                  |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"

sint s_ovrall ( sint before )
{

   sint  rc =  0  ;                    /* New length or error code     */


   if ( before > S_MAXLEN )
   {
      rc = S_BAD_SIZE;
   }
   else if ( before < 0 )
   {
      rc = S_BAD_SIZE;
   }
   else if ( s_keepsw == TRUE )
   {
      rc = before;
   }
   else if ( before > 256 ) rc = before+10+(before % 2);
   else if ( before > 100 ) rc =       260 ;          /* Slot 4        */
   else if ( before >  50 ) rc =       124 ;          /* Slot 3        */
   else if ( before >  16 ) rc =        60 ;          /* Slot 2        */
   else if ( before >   0 ) rc =        28 ;          /* Slot 1        */
   else                     rc =         0 ;          /* Slot 0        */

   return(rc);
}
