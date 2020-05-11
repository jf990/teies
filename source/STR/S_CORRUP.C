/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_corrup()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string   INPUT    the string to be checked |*/
/*|                                                                   |*/
/*|  :PURPOSE. Tests string to see if it might be corrupt.  So far,   |*/
/*|            the only indicatior of this is that the usage lenght   |*/
/*|            is greather than the allocated lenght, or the allocated|*/
/*|            length is zero.  This gives slightly better than a 50% |*/
/*|            chance of catching a bad pointer (which is what a      |*/
/*|            corrupt string is really expected to be).  Later the   |*/
/*|            memory context should be checked to verify that the    |*/
/*|            pointer is in range.                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns TRUE or FALSE based on checking.               |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    06_Aug_86   N.Voronka   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    29_Oct_86   K.J.Walsh   Added checking of A or U lens  |*/
/*|                                    less than zero.                |*/
/*|  :MODS.    25_Jan_88   K.J.Walsh   Obsoleted by os_cp.c & os_csp.c|*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"

sint s_corrupt (tstr)
string    tstr ;  /* TEIES string to test                             */
{
   string   nilptr   =  NIL;
   string   negptr   =  NIL;


   negptr--;

/*+-------------------------------------------------------------------+*/
/*| S_NUL pointer indicates an unallocated string, never corrupt      |*/
/*+-------------------------------------------------------------------+*/
   if ( tstr == S_NUL )
   return(FALSE);

/*+-------------------------------------------------------------------+*/
/*| On IBM, allocations are not in the less than x'FF' range (maybe)  |*/
/*| Zero or less is a definate no-no.                                 |*/
/*+-------------------------------------------------------------------+*/
   if ( tstr < (string) 0xFF )
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| We don't know where a good pointer should point, but maybe we know|*/
/*| where a bad pointer might point:  If it is the same as what is at |*/
/*| address NIL, then it is quite likely that we have a bad one.      |*/
/*+-------------------------------------------------------------------+*/
   if ( tstr == nilptr )
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| Under the same assumption as above, a -1 pointer might result from|*/
/*| an operation that returns a -1 as an error instead of an          |*/
/*| an allocation error.                                              |*/
/*+-------------------------------------------------------------------+*/
   if ( tstr == negptr )
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| This version of the system can handle only SIMPLE strings         |*/
/*+-------------------------------------------------------------------+*/
   if ( ! s_simp(tstr) )
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| Allocation lenght shows how much memory was allocated for the     |*/
/*| string.  It is not possible to allocate no memory or negative     |*/
/*| lenght segments of memory.                                        |*/
/*+-------------------------------------------------------------------+*/
   if (s_smp_al(tstr) < 1 )
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| The usage length can be zero but of course not less than zero.    |*/
/*+-------------------------------------------------------------------+*/
  if (s_smp_ul(tstr) <  0 )
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| If the usage length is greater than the usage length, then more   |*/
/*| more memory is being used than was allocated!                     |*/
/*+-------------------------------------------------------------------+*/
  if (s_smp_ul(tstr) > s_smp_al(tstr))
   return(TRUE);

/*+-------------------------------------------------------------------+*/
/*| Having passed all these tests, there is a chance that the string  |*/
/*| is not corrupt.                                                   |*/
/*+-------------------------------------------------------------------+*/
   return(FALSE);
}
