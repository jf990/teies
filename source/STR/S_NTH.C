/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_nth()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   needle      string   INPUT    indicates pattern to find|*/
/*|  :PARAM.   haystack    string   INPUT    string inwhich to find it|*/
/*|  :PARAM.   beg         sint     INPUT    starting point in haystak|*/
/*|  :PARAM.   len         sint     INPUT    length from starting pt. |*/
/*|  :PARAM.   N           sint     INPUT    Which occurance is valid |*/
/*|                                                                   |*/
/*|  :PURPOSE. To find the nth   occurence of one string in a defined |*/
/*|            segment of another.                                    |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a sint indicating the Nth occurane of the 2nd  |*/
/*|            string in the first.                                   |*/
/*|                                                                   |*/
/*|  :CALLS.   s_len()                                                |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    15_Jan_88   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"



sint s_nth (needle , haystack, beg, len, N)
string needle  ;                  /* Contains pattern to be found      */
string haystack  ;                  /* Contains source to be searched  */
sint   beg     ;                  /* Beginning of section to search    */
sint   len     ;                  /* Length of section to be searched  */
sint   N       ;                  /* Indicates which one is to be found*/
{
   sint count  =  0  ;            /* Counter for the haystack          */
   sint countn =  0  ;            /* Counter for the needle            */
   sint lenh   =  0  ;            /* Used length of haystack           */
   sint lenn   =  0  ;            /* Used length of needle             */
   sint match  =  0  ;            /* Number of chars properly matched  */
   byte chr_h  =  0  ;            /* Char from  haystack to be matched */
   byte chr_n  =  0  ;            /* Char from needle  to be matched   */
   sint hits   =  0  ;            /* How close to finding N we are     */
   sint hit_at =  0  ;            /* Where a particular match is       */




   if (len < 0)                   /* Check for negative length         */
   {
   return(S_BAD_LEN);
   }
   lenh = s_len (haystack) ;        /* Get used length of source       */
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

   lenn = s_len (needle );        /* Get used length of haystack       */
   if (lenn == 0)
   {
   return(0);
   }

   hits = 0;

/*+-------------------------------------------------------------------+*/
/*| First of three cases:  find Nth from front...                     |*/
/*+-------------------------------------------------------------------+*/
   if ( N > 0 )                        /* Find from front              */
   {
      count  = beg;               /* Start at beginning                */
      match  = 0 ;                /* Number of matching chars so far   */
      countn = 0 ;                /* Initialize needle  counter        */

      do
      {
         chr_h = s_char_s (haystack , count ) ;
         chr_n = s_char_s (needle , countn ) ;
         count  = count  + 1 ; /* Look at next char in haystak      */
         countn = countn + 1 ; /* Look at next char in needle       */
         if (chr_n == chr_h)   /* Check if haystack matches needle  */
            match = match + 1 ;
         else
         {
            match  = 0 ;
            countn = 0 ;
         }
         if (match == lenn)
         {
            if ( ++hits == N )
            {
   return(count - lenn + 1);
            }
            match  = 0 ;
            countn = 0 ;
         }
      }
      while (count < beg+len );
   }
/*+-------------------------------------------------------------------+*/
/*| Second of three cases:   find Nth from rear...                    |*/
/*+-------------------------------------------------------------------+*/
   else if ( N < 0 )                   /* Find from rear               */
   {
      count  = beg + len;         /* Start at rear--last character + 1 */
      match  = 0 ;                /* Number of matching chars so far   */
      countn = lenn ;             /* Initialize needle  counter        */

      do
      {
         count  = count  - 1 ;    /* Look at preeceding char in haystak*/
         countn = countn - 1 ;    /* Look at preeceding char in needle */
         chr_h = s_char_s (haystack , count ) ;
         chr_n = s_char_s (needle , countn ) ;
         if (chr_h == chr_n)     /* Check if haystack matches needle    */
            match = match + 1 ;
         else
         {
            match  = 0    ;
            countn = lenn ;
         }
         if (match == lenn)
         {
            if ( --hits == N )
            {
               count++;
   return(count);
            }
            match  = 0    ;
            countn = lenn ;
         }
      }
      while (count > beg );
   }

/*+-------------------------------------------------------------------+*/
/*| Third of three cases:  find zero-th (ie, user out to lunch)       |*/
/*+-------------------------------------------------------------------+*/
   else                                /* N == 0?                      */
   {
   return(0);
   }
   return(0);
}
