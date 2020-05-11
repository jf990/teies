/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_getlow()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   n1          tint     INPUT    a candidate for lowest   |*/
/*|  :PARAM.   n2          tint     INPUT    a candidate for lowest   |*/
/*|  :PARAM.   n3          tint     INPUT    a candidate for lowest   |*/
/*|                                                                   |*/
/*|  :PURPOSE. Figures out which of three non-zero numbers is lowest  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns the lowest of the three vacations              |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Inaccurate results if sent a sint--must be passed tints|*/
/*|                                                                   |*/
/*|  :MODS.    12_Dec_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    put in calls to #s_check()     |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    21_Jan_88   K.J.Walsh   Rendered unneeded after        |*/
/*|                                    alteration of s_type()         |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

sint s_getlow (n1, n2, n3)
tint n1, n2, n3   ;               /* The three numbers to be compared  */
{
   tint lowest ;

   #D_start (s_getlow, str, 'n1=%d, n2=%d, n3=%d', 'n1,n2,n3')
   if (((n1 < n2    ) && (n1 > 0)) || (n2     == 0))
      lowest = n1 ;
   else
      lowest = n2 ;
   if (((n3 < lowest) && (n3 > 0)) || (lowest == 0))
      lowest = n3 ;
   #D_return (lowest)
}
