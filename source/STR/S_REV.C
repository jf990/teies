/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_rev()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  IN/OUT   string to be updated     |*/
/*|  :PARAM.   source      string   INPUT    string to get from       |*/
/*|  :PARAM.   beg         sint     INPUT    where to start from      |*/
/*|  :PARAM.   len         sint     INPUT    how much to reverse      |*/
/*|                                                                   |*/
/*|  :PURPOSE. makes or changes the target string to the reversed     |*/
/*|            image of the indicated portion of the source string    |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_alloc(), s_copy()                                    |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    16_Mar_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    18_Jan_88   K.J.Walsh   Slight bug in call to s_copy() |*/
/*|                                    fixed.                         |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "str.h"
#include "osk.h"
#include "debug.h"

sint s_rev(string *out, string in, sint beg, sint len)
{
   sint     count    =  0     ;
   sint     count2   =  0     ;
   sint     u_len    =  0     ;
   sint     rcode    =  0     ;
   string   work     =  S_NULL;




/*+-------------------------------------------------------------------+*/
/*| Check length / calculate intended beg and end                     |*/
/*+-------------------------------------------------------------------+*/

   if ( len < 0 )
   {
      rcode = S_BAD_LEN;
       s_nulout(out);
   }
   else
   {
      u_len = s_len(in);
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > u_len )       /* also OK if LEN is too long   */
      {
         len = u_len;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = u_len;
      }
      if ( beg > u_len )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > u_len )        /* range bleeds right           */
      {
         len = u_len - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = u_len + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = u_len;
         }
         if ( beg > u_len )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > u_len )     /* range bleeds right           */
         {
            len = u_len - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = u_len;
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
         s_nulout(out);
      }
      else
      {
         s_alloc(&work,len);
         s_smp_ul(work) = len;
         count2 = len - 1 + beg;
         for ( count = 0 ; count < len ; count++)
         {
            s_char_s(work,count) = s_char_s(in,count2--);
         }

         if ( in != *out )
         {
            s_drop(out);
         }

         *out = work;
      }
   }
   return(rcode);
}
