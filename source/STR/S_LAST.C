/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_last                                                 |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   needle      string   INPUT    the needle to be found   |*/
/*|  :PARAM.   haystack    string   INPUT    the haystack to look in  |*/
/*|  :PARAM.   beg         sint     INPUT    where to start looking   |*/
/*|  :PARAM.   len         sint     INPUT    how far to look          |*/
/*|                                                                   |*/
/*|  :PURPOSE. finds the last occurance of a string within the        |*/
/*|            designated portion of another string                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC or a postive     |*/
/*|            sint indicating where the needle was found in the      |*/
/*|            haystack                                               |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    16_Mar_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"


sint s_last (needle , haystack, beg, len)
string needle  ;                  /* Contains pattern to be found      */
string haystack;                  /* Contains haystack to be searched  */
sint   beg     ;                  /* Beginning of section to search    */
sint   len     ;                  /* Length of section to be searched  */
{
   sint count  =  0  ;            /* Counter for the haystack          */
   sint countn =  0  ;            /* Counter for the needle            */
   sint lenh   =  0  ;            /* Used length of haystack           */
   sint lenn   =  0  ;            /* Used length of needle             */
   sint match  =  0  ;            /* Number of chars properly matched  */
   byte chr_h  =  0  ;            /* Char from  haystack to be matched */
   byte chr_n  =  0  ;            /* Char from needle  to be matched   */




   if (len < 0)                   /* Check for negative length         */
   {
   return(S_BAD_LEN);
   }

   lenh = s_len (haystack) ;        /* Get used length of haystack         */
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > lenh )       /* also OK if LEN is too long   */
      {
         len = lenh;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = lenh;
      }
      if ( beg > lenh )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > lenh )        /* range bleeds right           */
      {
         len = lenh - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = lenh + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = lenh;
         }
         if ( beg > lenh )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > lenh )     /* range bleeds right           */
         {
            len = lenh - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = lenh;
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
   if (len == 0)
   {
   return(0);
   }

   lenn = s_len (needle );
   if (lenn == 0)
   {
   return(0);
   }

   count  = beg + len;            /* Start at rear--last character + 1 */
   match  = 0 ;                   /* Number of matching chars so far   */
   countn = lenn ;                /* Initialize needle  counter        */

   do
   {
      count  = count  - 1 ;       /* Look at preeceding char in haystak*/
      countn = countn - 1 ;       /* Look at preeceding char in needle */
      chr_h = s_char_s (haystack , count ) ;
      chr_n = s_char_s (needle , countn ) ;
      if (chr_h == chr_n)     /* Check if haystack matches needle    */
         match = match + 1 ;
      else
      {
         match  = 0    ;
         countn = lenn ;
      }
      if (match == lenn)          /* If match found compute beginning  */
      {
         count++;
   return(count);
      }
   }
   while (count > beg );

   return(0);
}
