/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_DDIF.C

:LEVEL.                TM

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.
                   date1:   A structure of type date.
                   date2:   A structure of type date.

:OUTPUT.           rc   :   A structure of type date (see OSI.H).

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

                Returns the number of centuries, years, months, and days
                difference between date1 and date2 throuth date3.
                where: date1 >= date1

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"







sint   dt_ddif( p,  n,  r )

date           *p, *n, *r;


{
    tint years    = 0,
         leap_flg = 0;


#D_begin(   dt_ddif,  TM    );

    years = p->century * 100 + p->year;

                                                  /* days difference */
    if ( p->day < n->day )
    {                                               /* if result < 0 */

         leap_flg = LEAP(years);
         p->day += day_tab(:leap_flg:)(: p->month :);
         p->month -= 1;

    }
    r->day = p->day - n->day;

                                                 /* month difference */
    if ( p->month < n->month )
    {

         p->year -= 1;
         p->month += 12;

    }
    r->month = p->month - n->month;

                                                  /* year difference */

    if ( p->year < n->year )
    {

         p->century -= 1;
         p->year += 100;

    }
    r->year = p->year - n->year;

                                               /* century difference */

    r->century = p->century - n->century;

    #D_return( r );
}
