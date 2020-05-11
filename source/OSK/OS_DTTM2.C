/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_DTTM.C

:LEVEL.                OSK

:AUTHOR.               Siva Raparla

:DATE.                 April 16, 1987

:INPUT.            dt   : Pointer to a structure instance of datetime

:OUTPUT.           rc   : always 0

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.          Read the system hardware clock and return
                   date and time.

:CALLS.            OSTODCLK

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"



sint     os_dttm( ptr )
datetime         *ptr;
{

sint     rc  =  0;
struct   tmpstruct
{
   sint  microsecs;
   sint  numofdays;
}  tmpdttm;
sint  tmp, tmp2;


#D_start( os_dttm, OSK,'Ptr = 0x%x','ptr'
#D_dump( 'ptr','sizeof(datetime)',' What is already there' );

if( (rc = Inter( OSTODCLK, tmpdttm )) == 0 )
{

   #D_show('# of days since epoc = 0x%x','tmpdttm.numofdays')
   #D_show('# of micro secs since midnight = 0x%x','tmpdttm.microsecs')

   ptr->fract = tmp = tmpdttm.microsecs % 10000;
   #D_show('Fractions = 0x%x(%d)','tmp,tmp')

   tmp = tmpdttm.microsecs / 10000;
   #D_show('Seconds since midnight = 0x%x(%d)','tmp,tmp')

   ptr->second = tmp % 60;
   #D_show('Seconds calculated = %d','ptr->second')

   tmp = tmp / 60;
   #D_show('Minutes since midnight = 0x%x(%d)','tmp,tmp')

   ptr->minute = tmp % 60;
   #D_show('Minutes calculated = 0x%x(%d)','ptr->minute')

   tmp = tmp / 60;
   #D_show('Hours since midnight = 0x%x(%d)','tmp,tmp')

   ptr->hours = tmp;


   tmpdttm.numofdays++;          /* Add one more day since the number  */
                                 /* of days we got from OSTODCLK were  */
                                 /* starting from Jan 1, 1900(means    */
                                 /* that day was 0)                    */
   tmpdttm.numofdays -= 365;     /* Tweak remove year 1900 special case*/

   leap_years = tmpdttm.numofdays / (365 * 4 + 1);
   leap_days  = tmpdttm.numofdays % (365 * 4 + 1);
   numofdays  =















#D_mdump( 'ptr', 'sizeof(datetime)' );
#D_return( rc );
}
