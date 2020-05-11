/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_DDTE.C

:LEVEL.                TM

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

                   Returns the number of centuries, years, months, and
                   days difference between date1 and date2.
                   Where: date1 >= date1.

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"


date       *dt_ddte( p,  n )

date                *p, *n;

{

    date    *r = NIL;     /* resulting structure pointer */
    tint years =   0;


#D_begin(  dt_ddte,  TM   );


    r = ( date * ) os_mema( sizeof( date ) );
    years = p->century * 100 + p->year;           /* total years */

 /* days difference */

    if ( ( r->day = p->day - n->day ) < 0 )
    {                                             /* if result < 0 */

         p->day += day_tab(:LEAP(years):)(:p->month:);
         if((  p->month == 2 )          &&
            ( years % 4 == 0 && years % 100 != 0 || years % 400 == 0 ))
         {

               p->day += 1;

         }
         p->month -= 1;
         r->day = p->day - n->day;
    }

 /* month difference */

    if(( r->month = ( p->month - n->month )) < 0 )
    {

         p->year -= 1;
         p->month += 12;
         r->month = p->month - n->month;

    }

 /* year difference */

    if ( ( r->year = p->year - n->year ) < 0 )
    {
         p->century -= 1;
         p->year += 100;
         r->year = p->year - n->year;
    }

 /* century difference */

    r->century = p->century - n->century;

#D_return( r );
}
