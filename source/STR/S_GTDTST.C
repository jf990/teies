/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_gtdtst()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.  *tstr        string   INPUT    target string            |*/
/*|  :PARAM.  *dt          datetime INPUT    pointer to date-time stru|*/
/*|  :PARAM.   fmt         byte     INPUT    desired format           |*/
/*|  :PARAM.   len         tint     INPUT    length desired           |*/
/*|                                                                   |*/
/*|  :PURPOSE. inserts the isv string version of the format into a    |*/
/*|            a string of length equal to the length of the date-    |*/
/*|            time structure                                         |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_sitos(), s_nulout()                                  |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.  Because LEN is a TINT, errenous results will occur if  |*/
/*|            passed a negative value.                               |*/
/*|                                                                   |*/
/*|  :MODS.    26_May_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                                                   |*/
/*| :MODS.     19_Sep_88   K.J.Walsh   Added calls to isv_numb();     |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"

sint s_gtdtst (tstr, dt, fmt, len)
string   *tstr   ;                /* Target string                     */
datetime *dt     ;                /* Pointer to date-time structure    */
byte      fmt    ;                /* String containing format of dt    */
tint      len    ;                /* Length of string requested        */
{
   sint   numout = 0 ;            /* Number from the datetime routine  */
   sint   rc     = 0 ;            /* Return code from string funs      */
   tint   count  = 0 ;            /* Counter for loop                  */

   s_nulout(tstr);
   switch (fmt)
   {
      case 'M':       /* Month of the year                 */
         numout = dt->month + ISV_JAN - 1;
         rc = isv_get (tstr, (isv) numout, len) ;
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
         numout = numout + ISV_MON - 1;      /* Cause 1 = MON */
         rc = isv_get (tstr, (isv) numout, len) ;
      break ;

      case 'T':       /* Time of the day (AM/PM)           */
         if (dt->hour >11)
            rc = isv_get (tstr, ISV_PM, len) ;
         else
            rc = isv_get (tstr, ISV_AM, len) ;
      break ;

      case 'D':       /* Day of the month                  */
      case 'C':       /* Century of the ages               */
      case 'Y':       /* Year of the century               */
      case 'H':       /* Hour of the day in 12hr format    */
      case 'I':       /* Hour of the day in 24hr format    */
      case 'N':       /* Minutes of the hour               */
      case 'S':       /* Seconds                           */
      case 'F':       /* Fractions of a second             */
      case 'Z':       /* Time zone                         */
         s_nulout (tstr) ;
         rc = S_BAD_PARM;
      break;

      default :
         s_nulout (tstr) ;
         rc = S_BAD_PARM;
      break ;


   }
   return(rc);
}
