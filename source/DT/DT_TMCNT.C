/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_TMCNT.C

:LEVEL.                TM

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
 *
 *       Given the pointer to a tdate structure, this function
 *       returns an integer of the total number of days till that
 *       date.
 *

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"


tint dt_tmcnt(dt)

date         *dt;

{

    tint years     = 0;        /* years count                    */
    tint months    = 0;        /* month count                    */
    tint days      = 0;        /* days conut initialized to zero */
    tint leap_flg  = 0;        /* for leap year check            */

#D_begin(  dt_tmcnt,  TM   )


    years = dt->century * 100 + dt->year;
    while ( years > 0 )
    {                              /* assumed that initial year is 1 */

         leap_flg = LEAP(years);
         if ( leap_flg )
         {

               days += 366;

         }
         else
         {

               days += 365;

         }
         years -= 1;

    }
    for ( months = dt->month; months != 0; months-- )
    {

         days += day_tab(:LEAP(years):)(:months:);/* days from months*/

    }
    #D_return ( 'days += dt->day' );       /* add the remaining days */
}
