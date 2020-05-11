/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_stoti()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tintout     *tint    OUTPUT   Output of conversion.    |*/
/*|  :PARAM.   str_in      string   INPUT    Contains the number.     |*/
/*|  :PARAM.   beg         tint     INPUT    Start of where to find it|*/
/*|  :PARAM.   len         tint     INPUT    How long to look for it  |*/
/*|                                                                   |*/
/*|  :PURPOSE. Converts S1 to a type tint returned as TINT.           |*/
/*|            Does NOT check for corrupt stringer.  All leading and  |*/
/*|            trailing non-numerics are considered white space.      |*/
/*|            Only "white space" delimited number is returned.       |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Returns sint in accordance with standard STR error rc's|*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    05_May_87   K.J.Walsh   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    27_May_87   N.Voronka   Added beg/len paramaters       |*/
/*|  :MODS.    01_Jun_87   K.J.Walsh   Brought back to 1.42 doc stds. |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"

extern   sint  s_plex   () ;

sint  s_stoti(tintout, str_in, beg, len)

tint     *tintout ;                    /* Results - a tint conversion  */
string   str_in   ;                    /* Input - a tint-able string   */
tint     beg      ;                    /* Beginning position of tint   */
tint     len      ;                    /* Length of tint in str        */

{
   sint  count    =  0  ;              /* A counter                    */
   tint  power    =  1  ;              /* Powers of ten for conversion */
   tint  result   =  0  ;              /* What we return               */
   tint  ulens          ;              /* Used length of source        */


   ulens = s_len (str_in) ;
   if (beg == 0)
      beg = 1 ;
   if (beg > ulens)
      beg = ulens ;
   if (len == 0)
      len = ulens ;
   if (beg+len-1 > ulens)
      len = ulens - beg + 1 ;

   count = beg + len - 1;         /* We start at the end               */

   while (s_char_s(str_in,count-1)<'0' || s_char_s(str_in,count-1)>'9')
   {
      count--;                       /* Skip all non-numeric         */
      if ( count <= beg-1 )            /* It was all non-numeric?      */
      {
         *tintout = 0;
   return(S_BAD_NUM);
      }
   }


   if ( count <= beg-1 )               /* It was all non-numeric?      */
   {
   return(S_BAD_NUM);
   }

   while (s_char_s(str_in,count-1) >= '0'
          && s_char_s(str_in,count-1) <= '9' )
   {
      result += power * (s_char_s(str_in,count-1) - '0' ) ;
      power *= 10;                     /* super - increment.           */
      count--;
      if (count < beg-1)
      {
         *tintout = result;            /* Update output                */
   return(0);
      }
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| If we get here we ended because we encountered an non-numeric.    |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

  *tintout = result;                   /* Update output                */
   return(0);
}
