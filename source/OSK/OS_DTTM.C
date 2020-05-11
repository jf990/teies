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

#define SYSTEM_CLOCK_ERROR 12345

sint     os_dttm( ptr )
datetime         *ptr;
{

tint     rc    =  0,
         tmp   =  0,
         totsecs =  0,
         microsecs =  0,
         secs  =  0,
         mins  =  0,
         years =  0,
         days  =  0,
         month =  0,
         leap_years = 0,
         leap_days  = 0;

struct   tmpstruct
{
   tint  word1; /* number of micro seconds since epoc */
   tint  word2; /* number of minutes since epoc */
}  tmpdttm;



if( (rc = Inter( OSTODCLK, &tmpdttm )) == 0 )
{


#if 0 /* old first, repl by Ken 28 June 1989 */
   totsecs = tmpdttm.word2 * 60 + tmpdttm.word1 % 60;
#else
                                  /* micro sec / 1 million */
   totsecs = tmpdttm.word2 * 60 + tmpdttm.word1 / 1000000;
#endif
   totsecs = totsecs - (60 * 60);  /* TWEAK! We're off hour for some   */
                                   /* strange reason                   */
#if 0 /* old first, repl by Ken 28 June 1989 */
   microsecs = tmpdttm.word1 / 60;
#else
   microsecs = tmpdttm.word1 % 1000000;
#endif


   secs = totsecs % (24 * 60 * 60 );
   days = totsecs / (24 * 60 * 60 );

#if 0 /* old first, repl by Ken 28 June 1989 */
   ptr->fract = microsecs % 255;
#else
                /* to map micro sec -> 256ths of a sec */
   ptr->fract = microsecs / (1000000 / 256);
#endif

   ptr->second = secs % 60;

   mins = secs / 60;

   ptr->minute = mins % 60;

   ptr->hour = mins / 60;


   days++;                       /* Add one more day since the number  */
                                 /* of days we got from OSTODCLK were  */
                                 /* starting from Jan 1, 1900(means    */
                                 /* that day was 0)                    */
   days -= 365;                  /* Tweak remove year 1900 special case*/

   leap_years = days / (365 * 4 + 1);
   leap_days  = days % (365 * 4 + 1);
   years = leap_days / 365;
   days  = leap_days % 365;

   if( years == 4 )
   {

      years = 3;
      days += 365;

   }

   years = years + leap_years * 4;
   years += 1;                    /* Untweak for year 1900             */

   ptr->century = (years / 100) + 19;
   ptr->year = years =  years % 100;

   if( ((years / 4) * 4) == years )
   {

      tmp = 1;

   }
   else
   {
      tmp = 0;
   }

   if( ! ((((years/400)*400) == years) || (((years/100)*100) != years)) )
   {

      tmp = 0;

   }

   if( days > (59 + tmp) )
   {

      days = days + 2 - tmp;

   }

   month = ( ( days + 91 ) * 100 ) / 3055;
   ptr->day = ( days + 91 ) - ( month * 3055) / 100;
   ptr->month = month - 2;

}
else
{

   rc = SYSTEM_CLOCK_ERROR;

}




   return(rc);
}
