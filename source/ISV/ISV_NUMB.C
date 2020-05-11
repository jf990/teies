/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     ISV_NUMB H                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    ISV                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    *result    string     OUT  Outgoing result string      |*/
/*| :PARAM.    number     sint       IN   Number to convert           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Creates a string consisting of the text version of the |*/
/*|            input number.                                          |*/
/*|                                                                   |*/
/*|            Format of numbers in accordance with "Publication      |*/
/*|            Manual of the American Psychological Association",     |*/
/*|            c 1983 by American Psychological Association,          |*/
/*|            Banta Compay/George Banta Company, Inc., Menasha,      |*/
/*|            Wisconsin.                                             |*/
/*|                                                                   |*/
/*| :OUTPUT.   0 or RC of failing element                             |*/
/*|                                                                   |*/
/*| :CALLS.    s_get, s_titos                                         |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   Not necessarily internationally portable (for instance,|*/
/*|            in the Danish language numbers between nineteen and    |*/
/*|            one hundred exclusive are expressed as "ones and tens",|*/
/*|            such as 5 and 20).                                     |*/
/*|                                                                   |*/
/*|            This is a dummy implementation and returns only the    |*/
/*|            string version of the number (in numberals, not        |*/
/*|            text).                                                 |*/
/*|                                                                   |*/
/*| :MODS.     19_Sep_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv.h"
#include "isv_i.h"
#include "osk.h"
#include "debug.h"

sint isv_numb(result,number)

string  *result;           /* pointer to be set                        */
sint    number ;           /* number to convert                        */

{
   string   work_isv =  S_NULL   ;     /* Temporary holder for ISV's   */
   string   work     =  S_NULL   ;     /* Temporary string holder      */
   sint     rc       =  0        ;     /* Our rcode                    */

   #D_start('isv_numb','isv','ptr=0x%X; number=%d','result,number');
   #s_pcheck('result');
   #D_str('*result','target string');

   s_nulout(result);

   rc = s_sitos(result,number);

   #D_str('*result','at end');
   #D_return(rc);

}
