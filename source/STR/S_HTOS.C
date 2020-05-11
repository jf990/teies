/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_htos()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  IN/OUT   string to be updated     |*/
/*|  :PARAM.   source      hword    INPUT    value to set to          |*/
/*|                                                                   |*/
/*|  :PURPOSE. creates a string from the given Hword                  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_init(), s_inttoc()                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    25_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|  :MODS.    01_Jan_88   K.J.Walsh   Added testing, DCSS support    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


extern s_alloc  () ;

sint s_htos (target, source)
string *target ;
hword   source ;
{
   byte  empty = 0 ;    /* Boolean indicating if 'out' is empty        */
   byte  spos  = 0 ;    /* Indicates character position in 'out'       */
   byte  npos  = 0 ;    /* Indicates position in source                */
   byte  value = 0 ;    /* Numeric value for conversion                */
   hword ten   = 0 ;    /* Power of ten for division and modulus       */
   sint  rc    = 0 ;    /* Rcode from stuff                            */


   rc = s_alloc(target,10);


   ten = 10000 ;
   empty = TRUE ;
   for (npos = 0 ; npos <= 4 ; npos++)
   {
      value  = source / ten ;
      source = source % ten ;
      ten = ten / 10 ;
      if (empty)
      {
         if (value != 0)
         {
            s_char_s(*target,spos++) = OS_V2CHAR (value) ;
            empty = FALSE ;
         }
      }
      else
      {
         s_char_s(*target,spos++) = OS_V2CHAR (value) ;
      }
   }
   if (spos == 0)
   {
      s_char_s(*target,spos++) = '0' ;
   }
   s_smp_ul(*target) = spos;
   return(0);
}
