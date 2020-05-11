/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_stosi()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   sintout     *sint    OUTPUT   Output of conversion.    |*/
/*|  :PARAM.   str_in      string   INPUT    Contains the number.     |*/
/*|  :PARAM.   beg         tint     INPUT    Start of where to find it|*/
/*|  :PARAM.   len         tint     INPUT    How long to look for it  |*/
/*|                                                                   |*/
/*|  :PURPOSE. Converts S1 to a type sint returned as SINT.           |*/
/*|            Does NOT check for corrupt strings.   All leading and  |*/
/*|            trailing non-numerics are considered white space.      |*/
/*|            Only "white space" delimited number is returned.       |*/
/*|            If a "-" is found immediately before the first digit   |*/
/*|            (no white space), then the number is negative.         |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Returns sint in accordance with standard STR error rc's|*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Documented to 1.42 standards   |*/
/*|  :MODS.    11_Aug_87   K.J.Walsh   Implementation completed       |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "str.h"
#include "osk.h"
#include "debug.h"

sint  s_stosi(sintout, str_in, beg, len)

sint     *sintout ;                    /* Results - a sint conversion  */
string   str_in   ;                    /* Input - a sint-able string   */
sint     beg      ;                    /* Beginning position of sint   */
sint     len      ;                    /* Length of sint in str        */

{
   sint  count    =  0  ;              /* A counter                    */
   sint  power    =  1  ;              /* Powers of ten for conversion */
   sint  result   =  0  ;              /* What we return               */
   sint  ulens    =  0  ;              /* Used length of source        */
   sint  rcode    =  0  ;              /* Return code from function    */

   *sintout = 0;


   ulens = s_len (str_in);
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > ulens )       /* also OK if LEN is too long   */
      {
         len = ulens;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = ulens;
      }
      if ( beg > ulens )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > ulens )        /* range bleeds right           */
      {
         len = ulens - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = ulens + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = ulens;
         }
         if ( beg > ulens )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > ulens )     /* range bleeds right           */
         {
            len = ulens - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = ulens;
      }
      else if ( beg + len < 0 )          /* Completely out of range      */
      {
         len = 0;
      }
      else
      {                                /* Range bleeds left            */
         len = len + beg;                 /* NB -- adding a negative      */
         beg = 0;
      }
   }
}


   if ( len > 0 )                      /* If anything in range...      */
   {
      count = beg + len - 1;           /* We start at the end          */

      while ( NOT OS_ISNUM(s_char_s(str_in,count)) )
      {
         count--;                      /* Skip all non-numeric         */
         if ( beg > count )            /* It was all non-numeric?      */
         {
   return(S_BAD_NUM);
         }
      }


      while (OS_ISNUM(s_char_s(str_in,count)))
      {
         result += power * ( OS_C2VAL(s_char_s(str_in,count)) );
         power *= 10;                  /* super - increment.           */
         count--;
         if (count <  beg)
         {
            *sintout = result;         /* Update output                */
   return(0);
         }
      }

   /*+----------------------------------------------------------------+*/
   /*|                                                                |*/
   /*| If we get here we ended because we encountered an non-numeric. |*/
   /*| The non-numeric might be a '-', indicating a negative number.  |*/
   /*|                                                                |*/
   /*+----------------------------------------------------------------+*/

      if (s_char_s(str_in,count) == '-' )
      {
         *sintout = - result;          /* Update output as NEGATIVE    */
         rcode = 0;
      }

      else

      {
         *sintout = result;            /* Update output                */
         rcode = 0;
      }
    }
   /*+----------------------------------------------------------------+*/
   /*| (end of "if" from near top)                                    |*/
   /*+----------------------------------------------------------------+*/
   else
   {
      rcode = S_BAD_NUM;
   }

   return(rcode);
}
