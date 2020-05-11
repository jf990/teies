/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_stoh()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   hw_out     *hword   OUTPUT   Output of conversion.     |*/
/*|  :PARAM.   str_in      string   INPUT    Contains the number.     |*/
/*|  :PARAM.   beg         tint     INPUT    Start of where to find it|*/
/*|  :PARAM.   len         tint     INPUT    How long to look for it  |*/
/*|                                                                   |*/
/*|  :PURPOSE. Converts S1 to a type hword returned as HWORD.         |*/
/*|            Does NOT check for corrupt strings.   All leading and  |*/
/*|            trailing non-numerics are considered white space.      |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Returns a standard STR error code or 0.                |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    17_Jan_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


sint   s_stoh(hw_out, str_in, beg, len)

hword    *hw_out  ;                    /* Results - a hword conversion */
string   str_in   ;                    /* Input - a hword-able string  */
sint     beg      ;                    /* Beginning position of hword  */
sint     len      ;                    /* Length of hword in str       */

{
   sint  count    =  0  ;              /* A counter                    */
   sint  power    =  1  ;              /* Powers of ten for conversion */
   hword result   =  0  ;              /* What we return               */
   sint  ulens    =  0  ;              /* Used length of source        */

   *hw_out = 0;

   if ( len < 0 )
   {
   return(S_BAD_LEN);
   }

   ulens = s_len (str_in) ;
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
   if ( len == 0 )
   {
   return(0);
   }

   count = beg + len;             /* We start at the end               */

   while ( NOT OS_ISNUMERIC( s_char_s(str_in,count-1) ) )
   {
      count--;                         /* Skip all non-numeric         */
      if ( count <= beg )              /* It was all non-numeric?      */
      {
   return(S_BAD_NUM);
      }
   }


   if ( count <= beg-1 )               /* It was all non-numeric?      */
   {
   return(S_BAD_NUM);
   }

   while ( OS_ISNUMERIC(s_char_s(str_in,count-1) ) )
   {
      result += power * (s_char_s(str_in,count-1) - '0' ) ;
      power *= 10;                     /* super - increment.           */
      count--;
      if (count <= beg)
      {
         *hw_out = result;            /* Update output                */
   return(0);
      }
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| If we get here we ended because we encountered an non-numeric.    |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   *hw_out = result;                  /* Update output                */
   return(0);
}
