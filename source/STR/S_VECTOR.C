/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_vector()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   master      string   INPUT    complex string to be done|*/
/*|  :PARAM.   element     tint     INPUT    element of string to be  |*/
/*|                                          indicated                |*/
/*|                                                                   |*/
/*|  :PURPOSE. Accesses a complex string as if it were an array,      |*/
/*|            returning addresses to pointers.  The result can be    |*/
/*|            used just like a simple string.                        |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns an address to a string pointer                 |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Does not support complex strings with embedded simple  |*/
/*|            components.                                            |*/
/*|                                                                   |*/
/*|  :MODS.    16_Jun_87   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"

string s_vector(master,element)
string *master ;                  /* Existing complex string           */
tint   element ;                  /* Which simple string to extract    */
{
   string   address  ;                 /* address to be returned       */
   sint     has      ;                 /* # of elements in vector      */
   sint     canhave  ;                 /* allocation available         */

   #D_start(s_vector,str,'*master=0x%X, element=%d','*master,element');

   if ( element <= 0 )
   {
      #D_leave(S_BAD_PARM,'element # not greater than zero');
   }

   if ( master == S_NULL )
   {
      has = 0;
      canhave = 0;
   }
   else if ( ! s_ckhibt(master))
   {
      #D_leave(S_BAD_STR,'master string is simple');
   }
   else
   {
      has = (s_smp_ul(master)) - S_HI_ON ;
      canhave = (s_smp_al(master)) - S_HI_ON ;
   }

   #D_show('input str has %d elements and can fit %d\n','has,canhave');

   #D_return(address);
}
