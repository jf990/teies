/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_CMPT.C       ( was tdiff()  )

:LEVEL.                TM

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

 *                                                                   *
 *   Given two pointers of ttime structures, the following           *
 *   function returns a pointer to another ttime structure           *
 *   with the difference in time:HHMMSSFF                            *
 *   Where: time1 >= time2.                                          *

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"


sint   dt_cmpt(  p,   n,   r   )
time            *p,
                     *n,
                          *r;


{


tint     psec = 0,
         nsec = 0;


#D_begin(  dt_cmpt,  TM  )


                                     /* convert HHMMSS into seconds   */
    psec = ( p->hour * 60 + p->minute ) * 60 + p->second;
    nsec = ( n->hour * 60 + n->minute ) * 60 + n->second;

                                     /* difference of the fraction    */
    if ( p->fract < n->fract ) {
         psec = psec - 1;            /* decrement a second            */
         p->fract += 100;            /* add 100 fractions of seconds  */
    };
    r->fract = p->fract - n->fract;  /* resulting fraction            */
    psec    -= nsec;                 /* remaining seconds             */

                                     /* convert psec back into HHMMSS */
    r->hour   = psec / 3600;
    r->minute = ( psec % 3600 ) / 60;
    r->second = ( psec % 3600 ) % 60;



#D_return( 0 );

}
