/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_CMPD.C

:LEVEL.                OSI

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

 *   dt_diff():                                                  *
 *   Given three pionters, date1, date2, date3, of tdate structures, *
 *   the following function puts    the difference in dates of date1,*
 *   and date2 in memory pointed by time3.                           *
 *   The return codes are:                                           *
 *       0 : if all right                                            *
 *       1 : if date data mismatch                                   *
 *       2 : if month data mismatch                                  *

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"



sint    dt_cmpd(  date1,   date2,   date3  )

date             *date1,  *date2,  *date3;

{

    tint ret_code  = 0,
         flag      = 0;

    #D_begin(  dt_cmpd,  TM  )

                                                   /* input data check */

    if ( (date1->month > 12) || (date2->month > 12) )
    {

         #D_return ( 2 );                      /*invalid data of month */

    }
    else if ( (date1->month < 1) || (date2->month < 1) )
    {

         #D_return ( 2 );

    }
    if ( (date1->day > 31) || (date2->day > 31) )
    {

         #D_return ( 1 );                       /* invalid data of day */

    }
    else if ( (date1->day < 1) || (date2->day < 1) )
    {

         #D_return ( 1 ) ;

    }
    flag = os_cmpdt( date1, date2 );
    if ( flag  == 1 )
    {

         ret_code = ( os_ddif(date1, date2, date3) );

    }
    else if ( flag == 2 )
    {

         ret_code = ( os_ddif( date2, date1, date3 ) );

    }
    else
    {
         date3->century = 0;
         date3->year    = 0;
         date3->month   = 0;
         date3->day     = 0;
         ret_code       = 0;
    }
    #D_return ( ret_code );     /* all right */
}
