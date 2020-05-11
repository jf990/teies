/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_first()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   needle      string   INPUT    indicates pattern to find|*/
/*|  :PARAM.   haystack    string   INPUT    string inwhich to find it|*/
/*|  :PARAM.   beg         sint     INPUT    starting point in haystak|*/
/*|  :PARAM.   len         sint     INPUT    length from starting pt. |*/
/*|                                                                   |*/
/*|  :PURPOSE. To find the first occurence of one string in a defined |*/
/*|            segment of another.                                    |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a sint indicating the 1st occurane of the 2nd  |*/
/*|            string in the first.                                   |*/
/*|                                                                   |*/
/*|  :CALLS.   s_len(), #s_calc()                                     |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    16_Mar_87   N.Voronka   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    16_Jan_88   K.J.Walsh   Removed complex, added s_calc()|*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "str_i.h"
#include "osk.h"                       /* For pointer testing          */

sint s_first (string needle, string haystack, sint beg, sint len)
{
   sint counth =  0  ;            /* Counter for the haystack          */
   sint countn =  0  ;            /* Counter for the needle            */
   sint lenh   =  0  ;            /* Used length of haystack           */
   sint lenn   =  0  ;            /* Used length of needle             */
   sint end    =  0  ;            /* Last character in needle pointer  */
   sint rc     =  0  ;            /* Our return code                   */




   if (len < 0)                   /* Check for negative length         */
   {
      rc = S_BAD_LEN;
   }
   else
   {

      lenh = s_len(haystack);
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
      if ( len == 0 )
      {
         rc = 0;
      }
      else
      {
         lenn = s_len(needle );
         if ( lenn == 0 )
         {
   return(0);
         }

         counth = beg;            /* Start at beginning                */
         end = beg + len;         /* Where to stop looking             */
         countn = 0 ;             /* Initialize needle  counter        */

         do
         {

            if (s_char_s(haystack,counth) == s_char_s (needle,countn))
            {
               countn++;           /* Look at next character in needle */
            }
            else
            {
               countn = 0 ;        /* Go back to start of needle       */
            }
            counth++;              /* Look at next char in haystak     */
            if (countn == lenn)    /* If match found compute beginning */
            {
               rc = counth - lenn + 1;
               counth = end + 1;   /* To cause exit from WHILE loop    */
            }
         }
         while (counth < end );
      }
   }

   return(rc);
}
