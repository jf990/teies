/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_char_p                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string   INPUT    origional string to fetch|*/
/*|                                          from                     |*/
/*|  :PARAM.   c           sint     INPUT    indicates desired char   |*/
/*|                                                                   |*/
/*|  :PURPOSE. retrieves the c'th character from the complex string   |*/
/*|            tstr                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a byte;                                        |*/
/*|                                                                   |*/
/*|  :CALLS.   s_len()                                                |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    17_Mar_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42;             |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

s_char_p (tstr, c)
string   tstr     ;               /* Pointer to complex TEIES string   */
sint     c        ;               /* Position of character to obtain   */
{
   tint   totpart ;               /* Number of parts in the cmplx str  */
   tint   curpart ;               /* Current part in loop              */
   string curptr  ;               /* Currnt pointer to simple string   */
   tint   curpos  ;               /* Current position in the loop      */
   tint   curlen  ;               /* Length of current simple string   */
   byte   chr = 0 ;               /* Character to return               */

   #D_start (s_char_p, str, 'str_ptr=0x%X  c=%d','tstr, c') ;

   totpart = s_smp_ul (tstr) / (S_PTRLEN + 1) ;
   curpart = 0 ;
   curpos = 0 ;
   do
   {
      curpart = curpart + 1 ;               /* Increment counter       */
      curptr = s_ptr (tstr, curpart-1) ;    /* Fetch pointer to part   */
      curlen = s_len (curptr) ;             /* Get length of component */
      curpos = curpos + curlen ;
      if (c <= curpos-1)
         chr = s_char_s (curptr, c-(curpos-curlen)) ;
   }
   while (curpart < totpart && curpos < c+1) ;
   #D_return (chr) ;
}
