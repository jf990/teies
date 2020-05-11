/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    os_dtsub(datetime **date_in, datetime *date_to_sub)    |
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
  |                                      subtractions are to be done. |
  |            date_to_sub struct datetime IN Pointer to datetime     |
  |                                           structure which has to  |
  |                                           be subtracted.          |
  |                                                                   |
  |  :PURPOSE. To subtract a datetime structure from another one and  |
  |            pass the result back in the date_in datetime structure.|
  |            If date_in pointer is NIL then current date and time is|
  |            assumed, memory allocation done and pointer returned.  |
  |                                                                   |
  |  :OUTPUT.  returns 0 if sucessful.                                |
  |                   -1 if date_to_sub pointer is NIL.               |
  |                                                                   |
  |  :CALLS.   os_mema(), D_mdump(), D_show(), D_begin(), D_return()  |
  |            os_dttm().                                             |
  |                                                                   |
  |  :ERRORS.  -1 if date_to_sub datetime structure is NIL.           |
  |                                                                   |
  |  :LIMITS.  NONE Known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"

sint os_dtsub(date_in, date_to_sub)

datetime **date_in;
datetime *date_to_sub;

{
 datetime *tdate_in = 0;

 sint  fract = 0;
 sint  second = 0;
 sint  minute = 0;
 sint  hour = 0;
 sint  day = 0;
 sint  month = 0;
 sint  year = 0;

 sint  sec_borrow = 0;
 sint  min_borrow = 0;
 sint  hour_borrow = 0;
 sint  day_borrow = 0;

 sint  year_borrow = 0;
 byte  cent_borrow = 0;

 sint  leap = 0;
 sint  sub_day = 0;


 /* check if either input pointer is NIL */
 if (date_to_sub == NIL || date_in == NIL )
   {
   return(-1);
   }
 if ( *date_in == NIL)
   {
    tdate_in = (datetime *)os_mema(sizeof(datetime));
    os_dttm(tdate_in);
    *date_in = tdate_in;
   }

 if ((fract = (sint)((*date_in)->fract - (date_to_sub)->fract)) < 0 )
   {
    sec_borrow = 1;
    fract = (sint)((*date_in)->fract + 100 - (date_to_sub)->fract);
   }

 if ((second = (sint)((*date_in)->second - (date_to_sub)->second -
                                                     sec_borrow))  < 0 )
   {
    min_borrow = 1;
    second = (sint)((*date_in)->second + 60 - (date_to_sub)->second -
                                                            sec_borrow );
   }

 if ((minute = (sint)((*date_in)->minute - (date_to_sub)->minute -
                                                     min_borrow)) < 0 )
   {
    hour_borrow = 1;
    minute = (sint)((*date_in)->minute + 60 - (date_to_sub)->minute -
                                                            min_borrow );
   }

 if ((hour = (sint)((*date_in)->hour - (date_to_sub)->hour -
                                                     hour_borrow)) < 0 )
   {
    day_borrow = 1;
    hour = (sint)((*date_in)->hour + 24 - (date_to_sub)->hour -
                                                           hour_borrow );
   }

 if ((month = (sint)((*date_in)->month - (date_to_sub)->month)) < 0 )
   {
    year_borrow = 1;
    month = (sint)((*date_in)->month + 12 - (date_to_sub)->month);
   }
 if ((year = (sint)((*date_in)->year - (date_to_sub)->year -
                                                     year_borrow)) < 0 )
   {
    cent_borrow = 1;
    year = (sint)((*date_in)->year + 100 - (date_to_sub)->year -
                                                           year_borrow );
   }
 (*date_in)->century -=  ( (date_to_sub)->century- cent_borrow );

 if (month == 0)
    month = 1;
 if ( (year % 4) == 0 )
   {
    if ( (month == 4) || (month == 3) )
      {
       leap = 1;
      }
   }
 if ((day = (sint)((*date_in)->day - (date_to_sub)->day -
                                                     day_borrow)) <= 0 )
   {
    switch (month)

       {
        case 12:
        case 10:
        case 7:
        case 5:
                day = 61;
                sub_day = 31;
                break;
        case 11:
        case 8:
        case 6:
        case 1:
                day = 61;
                sub_day = 30;
                break;
        case 2:
        case 9:
                day = 62;
                sub_day = 31;
                break;
        case 3:
                day = 59 + leap;
                sub_day = 31;
                break;
        case 4:
                day = 59 + leap;
                sub_day = 28 + leap;
                break;
        default:
                break;
       }
    month -= 2;
    if (month < 0)
      {
       month += 12;
       year -= 1;
       if (year < 0)
         {
          year = 99;
          (*date_in)->century -= 1;
         }
      }
    day = day + (sint)((*date_in)->day - date_to_sub->day - day_borrow);

    if ((day = day - sub_day) > 0)
      {
       month += 1;
      }
   }

 (*date_in)->fract = (byte) fract;
 (*date_in)->second = (byte) second;
 (*date_in)->minute = (byte) minute;
 (*date_in)->hour = (byte) hour;
 (*date_in)->day = (byte) day;
 (*date_in)->month = (byte) month;
 (*date_in)->year =  (byte) year;

   return(0);
}
