/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_len()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string   INPUT    string to be measured    |*/
/*|                                                                   |*/
/*|  :PURPOSE. To compute tyhe used length of the given string.       |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a sint denoting the length of the string.      |*/
/*|                                                                   |*/
/*|  :CALLS.   #s_check(),                                            |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    07_Aug_86   N.Voronka   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    to #s_check() instead of  calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    01_Jun_87   K.J.Walsh   Removed #s_check()--not approp.|*/
/*|  :MODS.    16_Jun_87   K.J.Walsh   "Mothballed"--tells user it    |*/
/*|                                    should not have been called.   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"
#undef s_len
sint s_len  (tstr)
string  tstr ;            /* Tstring whose used length is to be found  */
{
   sint usize ;                 /* Amount used in string               */

   #D_start ( s_len , str , 'ptr=0x%X' , tstr )
   #D_str(tstr,'checking');

/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING FOLLOWS                                               |*/
/*+-------------------------------------------------------------------+*/

   D_pr("####:WARNING:  OLD VERSION OF s_len() IN USE.\n");
   #D_return('(tstr == S_NUL ? 0 : s_smp_ul(tstr))');

/*+-------------------------------------------------------------------+*/
/*| MOTHBALLING END                                                   |*/
/*+-------------------------------------------------------------------+*/


   if (tstr == S_NULL)
      #D_leave(0,'String was NULL')

   if (s_simp(tstr))            /* IF simple string                    */
   {
      usize = s_smp_ul(tstr) ;
      #D_return (usize)
   }
   else                         /* else it is a complex string         */
   {

      #cmplx () ;
      #D_return(0) ;
   }
}
