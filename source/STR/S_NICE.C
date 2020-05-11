/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     s_nice()                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    str                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    str        string     IN/OUT   string to be converted  |*/
/*| :PARAM.    beg        sint       INPUT    point at which to begin |*/
/*| :PARAM.    len        sint       INPUT    length of region to conv|*/
/*|                                                                   |*/
/*| :PURPOSE.  Capitalizes the first character of every word  & makes |*/
/*|            the rest of the word in lower case.                    |*/
/*|                                                                   |*/
/*|            A word is delimited by any non-alphabetic character    |*/
/*|                                                                   |*/
/*| :OUTPUT.   returns zero or standard STR RC to indicate status     |*/
/*|                                                                   |*/
/*| :CALLS.    s_type,s_upper                                         |*/
/*|                                                                   |*/
/*| :ERRORS.   S_BAD_LEN                                              |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*| :MODS.     28_Jun_88   Sangeeta    Written.                       |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"
#include "str.h"

sint s_nice (str,beg,len)
string  *str;
sint    beg,len;
{
    sint count=0,                     /* position in the string */
         strlen=0,                    /* used length of the string */
         end=0;                       /* end position in the string */

    strlen=s_len(*str);
    if (len < 0)                      /* check for negative len */
   return(S_BAD_LEN);
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > strlen )       /* also OK if LEN is too long   */
      {
         len = strlen;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = strlen;
      }
      if ( beg > strlen )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > strlen )        /* range bleeds right           */
      {
         len = strlen - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = strlen + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = strlen;
         }
         if ( beg > strlen )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > strlen )     /* range bleeds right           */
         {
            len = strlen - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = strlen;
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
    count = beg;
    end = beg + len + 1;
    while (count< end)
    {
         if (NOT OS_ISLETTER(s_char_s(*str,count)))/* if not character */
            {
            while (NOT OS_ISLETTER(s_char_s(*str,count)) \
                             && count < end) /* skip */
              count++;
            }
         if (count < end )            /* beginning of the word */
         {
              if (OS_ISLOWER(s_char_s(*str,count)) )
                  s_char_s(*str,count) = s_char_s(*str,count) + \
                       OS_UP_DIFF;    /* uppercase first character */
              count ++;
              while (OS_ISLETTER(s_char_s(*str,count)) && count < end)
                                      /* lowercase rest of the word */
              {
               if (NOT OS_ISLOWER(s_char_s(*str,count)) )
                     /* change to lower case only if in upper case */
                   s_char_s(*str,count)= s_char_s(*str,count) + \
                                       OS_LO_DIFF;
                count ++;
              }
         }
    }
   return(0);
}
