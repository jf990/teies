/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_rtos()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  OUTPUT   Target to be changed     |*/
/*|  :PARAM.   source      real     INPUT    What to convert          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Converts a given real type to string                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_init();                                              |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Scaffold version of routine!                           |*/
/*|                                                                   |*/
/*|  :MODS.    25_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|  :MODS.    11_Dec_87   K.J.Walsh   Removed local vars for use to  |*/
/*|                                    facilitate DCSS usage.         |*/
/*|  :MODS.    01_Jan_88   K.J.Walsh   Added testing, DCSS support    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "str_i.h"
#include "osk.h"


#define  MAX_LENGTH 260                /* Maximum size                 */

extern            sprintf  () ;

sint s_rtos (target, source)
string *target ;
real    source ;
{
   sint     rc    = 0      ;           /* Rcodes from stuff           */
   int      size  = 0      ;           /* RC from sprintf() = size    */


   rc = s_alloc(target,MAX_LENGTH);
   s_char_s(*target,0) = '\0';

   size = (sint) sprintf (s_buffer(*target), "%f", source) ;

   if ( size < 0 )
   {
      s_char_s(*target,0) = '0';
      s_smp_ul(*target) = 2;
   return(size-16000);
   }
   else if ( size > MAX_LENGTH )
   {
      s_char_s(*target,1) = '0';
      s_smp_ul(*target) = 2;
   return(S_BAD_CONV);
      /* In the event that the above occurs, increase MAX_LEN */
   }

   s_smp_ul(*target) = size;

   return(0);
}
