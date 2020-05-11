/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_gtdtnm                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ucb->timezone byte INPUT diff off GMT for   |*/
/*|                                                user's timezone    |*/
/*|                                                                   |*/
/*|  :PARAM.  *tstr        string   IN/OUT   target string            |*/
/*|  :PARAM.  *dt          datetime INPUT    pointer to date-time strc|*/
/*|  :PARAM.   fmt         byte     INPUT    format                   |*/
/*|                                                                   |*/
/*|  :PURPOSE. inserts the numeric verions of the format into the     |*/
/*|            the target string extracted from the datetime structure|*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_sitos()                                              |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    15_May_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"


extern sint s_sitos () ;

sint s_gtdtnm (tstr, dt, fmt)
string   *tstr   ;                /* Target string                     */
datetime *dt     ;                /* Pointer to date-time structure    */
byte      fmt    ;                /* String containing format of dt    */
{
   sint   numout = 0;             /* Number from the datetime routine  */
   sint   rc     = 0;             /* Return code from string funs      */

   fmt = OS_TOUPPER(fmt);
   switch (fmt)
   {
   case 'D':       /* Day of the month                  */
       numout = dt->day ;
       break ;
   case 'M':       /* Month of the year                 */
      numout = dt->month ;
      break ;
   case 'C':       /* Century of the ages               */
      numout = dt->century ;
      break ;
   case 'Y':       /* Year of the century               */
      numout = dt->year ;
      break ;
   case 'H':       /* Hour of the day in 12hr format    */
      numout = dt->hour ;
      if (numout > 12)
         numout = numout - 12 ;
      break ;
   case 'I':       /* Hour of the day in 24hr format    */
      numout = dt->hour ;
      break ;
   case 'N':       /* Minutes of the hour               */
      numout = dt->minute ;
      break ;
   case 'S':       /* Seconds                           */
      numout = dt->second ;
      break ;
   case 'F':       /* Fractions of a second             */
      numout = dt->fract ;
      break ;
   case 'W':       /* Day of the week                   */
      numout = dt->century * 100 + dt->year ;
{
   if (dt->month < 3)
   {
      numout = 365 * numout + dt->day + 31 * ( dt->month - 1 ) + ( numout - 1 ) / 4
                - 3 * (((numout - 1) / 100) + 1) / 4 ;
   }
   else
   {
      numout = 365 * numout + dt->day + 31 * (dt->month - 1) - ( 4 * dt->month + 23 ) / 10
               + ( numout ) / 4 - 3 * (((numout ) / 100) + 1) / 4 ;
   }
   numout = (numout + 5) % 7 + 1;
}
      break ;
   case 'Z':       /* Time zone                         */
      numout = TM_master->ucb->time_zone ;
      break ;
   case 'T':       /* Time of the day (AM/PM)           */
      if (dt->hour >11)
         numout = 1 ;   /* Post Meridium --- P.M.       */
      else
         numout = 0 ;   /* Ante Meridium --- A.M.       */
      break ;
   default :
      rc = S_BAD_PARM;
      s_nulout(tstr);
      numout = 0 ;
      break ;
   }

   if ( rc == 0 )
   {
      rc = s_sitos (tstr, numout) ;
   }

   return(rc);
}
