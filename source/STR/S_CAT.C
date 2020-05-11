/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  <NAME>    s_cat()                                                |*/
/*|                                                                   |*/
/*|  <LEVEL>   str                                                    |*/
/*|                                                                   |*/
/*|  <GLOBAL>                                                         |*/
/*|                                                                   |*/
/*|  <PARAM>   target      string*  INPUT    string to be updated     |*/
/*|  <PARAM>   start       string   OUTPUT   origional string 1st part|*/
/*|  <PARAM>   end         string   OUTPUT   origional string 2nd part|*/
/*|  <PARAM>   beg         sint     INPUT    starting position in 2nd |*/
/*|  <PARAM>   len         sint     INPUT    length to do in 2nd      |*/
/*|                                                                   |*/
/*|  <PURPOSE> To add the indicated portion of 'end' to 'start',      |*/
/*|            creating 'target'                                      |*/
/*|                                                                   |*/
/*|  <OUTPUT>  returns sint string rcode or zero                      |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  <ERRORS> S_BAD_LEN                                               |*/
/*|                                                                   |*/
/*|  <LIMITS>                                                         |*/
/*|                                                                   |*/
/*|  <MODS>    09_Feb_87   N.Voronka   Written.                       |*/
/*|  <MODS>    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*|  <MODS>    14_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*|  <END>                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For checking, allocation     */

sint s_cat (string *target, string start, string end, sint beg, sint len)
{
   sint     ulens    =  0     ;   /* Used length of start              */
   sint     ulene    =  0     ;   /* Used length of end                */
   sint     ulent    =  0     ;   /* Used length of target             */
   sint     count    =  0     ;   /* Counter for loops                 */
   string   dummy    =  S_NUL ;   /* Needed in some cases              */
   sint     rc       =  0     ;   /* Return code from other functions  */



   if (len < 0)                   /* Check for negative length         */
   {
      s_nulout(target);
      rc = S_BAD_LEN;
   }
   else
   {

      ulene = s_len (end) ;       /* Get used length of source2        */
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > ulene )       /* also OK if LEN is too long   */
      {
         len = ulene;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = ulene;
      }
      if ( beg > ulene )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > ulene )        /* range bleeds right           */
      {
         len = ulene - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = ulene + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = ulene;
         }
         if ( beg > ulene )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > ulene )     /* range bleeds right           */
         {
            len = ulene - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = ulene;
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

      ulens = s_len(start);
      ulent = ulens + len ;

      if ( *target == end )
      {

         rc = s_alloc(&dummy,ulent);

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(start),s_buffer(dummy),ulens);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

            rc = os_memc(s_buffer(end)+beg,s_buffer(dummy)+ulens,len);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < ulens ; count++ )
               s_char_s(dummy,count) = s_char_s(start,count);

            for ( count = 0 ; count < len  ; count++ )
               s_char_s(dummy,count+ulens) = s_char_s(end, count+beg);

#endif

         rc = s_drop(target);

         *target = dummy;
      }
      else if ( *target == start )
      {

         rc = s_alloc(target,ulent);

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(end)+beg,s_buffer(*target)+ulens,len);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < len  ; count++ )
               s_char_s(*target,count+ulens) = s_char_s(end, count+beg);
#endif
      }
      else
      {

         rc = s_alloc(target,ulent);

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(start),s_buffer(*target),ulens);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

            rc = os_memc(s_buffer(end)+beg,s_buffer(*target)+ulens,
               len);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < ulens ; count++ )
               s_char_s(*target,count) = s_char_s(start,count);

            for ( count = 0 ; count < len  ; count++ )
               s_char_s(*target,count+ulens) = s_char_s(end, count+beg);

#endif
      }

      if ( ulent != 0 )
      {
         s_smp_ul(*target) = ulent;
      }
   }

   return(rc);
}
