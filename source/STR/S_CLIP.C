/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     s_clip (&target, &source, beg, len)                    |*/
/*|                                                                   |*/
/*| :LEVEL.    str                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    *target     string   IN/OUT   contains B&L of source   |*/
/*| :PARAM.    *source     string   IN/OUT   B&L is deleted from it   |*/
/*| :PARAM.     beg        string   INPUT    beg pos of str section   |*/
/*| :PARAM.     len        string   INPUT    len of string section    |*/
/*|                                                                   |*/
/*| :PURPOSE.  This routine clears target and then copies L chars     |*/
/*|            starting at the B'th position into it. Then it clears  |*/
/*|            this same section from source.                         |*/
/*|                                                                   |*/
/*|            If the target and source and the same string, then     |*/
/*|            the portion B/L is removed and discarded.              |*/
/*|                                                                   |*/
/*| :OUTPUT.   Standard string error codes.                           |*/
/*|                                                                   |*/
/*| :CALLS.  s_copy(), s_cat(), s_nulout(), s_len()                   |*/
/*|                                                                   |*/
/*| :ERRORS.  S_BAD_PARM, S_BAD_LEN                                   |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     31_May_87   N.Voronka   Written.                       |*/
/*|                                                                   |*/
/*| :MODS.     01_Jun_87   K.J.Walsh   Standard R'codes, minor        |*/
/*|                                                                   |*/
/*|                                         structure changes, calls. |*/
/*|                                                                   |*/
/*| :MODS.     21_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*| :MODS.     21_Sep_89   K.J.Walsh   Handles source==target.        |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For pointer testing.         */

#if NOT STR_LEVEL_OPTIMIZED
#error Only OPTIMIZED version of this routine.
#endif

s_clip (string *target, string *source, sint beg, sint len)
{
   sint   s_used = 0      ;
   sint   t_used = 0      ;
   sint   rc     = 0      ;
   sint   until  = 0      ;




   if (len < 0)
   {

      rc = s_nulout(target);

      rc = S_BAD_LEN;
   }
   else
   {
      s_used = s_len (*source) ;
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > s_used )       /* also OK if LEN is too long   */
      {
         len = s_used;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = s_used;
      }
      if ( beg > s_used )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > s_used )        /* range bleeds right           */
      {
         len = s_used - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = s_used + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = s_used;
         }
         if ( beg > s_used )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > s_used )     /* range bleeds right           */
         {
            len = s_used - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = s_used;
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
      if (len == 0 || s_used == 0 )
      {

         rc = s_nulout(target);

         rc = 0;
      }

      else
      {
         if ( *target != *source )
         {
            /*+-------------------------------------------------------+*/
            /*| If target and source differ, copy out the portion     |*/
            /*| being cut out.                                        |*/
            /*+-------------------------------------------------------+*/

            rc = s_alloc(target,len);

            if ( rc == 0 )
            {
               s_smp_ul(*target) = len;
               rc = os_memc(s_buffer(*source)+beg,s_buffer(*target),len);

               if ( rc != 0 )
               {
                  rc = S_OSK_ERROR;
               }
            }
         }

         if ( rc == 0 )
         {
            /*+----------------------------------------------------+*/
            /*| Now move the remaining portion of the source up to |*/
            /*| where the cut started.                             |*/
            /*+----------------------------------------------------+*/
            until = s_used - (beg + len);
            s_smp_ul(*source) = s_used - len;
            rc = os_memc( s_buffer(*source)+beg+len,
                          s_buffer(*source)+beg, until );
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }
         }
      }
   }

   return(rc);
}
