/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_btos()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  INPUT    target string            |*/
/*|  :PARAM.   source      byte     INPUT    character to do it with  |*/
/*|                                                                   |*/
/*|  :PURPOSE. to create a string consisting of the numeric value of  |*/
/*|            the given byte.                                        |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_init(), s_intoc()                                    |*/
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


extern sint s_alloc  () ;

sint s_btos (target, source)
string *target ;
byte    source ;
{
   byte empty = 0  ;    /* Boolean indicating if 'out' is empty        */
   byte spos  = 0  ;    /* Indicates character position in 'out'       */
   byte npos  = 0  ;    /* Indicates position in source                */
   byte value = 0  ;    /* Numeric value for conversion                */
   byte ten   = 0  ;    /* Power of ten for division and modulus       */
   sint rc    = 0  ;    /* Return Code from things                     */


   rc = s_alloc(target,4);

   ten = 100 ;
   empty = TRUE ;
   for (npos = 0 ; npos <= 2 ; npos++)
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
      s_char_s(*target,spos++) = '0';
   }
   s_smp_ul(*target) = spos;
   return(0);
}
