/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_allen()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string   INPUT    string to be "measured"  |*/
/*|                                                                   |*/
/*|  :PURPOSE. computes the allocation length of a given string       |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC (which are neg), |*/
/*|            or a positive number indicating the allocation length  |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    18_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    16_Jun_87   K.J.Walsh   "Mothballed"--tells user it    |*/
/*|                                    should not have been called    |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    put in calls to #s_check()     |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"

#undef s_allen                         /* (now a PPC macro)            */

sint s_allen (tstr)
string   tstr ;           /* Tstring whose alloc length is to be found */
{
   tint  alsize ;               /* Amount allocated for string         */

   #D_start ( s_allen , str , 'ptr=0x%X' , tstr )

/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING FOLLOWS                                               |*/
/*+-------------------------------------------------------------------+*/

   D_pr("####:WARNING:  OLD VERSION OF s_len() IN USE.\n");
   #D_leave('(tstr == (sint) (S_NUL ? 0 : s_smp_al(tstr)))', \
      'OLD VERSION CALLED');

/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING END                                                   |*/
/*+-------------------------------------------------------------------+*/
   if ( tstr == S_NULL )        /* Nothing allocated at all            */
      #D_return(0)

   if (s_simp(tstr))            /* IF simple string                    */
   {
      alsize = s_smp_al(tstr) ;
      #D_return (alsize)
   }
   else                         /* else it is a complex string         */
   {
      #cmplx () ;

   }
}
