/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    os_dtadd(datetime **date_in, datetime *date_to_add)    |
  |                                                                   |
  |  :AUTHOR.  Tanmay S Kumar                                         |
  |                                                                   |
  |  :DATE.    August 10, 1988                                        |
  |                                                                   |
  |  :LEVEL.   OSK                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   **date_in struct datetime IN/OUT pointer to pointer to |
  |                                      datetime structure to which  |
  |                                      additions are to be done.    |
  |            date_to_add struct datetime IN Pointer to datetime     |
  |                                           structure which has to  |
  |                                           be added.               |
  |                                                                   |
  |  :PURPOSE. To add      a datetime structure from another one and  |
  |            pass the result back in the date_in datetime structure.|
  |            If date_in pointer is NIL then current date and time is|
  |            assumed, memory allocation done and pointer returned.  |
  |                                                                   |
  |  :OUTPUT.  returns 0 if sucessful.                                |
  |                   -1 if date_to_add pointer is NIL.               |
  |                                                                   |
  |  :CALLS.   os_mema(), D_mdump(), D_show(), D_begin(), D_return()  |
  |            os_dttm().                                             |
  |                                                                   |
  |  :ERRORS.  -1 if date_to_add datetime structure is NIL.           |
  |                                                                   |
  |  :LIMITS.  NONE Known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"

sint os_dtadd(date_in, date_to_add)

datetime **date_in;
datetime *date_to_add;

{
 datetime *tdate_in = 0;
 byte  flag = 0;
 byte  over_fract = 0;
 byte  fract = 0;
 hword rem_day = 0;
 byte  over_mon = 0;
 byte  rem_mon = 0;
 byte  over_hour = 0;
 sint  rem_hour = 0;
 hword rem_min = 0;
 byte  over_year = 0;
 byte  year = 0;
 byte  day = 0;
 byte  month = 0;
 byte  hour = 0;
 byte  minute = 0;
 byte second = 0;
 sint temp = 0;

 if (date_to_add == NIL || date_in == NIL )
   {
   return(-1);
   }
 if (*date_in == NIL)
   {
    tdate_in = (datetime *)os_mema(sizeof(datetime));
    os_dttm(tdate_in);
    *date_in = tdate_in;
   }

 /* check for special case when the time is 00:00:00.00 */
 if (((*date_in)->fract == 0) && ((*date_in)->second == 0) &&
     ((*date_in)->minute == 0) && ((*date_in)->hour == 0))
   {
    /* if yes then subtract a day and make hours 24 */
    (*date_in)->day -= 1;
    (*date_in)->hour = 24;
   }
 /*add the 2 second fractions and get the minute overflow and remainder*/
 over_fract = (((*date_in)->fract)+ date_to_add->fract) / 100;
 /* fract has the final fract figure */
 fract = (((*date_in)->fract) + date_to_add->fract) % 100;

 temp  = (sint)((*date_in)->second + date_to_add->second + over_fract);
 temp += (sint)(((*date_in)->minute + date_to_add->minute) * 60);
 /* temp has the total # of sec's without the fraction */
 temp += (sint)(((*date_in)->hour + date_to_add->hour) * 3600);
 over_hour = (byte)(temp / (3600 * 24));
 /* rem_hour has the number of seconds remaining in the day */
 rem_hour = (sint)(temp % (3600 * 24));

 /* add the 2 month amounts and get the month overflow and remainder */
 /* -1 to adjust for current month */
 over_mon = ((*date_in)->month - 1 + date_to_add->month) / 12;
 rem_mon = ((*date_in)->month + date_to_add->month) % 12;

 /* over_year has the number of centuries */
 /* -1 to adjust for current year */
 over_year = ((*date_in)->year + date_to_add->year + over_mon -1) / 100;
 /* year has the final year */
 year = ((*date_in)->year + date_to_add->year + over_mon) %100;
 switch (rem_mon)
       {
        case 0: rem_day = 334;
                break;
        case 1: rem_day = 0;
                break;
        case 2: rem_day = 31;
                break;
        case 3: rem_day = 59;
                break;
        case 4: rem_day = 90;
                break;
        case 5: rem_day = 120;
                break;
        case 6: rem_day = 151;
                break;
        case 7: rem_day = 181;
                break;
        case 8: rem_day = 212;
                break;
        case 9: rem_day = 243;
                break;
        case 10: rem_day = 273;
                 break;
        case 11: rem_day = 304;
                 break;
        default: break;
       }
 /* if it is a leap year and month is > 2 add an extra day */
 if ((((*date_in)->year + date_to_add->year + over_mon) % 4) == 0)
   {
    if ((rem_mon > 2) || (rem_mon == 0))
       rem_day += 1;
   }
 rem_day += (*date_in)->day + date_to_add->day + over_hour;

 /* hour has the final hours */
 hour = (byte)(rem_hour / 3600);
 rem_min = (hword)(rem_hour % 3600);
 /*minute has the final minutes */
 minute = (byte)(rem_min / 60);
 /* second  has the final seconds */
 second = rem_min % 60;

 if ((year % 4)== 0)
    flag = 1;
 if ((rem_day >= 0) && (rem_day <= 31))
   {
    month = 1;
    day = (byte)rem_day;
   }

 else if (rem_day <= 59 + flag)
   {
    month = 2;
    day = (byte)(rem_day - 31);
   }

 else if (rem_day <= 90 + flag)
   {
    month = 3;
    day = (byte)(rem_day - 59 - flag);
   }

 else if (rem_day <= 120 + flag)
   {
    month = 4;
    day = (byte)(rem_day - 90 - flag);
   }

 else if (rem_day <= 151 + flag)
   {
    month = 5;
    day = (byte)(rem_day - 120 - flag);
   }

 else if (rem_day <= 181 + flag)
   {
    month = 6;
    day = (byte)(rem_day - 151 - flag);
   }

 else if (rem_day <= 212 + flag)
   {
    month = 7;
    day = (byte)(rem_day - 181 - flag);
   }

 else if (rem_day <= 243 + flag)
   {
    month = 8;
    day = (byte)(rem_day - 212 - flag);
   }

 else if (rem_day <= 273 + flag)
   {
    month = 9;
    day = (byte)(rem_day - 243 - flag);
   }

 else if (rem_day <= 304 + flag)
   {
    month = 10;
    day = (byte)(rem_day - 273 - flag);
   }

 else if (rem_day <= 334 + flag)
   {
    month = 11;
    day = (byte)(rem_day - 304 - flag);
   }

 else
   {
    month = 12;
    day = (byte)(rem_day - 334 - flag);
   }
 if ((month == 12) && (day > 31))
   {  /* this condition happens when we adjust for special case */
    year += 1;
    month = 1;
    day -= 31;
   }
 (*date_in)->fract = fract;
 (*date_in)->second = second;
 (*date_in)->minute = minute;
 (*date_in)->hour = hour;
 (*date_in)->day = day;
 (*date_in)->month = month;
 (*date_in)->year = year;
 (*date_in)->century =
                  (*date_in)->century + date_to_add->century + over_year;
   return(0);
}
