/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     S_VERIFY C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    subject    string     IN   String to be verified;      |*/
/*|                                                                   |*/
/*| :PARAM.    char_set   string     IN   String of allowed or        |*/
/*|                                          disallowed characters;   |*/
/*|                                                                   |*/
/*| :PARAM.    beg        sint       IN   Where to begin looking in   |*/
/*|                                          the subject string;      |*/
/*|                                                                   |*/
/*| :PARAM.    len        sint       IN   How long to look in subject |*/
/*|                                          string;                  |*/
/*|                                                                   |*/
/*| :PARAM.    param      sint       IN   Indicates if char_set string|*/
/*|                                          characters are permitted |*/
/*|                                          to occur in subject      |*/
/*|                                          string (S_ALLOW) or not  |*/
/*|                                          permitted (S_DISALLOW).  |*/
/*|                                                                   |*/
/*| :PURPOSE.  Verifies that a string is made up entirely of          |*/
/*|            characters indicated in the second string (S_ALLOW)    |*/
/*|            or if it does not contain any of the characters in the |*/
/*|            second string (S_DISALLOW).                            |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns zero if the indicated portion of the string    |*/
/*|            is verified; returns a positive number indicating      |*/
/*|            the location of the first string found searching       |*/
/*|            from right-to-left that is not valid; or returns a     |*/
/*|            negative string error code for other errors.           |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     03_Mar_89   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "str_i.h"
#include "debug.h"

sint s_verify( string   subject,
               string   char_set,
               sint     beg,
               sint     len,
               sint     param)
{
   sint     ulen     =  0     ;        /* Used length of subject str   */
   sint     cs_len   =  0     ;        /* Used length of char_set str  */
   sint     count    =  0     ;        /* Counter for loops            */
   sint     end      =  0     ;        /* Relative end of search in sub*/
   sint     rc       =  0     ;        /* Our return code              */
   bool     found    =  FALSE ;        /* Sub char found in char_set?  */
   byte     check_me =  ' '   ;        /* Char from sub str to check   */




   if (len < 0)
   {
      rc = S_BAD_LEN;
   }
   else
   {
      ulen = s_len(subject);
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > ulen )       /* also OK if LEN is too long   */
      {
         len = ulen;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = ulen;
      }
      if ( beg > ulen )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > ulen )        /* range bleeds right           */
      {
         len = ulen - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = ulen + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = ulen;
         }
         if ( beg > ulen )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > ulen )     /* range bleeds right           */
         {
            len = ulen - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = ulen;
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
      end = beg + len;
      cs_len = s_len(char_set);


      switch(param)
      {

         case S_ALLOW:

            if ( cs_len < 1 )          /* If valid char set is empty   */
            {
               if ( len < 1 )          /* ...and so is subject string  */
               {
                  rc = 0;
               }
               else
               {
                  rc = 1;
               }
            }
            else
            {

               for ( ; beg < end ; beg++ )
               {
                  check_me = s_char_s(subject,beg);
                  found = FALSE;
                  for ( count = 0; count < cs_len ; count++)
                  {
                     if ( check_me == s_char_s(char_set,count) )
                     {
                        found = TRUE;
                        break;
                     }
                  }

                  if ( found == FALSE )
                  {
                     rc = beg+1;
                     break;
                  }
               }

            }

            break;

         case S_DISALLOW:

            if ( cs_len < 1 )          /* If invalid char set is empty */
            {
               rc = 0;
            }
            else
            {

               for ( ; beg < end ; beg++ )
               {
                  check_me = s_char_s(subject,beg);
                  found = FALSE;
                  for ( count = 0; count < cs_len ; count++)
                  {
                     if ( check_me == s_char_s(char_set,count) )
                     {
                        found = TRUE;
                        break;
                     }
                  }

                  if ( found == TRUE )
                  {
                     rc = beg+1;
                     break;
                  }
               }

            }

            break;

         default:

            rc = S_BAD_PARAM;
            break;

      }                                /* End of "switch(param)"       */

   }                                   /* End of "if (len<0)...else"   */

   return(rc);

}
