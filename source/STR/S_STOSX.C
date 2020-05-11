/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_mtoxs.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  OUTPUT   results                  |*/
/*|  :PARAM.   source      string   INPUT    string to be translated  |*/
/*|  :PARAM.   beg         sint     INPUT    where to start           |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|                                                                   |*/
/*|  :PURPOSE. To translate the contents of a string into a hexdump.  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns sint string rcode or zero                      |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    19_Jan_88   K.Walsh     Written                        |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"


extern   sint  s_nulout () ;
extern   sint  s_alloc  () ;

sint s_stosx (target,source,beg,len)
string *target ;                  /* Target for results                */
string  source ;                  /* String to use                     */
sint    beg    ;                  /* Beginning of string to use        */
sint    len    ;                  /* Length of string to user          */
{
   static byte hex_char[] = {'0','1','2','3','4','5','6','7','8','9',\
                               'A','B','C','D','E','F' };

   sint  real_len =  0  ;              /* Length of outgoing string.   */
   sint  ulen     =  0  ;              /* Used length of input string. */
   sint  counter  =  0  ;              /* A counter                    */
   sint  part     =  0  ;              /* Part of hex constant         */
   sint  rc       =  0  ;              /* RC from things               */





   rc = s_nulout(target);

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

   real_len = len * 2;
   if ( real_len > S_MAXLEN )
   {
   return(S_BAD_PARM);
   }


   s_alloc(target,real_len);
   s_smp_ul(*target) = real_len;

   counter = 0;
   while ( counter < real_len )
   {
      s_char_s(*target,counter++) = \
         hex_char[(((sint) source - (sint) source % 0x10) / 0x10)];
      s_char_s(*target,counter++) = \
         hex_char[(       (sint) source % 0x10)];
      source++;
   }

   return(0);
}
