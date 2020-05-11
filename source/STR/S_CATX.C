/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     S_CATX C                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    packet     string*    IN/OUT  String to update with    |*/
/*|                                          new string contents.     |*/
/*|                                                                   |*/
/*| :PARAM.    data       string     IN      String to add to packet. |*/
/*|                                                                   |*/
/*| :PARAM.    begin      sint       IN      Starting position in data|*/
/*|                                                                   |*/
/*| :PARAM.    len        sint       IN      How much data to add.    |*/
/*|                                                                   |*/
/*| :PURPOSE.  Concatenate the indicated portion of the data string to|*/
/*|            the end of the pacet string.  String header data is    |*/
/*|            preserved in the cat.  Each string header is aligned on|*/
/*|            an even pointer address.                               |*/
/*|                                                                   |*/
/*| :OUTPUT.   Zero indicates no error; otherwise standard STR errors.|*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     26_Jan_88   J.L.Foster  Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     17_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For pointer testing.         */

sint   s_catx( string *packet, string data, sint begin, sint len )

{

   sint     bias     =  0     ;        /* If 1, even alignment needed  */
   sint     ulen     =  0     ;        /* Usage len of data            */
   sint     rc       =  0     ;        /* Our return code              */
   byte    *pos      =  NIL   ;
   sint     out_len  =  0     ;        /* How long result will be      */





   if(len < 0)
   {
      rc = S_BAD_LEN;
   }
   else
   {
      ulen = s_len (data);
{
   if ( begin == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > ulen )       /* also OK if LEN is too long   */
      {
         len = ulen;
      }
   }
   else if ( begin > 0 )                  /* When B is positive...        */
   {
      begin--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = ulen;
      }
      if ( begin > ulen )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( begin + len > ulen )        /* range bleeds right           */
      {
         len = ulen - begin;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      begin = ulen + begin;
      if ( begin >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = ulen;
         }
         if ( begin > ulen )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( begin + len > ulen )     /* range bleeds right           */
         {
            len = ulen - begin;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         begin = 0;
         len = ulen;
      }
      else if ( begin + len < 0 )          /* Completely out of range      */
      {
         len = 0;
      }
      else
      {                                /* Range bleeds left            */
         len = len + begin;                 /* NB -- adding a negative      */
         begin = 0;
      }
   }
}

      if ( len == 0 )
      {
         rc = 0;
      }
      else
      {
         /*+----------------------------------------------------------+*/
         /*| Make sure the target string ends on an odd pointer       |*/
         /*| address so the next string cat'ed in will begin on an    |*/
         /*| even pointer address.                                    |*/
         /*+----------------------------------------------------------+*/
         if( len % 2 )
         {

            bias = 1;
         }

         /*+----------------------------------------------------------+*/
         /*| Calculate what the resulting packet size will be; if     |*/
         /*| necessary, extend the packet to the final size.          |*/
         /*+----------------------------------------------------------+*/
         out_len = s_len(*packet) + len + bias + S_OVERHEAD;

         rc = s_alloc(packet,out_len);
         if ( S_BAD_RC(rc) )
         {
         }
         else
         {
            pos = s_buffer(*packet) + s_len(*packet);

#if STR_LEVEL_OPTIMIZED

               rc = os_memc((byte*)data,pos,(len+S_OVERHEAD+bias));
               if ( rc != 0 )
               {
                  rc = S_OSK_ERROR;
               }

#else

               os_memc(data, pos, S_OVERHEAD);
               os_memc(s_buffer(data)+begin, pos+S_OVERHEAD, len);

#endif

            s_smp_ul(*packet) = out_len;
            s_smp_ul((string) pos) = len;
            s_smp_al((string) pos) = len+bias;

         }
      }
   }

   return(rc);

}
