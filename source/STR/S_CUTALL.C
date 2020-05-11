/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     S_CUTALL C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.   s_keepsw   boolean    IN   Leave size alone if set.    |*/
/*|                                                                   |*/
/*| :PARAM.    has        sint       IN   Current length of string.   |*/
/*|                                                                   |*/
/*| :PARAM.    need       sint       IN   Future length of string.    |*/
/*|                                                                   |*/
/*| :PURPOSE.  Calculates allocation reduction according to slot      |*/
/*|            specifications 1.42                                    |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns sint +number indicating new allocation size,   |*/
/*|            -1 if no change needed, or a negative STR error code.  |*/
/*|                                                                   |*/
/*| :CALLS.    s_ovrall()                                             |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   NOTE THAT MANY OTHER ROUTINES DEPEND ON THE LENGTH OF  |*/
/*|            A STRING BEIGN EVEN!                                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_May_87   K.J.Walsh   RC's and doc to 1.42 spec      |*/
/*|                                                                   |*/
/*| :MODS.     08_Jul_88   K.J.Walsh   Rewritten to have one return.  |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"

sint s_cutall (sint has, sint need )
{
   sint  rc =  0  ;                    /* Return code to caller        */

   if ( need == has )
   {
      rc = -1;
   }

   else if ( need > S_MAXLEN )
   {
      rc = S_BAD_SIZE;
   }

   else if ( s_keepsw == TRUE )
   {
      rc = -1;
   }

   else if ( need > has )
   {
      rc = s_ovrall(need);
   }

   else if ( need > 256 )
   {
      if ( ( has - need ) > 20 )
      {
         rc = need + 10;
      }
      else
      {
         rc = -1;
      }
   }

   else if ( need > 100 )
   {
      if ( has > 300 )
      {
         rc = 260;
      }
      else
      {
         rc = -1;
      }
   }


   else if ( need > 50 )
   {
      if ( has > 260 )
      {
         rc = 124;
      }
      else
      {
         rc = -1;
      }
   }

   else if ( need > 16 )
   {
      if ( has > 124 )
      {
         rc = 60;
      }
      else
      {
         rc = -1;
      }
   }

   else if ( need > 0 )
   {
      if ( has > 60 )
      {
         rc = 28;
      }
      else
      {
         rc = -1;
      }
   }

   else if ( need == 0 )
   {
      if ( has > 28 )
      {
         rc = 0;
      }
      else
      {
         rc = -1;
      }
   }

   return(rc);
}
