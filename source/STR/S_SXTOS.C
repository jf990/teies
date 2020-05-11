/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_mtoxs.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  OUTPUT   results                  |*/
/*|  :PARAM.   source      string   INPUT    string to be mapped      |*/
/*|  :PARAM.   beg         sint     INPUT    where to start in string |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|                                                                   |*/
/*|  :PURPOSE. Translates a string of hex characters into a string.   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  standard str RC if error                               |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    19_Jan_87   K.Walsh     Written                        |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "str.h"
#include "osk.h"
#include "debug.h"


sint s_sxtos (target,source,beg,len)
string *target ;                  /* Target for results                */
string  source ;                  /* String to use                     */
sint    beg    ;                  /* Beginning of string to use        */
sint    len    ;                  /* Length of string to user          */
{

   sint     counter  =  0     ;        /* A counter                    */
   sint     part     =  0     ;        /* Part of hex constant         */
   sint     end      =  0     ;        /* BEG+LEN, end of range        */
   sint     ulen     =  0     ;        /* Usage lenth of source        */
   sint     rc       =  0     ;        /* RC from things               */
   byte     attr     =  0     ;        /* charcter attributes mask     */
   byte     c1       =  0     ;        /* 1st part of hex digit nibble */
   byte     c2       =  0     ;        /* 2nd part of hex digit nibble */
   sint     x1       =  0     ;        /* 1's part of hex digit        */
   sint     x2       =  0     ;        /* 16's part of hex digit       */
   sint     outlen   =  0     ;        /* Accrued output length        */
   bool     same_str =  FALSE ;        /* Is source == target?         */
   string   work_str =  S_NUL ;        /* Used to create results       */





   if ( *target == source )            /* Does target == source?       */
   {
      same_str = TRUE;                 /* Yes--set for special process */
      work_str = S_NULL;               /* Use for real output          */
   }
   else
   {
      same_str = FALSE;                /* No--go about it normally     */
      work_str = *target;              /* Just copy address            */
      rc = s_nulout(&work_str);
   }

   if ( len < 0 )
   {
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
   if ( len == 0 )
   {
   return(0);
   }
   if ( len % 2 != 0 )
   {
   return(S_BAD_HLEN);
   }
   end = (beg + len);

   s_alloc(&work_str,len);
   s_smp_ul(work_str) = 0;
   outlen = 0;

   for ( counter = beg ; counter < end ; counter += 2 )
   {
      c1 = s_char_s(source,counter);
      c2 = s_char_s(source,counter+1);

      if ( OS_ISNUMERIC(c1) )
      {
         x1 = (OS_C2VAL(c1)) * 0x10;
      }
      else
      {
         c1 = OS_TOUPPER(c1);
         if ( ( c1 >= 'A' && c1 <= 'F' ))
         {
            x1 = (c1 - 'A' + 10) * 0x10;
         }
         else
         {
   return(S_BAD_NUM);
         }
      }

      if ( OS_ISNUMERIC(c2) )
      {
         x2 = OS_C2VAL(c2);
      }
      else
      {
         c2 = OS_TOUPPER(c2);
         if ( ( c2 >= 'A' && c2 <= 'F' ))
         {
            x2 = c2 - 'A' + 10;
         }
         else
         {
   return(S_BAD_NUM);
         }
      }
      s_char_s(work_str,outlen) = ( x1 + x2 );
      outlen++;
   }
   s_smp_ul(work_str) = outlen;

   if ( same_str == TRUE )
   {
      rc = s_drop(target);             /* Deallocate target/source str */
         if (S_BAD_RC(rc))
         {
             sint rc1 = 0;             /* Only needed in DEV system    */
             rc1 = s_drop(&work_str);
          }
   }

   *target = work_str;                 /* Return address (ok if same)  */
   return(0);
}
