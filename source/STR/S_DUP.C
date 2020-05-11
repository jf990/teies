/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_dup()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string   INPUT    string to be updated     |*/
/*|  :PARAM.   source      string*  OUTPUT   original string          |*/
/*|  :PARAM.   beg         sint     INPUT    starting position        |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|  :PARAM.   times       sint     INPUT    number of copies to make |*/
/*|                                                                   |*/
/*|  :PURPOSE. Duplicates the input string according to other params  |*/
/*|            and replaces the target string with it.                |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Not in test bed.                                       |*/
/*|                                                                   |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Documented to 1.42 standards   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"

sint s_dup (target, source, beg, len, times )
string *target ;                       /* Target for duplication output*/
string  source ;                       /* Input string                 */
sint    beg    ;                       /* Beginning of 'source' to copy*/
sint    len    ;                       /* Length of 'source' to copy   */
{
   sint     rc       =  0     ;        /* RC from called stuff         */
   sint     count    =  0     ;        /* counter for copy             */
   sint     pointer  =  0     ;        /* pointer into target          */
   string   *real_tar=  0     ;        /* duplicate of target for dup  */
   sint     tot_len  =  0     ;        /* length X times               */
   sint     ulen     =  0     ;        /* used length of source        */




   if ( times < 0 )
   {
      rc = s_nulout(target);
   return(0);
   }

   if (len < 0)                   /* Check for negative length         */
   {
      s_nulout(target);
   return(S_BAD_LEN);
   }

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
   tot_len = len * times;
   if ( len == 0 )
   {
      s_nulout(target);
   return(0);
   }
   else if ( len > S_MAXLEN )
   {
      s_nulout(target);
   return(S_BAD_SIZE);
   }


   if ( *target != source )            /* Do if not same               */
   {
      real_tar = target ;              /* Not identical               */
   }

   rc = s_alloc(real_tar,tot_len);

   s_smp_ul(*real_tar) = tot_len;

   pointer = 0 ;                       /* Pointer into target string   */

   for ( ; times > 0 ; times-- )       /* Do while not yet zero        */
   {
      for ( count = beg ; count < len + beg ; count++)
      {
         s_char_s(*real_tar,pointer++) = s_char_s(source,count);
      }
   }

   if ( *target == source )
   {
      rc = s_drop(target);
   }

   target = real_tar;


   return(0);
}
