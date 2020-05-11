/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_totos()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  IN/OUT   target string            |*/
/*|  :PARAM.   source      tint     INPUT    tint var to be converted |*/
/*|                                                                   |*/
/*|  :PURPOSE. To create a string of the given tint.                  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_init(), s_inttoc()                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    25_Aug_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    01_Jan_88   K.J.Walsh   Added testing, DCSS support    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


extern   sint  s_alloc  () ;

sint s_titos (target, source)
string *target ;
tint    source ;
{
   byte empty = 0  ;    /* Boolean indicating if 'out' is empty        */
   byte spos  = 0  ;    /* Indicates character position in 'out'       */
   byte npos  = 0  ;    /* Indicates position in source                */
   byte value = 0  ;    /* Numeric value for conversion                */
   tint ten   = 0  ;    /* Power of ten for division and modulus       */
   sint rc    = 0  ;    /* RC from things                              */


   rc = s_alloc(target,12);

   ten = 1000000000 ;
   empty = TRUE ;
   for (npos = 0 ; npos <= 9 ; npos++)
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
