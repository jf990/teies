/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_sitos()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  IN/OUT   string to be changed     |*/
/*|  :PARAM.   source      sint     INPUT    value to convert to str  |*/
/*|                                                                   |*/
/*|  :PURPOSE. Creates a string from the given sint.                  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_init(), s_inttoc(), s_alloc()                        |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Not in test bed.                                       |*/
/*|                                                                   |*/
/*|  :MODS.    25_Aug_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42;             |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


extern   sint  s_alloc  () ;
extern   sint  s_init   () ;

sint s_sitos (target, source)
string *target ;
sint    source ;
{
   byte empty = 0  ;    /* Boolean indicating if 'out' is empty        */
   byte spos  = 0  ;    /* Indicates character position in 'out'       */
   byte npos  = 0  ;    /* Indicates position in source                */
   byte value = 0  ;    /* Numeric value for conversion                */
   sint ten   = 0  ;    /* Power of ten for division and modulus       */
   sint rc    = 0  ;    /* Rcode from things                           */


   rc = s_alloc(target,12);

   ten = 1000000000 ;
   empty = TRUE ;
   if (source < 0)
   {
      source = -1 * source ;
      s_char_s(*target,spos++) = '-' ;
   }
   for (npos = 0 ; npos <= 9 ; npos++)
   {
      value  = source / ten ;
      source = source % ten ;
#if 0
#endif
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


