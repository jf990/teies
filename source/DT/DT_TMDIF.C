/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 dt_TMDIF.C

:LEVEL.                TM

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

 *  Given two pointers of type dttm, this routine compares the data  *
 *  and sets the return code as follows:                             *
 *       0     if data1 = data2                                      *
 *       1     if data1 > data2                                      *
 *       2     if data1 < data2                                      *

:CALLS.

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"





sint dt_tmdif(  dttm1,  dttm2 )

datetime       *dttm1, *dttm2;


{

    byte *a = NIL,
         *b = NIL;
    sint  i =   0;
    sint  rc= DT_EQUAL;



    a = (byte *) dttm1;
    b = (byte *) dttm2;

    while ( i++ < 8 )
    {

         if ( *a > *b )
         {

              rc = DT_GREATER_THAN; /*  dttm1 > dttm2 */
              break;

         }
         else if ( *a < *b )
         {

              rc = DT_LESS_THAN;   /* dttm1 < dttm2 */
              break;

         }
         a++;
         b++;
    }
   return(rc);
}
