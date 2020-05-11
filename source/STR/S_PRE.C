/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_pre()                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  OUTPUT   string to be updated     |*/
/*|  :PARAM.   current     string   INPUT    what to prefix           |*/
/*|  :PARAM.   prefix      string   INPUT    prefix (limit by B,L)    |*/
/*|  :PARAM.   beg         sint     INPUT    starting position        |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|                                                                   |*/
/*|  :PURPOSE. To prefix the indicated portion of "prefix" onto       |*/
/*|            "current" creating "target"                            |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns sint string rcode or zero                      |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS. S_BAD_LEN                                               |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    09_Feb_87   K.J.Walsh   Dummy version created;         |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Documented to 1.42 standards   |*/
/*|  :MODS.    11_Sep_87   K.J.Walsh   Implemented                    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"

sint s_pre(target, current, prefix, beg, len_p)
string           *target ;             /* TEIES String to output       */
string            current;             /* TEIES String to be prefixed  */
string            prefix ;             /* the prefix                   */
sint              beg    ;             /* indicates beginning of prefix*/
sint              len_p  ;             /* How many characters to prefix*/
{
   sint           count    =  0     ;  /* Counter for copying loop     */
   sint           count2   =  0     ;  /* Counter for copying loop     */
   sint           len_c    =  0     ;  /* Used length of current       */
   sint           p_u_len  =  0     ;  /* Used length of prefix        */
   sint           len_t    =  0     ;  /* Used len of target AFTERWARDS*/
   sint           rc       =  0     ;  /* Return code from str funs    */
   boolean        new_tar  =  TRUE  ;  /* Is target not also current?  */
   string         dummy    =  S_NULL;  /* Dummy incase we have to copy */




   if (len_p < 0)                 /* Check for negative length         */
   {
      s_nulout(target);
   return(S_BAD_LEN);
   }

   len_c = s_len(current);
   p_u_len = s_len(prefix);
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len_p == 0 || len_p > p_u_len )       /* also OK if LEN is too long   */
      {
         len_p = p_u_len;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len_p == 0 )
      {                                /* LEN zero indicates entire str*/
         len_p = p_u_len;
      }
      if ( beg > p_u_len )                  /* BEG past last byte           */
      {
         len_p = 0;
      }
      else if ( beg + len_p > p_u_len )        /* range bleeds right           */
      {
         len_p = p_u_len - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = p_u_len + beg;
      if ( beg >= 0 )
      {
         if ( len_p == 0 )
         {                             /* LEN zero indicates entire str*/
            len_p = p_u_len;
         }
         if ( beg > p_u_len )               /* BEG past last byte           */
         {
            len_p = 0;
         }
         else if ( beg + len_p > p_u_len )     /* range bleeds right           */
         {
            len_p = p_u_len - beg;             /* Cut length                   */
         }
      }
      else if ( len_p == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len_p = p_u_len;
      }
      else if ( beg + len_p < 0 )          /* Completely out of range      */
      {
         len_p = 0;
      }
      else
      {                                /* Range bleeds left            */
         len_p = len_p + beg;                 /* NB -- adding a negative      */
         beg = 0;
      }
   }
}

/*+-------------------------------------------------------------------+*/
/*| Calculate result string length--if zero, make sure the string is  |*/
/*| either unallocated or set used length to zero (nullout).          |*/
/*+-------------------------------------------------------------------+*/
   len_t = len_c + len_p ;


   if ( len_t == 0 )
   {
      if ( *target != S_NUL )          /* If target is not already     */
         s_smp_ul(*target) = 0;        /* NULL, then S_NULOUT it.      */
   }
   else                                /*                              */
   {                                   /*                              */
   /*+----------------------------------------------------------------+*/
   /*| If target == current then we have to copy the current          |*/
   /*+----------------------------------------------------------------+*/
      if ( *target == current )           /*Start & target are identical! */
      {
         new_tar = FALSE;
         rc = s_copy(&dummy,current,0,0);
      }
      else
      {
         dummy = current;
         new_tar = TRUE;
      }

      rc = s_alloc(target,len_t);

      s_smp_ul(*target) = len_t;          /* Stick in the used lenght  */

   /*+----------------------------------------------------------------+*/
   /*| Actual copying of string occurs here.                          |*/
   /*+----------------------------------------------------------------+*/

      for ( count = 0 ; count < len_p  ; count++ )
      {
         s_char_s(*target,count) = s_char_s(prefix,count+beg);
      }

      for ( count2 = 0 ; count2 < len_c ; count++, count2++ )
      {
         s_char_s(*target,count) = s_char_s(dummy,count2);
      }
   }
/*+-------------------------------------------------------------------+*/
/*| Cleanup & get gowin                                               |*/
/*+-------------------------------------------------------------------+*/


   if ( NOT new_tar )
   {
      rc = s_drop(&dummy);
   }

   return(rc);
}
