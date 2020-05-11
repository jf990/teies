/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_plex()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string   INPUT    string to be examined    |*/
/*|                                                                   |*/
/*|  :PURPOSE. To see if a given string is complex                    |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns TRUE or FALSE if given string is complex.      |*/
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
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"

sint s_plex (tstr)
string  tstr ;     /* TEIES string to test                             */
{

   if ( tstr == 0 )             /* Zero string considered simple       */
   return(FALSE);

   if (s_ckhibt(s_smp_al(tstr)))/*Test to see if the high bit is ONE  */
   return(TRUE);
   else
   return(FALSE);
}
