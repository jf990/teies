/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_ctos                                                 |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  IN/OUT   target string            |*/
/*|  :PARAM.   source      char     INPUT    desired contents         |*/
/*|                                                                   |*/
/*|  :PURPOSE. creates a string consisting of the given character     |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_init()                                               |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    20_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    08_Jan_88   K.J.Walsh   Revised for use in DCSS        |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

extern sint s_alloc () ;

sint s_ctos (target, source)
string *target ;                  /* Target - where to copy to         */
char    source ;                  /* Source - where to copy from       */
{
   sint rc = 0     ;              /* Return code from things           */


   rc = s_alloc(target,1);             /* Get space                    */

   s_char_s(*target,0) = (byte)source; /* Stick it in                  */
   s_smp_ul(*target) = 1;              /* And note that 1 byte used    */

   return(rc);
}
