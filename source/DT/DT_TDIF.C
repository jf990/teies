/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_TDIF.C      (was  tm_diff()  )

:LEVEL.                TM

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
 *                                                               *
 *   Given three pointers of ttime, time1, time2, & time3, this      *
 *   function puts the difference in the times, time1 & time2, in    *
 *   the memory pointed by  time3.                                   *
 *   The return codes are:                                           *
 *       0 : if all right                                            *
 *       1 : if fraction of seconds data mismatch                    *
 *       2 : if seconds data mismatch                                *
 *       3 : if minutes data mismatch                                *
 *       4 : if hours data mismatch                                  *
 *                                                                   *

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"


sint dt_tdif(time1, time2, time3)
time        *time1,*time2,*time3;

{

tint   ret_code,   flag;


#D_begin( dt_tdif,  TM  )
    /* input data check */

    if ( (time1->hour > 23) || (time2->hour > 23) )
         return ( 4 );   /*invalid data of hour */
    else if ( (time1->hour < 0) || (time2->hour < 0) )
         return ( 4 );
    if ( (time1->minute > 59) || (time2->minute > 59) )
         return ( 3 );   /* invalid data of minutes */
    else if ( (time1->minute < 0) || (time2->minute < 0) )
         return ( 3 ) ;
    if ( (time1->second > 59) || (time2->second > 59) )
         return ( 2 );   /* invalid data of seconds */
    else if ( (time1->second < 0) || (time2->second < 0) )
         return ( 2 ) ;
    if ( (time1->fract > 99) || (time2->fract > 99) )
         return ( 1 );   /* invalid data of seconds */
    else if ( (time1->fract < 0) || (time2->fract < 0) )
         return ( 1 ) ;

    flag = os_cmpdt( time1, time2 );
    if ( flag  == 1 )
         ret_code = ( os_cmpt(time1, time2, time3) );
    else if ( flag == 2 )
         ret_code = ( os_cmpt( time2, time1, time3 ) );
    else {
         time3->hour   = 0;
         time3->minute = 0;
         time3->second = 0;
         time3->fract  = 0;
         ret_code      = 0;
    }
#D_return ( ret_code );
}
