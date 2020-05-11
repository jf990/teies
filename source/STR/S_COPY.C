/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     name()                                                 |*/
/*|                                                                   |*/
/*| :LEVEL.    str                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    *target     string   OUTPUT   string to be overwritten |*/
/*| :PARAM.    source      string   INPUT    copy from this string    |*/
/*| :PARAM.    beg         sint     INPUT    postion to start copy    |*/
/*| :PARAM.    len         sint     INPUT    how much to copy         |*/
/*|                                                                   |*/
/*| :PURPOSE.  Copies the precscribed portion of 'source' to 'target' |*/
/*|                                                                   |*/
/*| :OUTPUT.   sint zero or standard string RC                        |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     18_Aug_86   N.Voronka   Written.                       |*/
/*|                                                                   |*/
/*| :MODS.     23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*| :MODS.     02_Jun_87   N.Voronka   Added s_alloc to take care of  |*/
/*|                                    the case when the target str   |*/
/*|                                    is S_NULL.                     |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*| :MODS.     16_Mar_89   K.J.Walsh   Introduced use of S_CALC()     |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"

#include "osk.h"                       /* For allocations functions    */

sint s_copy (string *target, string source, sint beg, sint len)
{
   sint     count =  0     ;           /* Counter for copy loop        */
   sint     ulen  =  0     ;           /* Used length of input         */
   sint     rc    =  0     ;           /* Return code to be used       */




   if (len < 0)
   {
      s_nulout(target);
      rc = S_BAD_LEN;
   }
   else
   {
      ulen = s_len(source);

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

      if ( len == 0 )
      {
         s_nulout(target);
         rc = 0;
      }
      else
      {
         rc = s_alloc (target, len) ;

         if ( rc != 0 )
         {
         }
         else
         {
            s_smp_ul(*target) = len;      /* Set target used len       */

#if STR_LEVEL_OPTIMIZED

               rc = os_memc(s_buffer(source)+beg,s_buffer(*target),len);
               if ( rc != 0 )
               {
                  rc = S_OSK_ERROR;
               }

#else

               for (count = 0   ; count <= len; count++)
                 s_char_s (*target,count) = s_char_s (source, count+beg);

#endif
         }
      }
   }


   return(rc);
}
