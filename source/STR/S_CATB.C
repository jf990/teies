/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     s_catb.c                                               |*/
/*|                                                                   |*/
/*| :AUTHOR.   John L. Foster                                         |*/
/*|                                                                   |*/
/*| :DATE.     January 29, 1988                                       |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.   None                                                   |*/
/*|                                                                   |*/
/*| :PARAM.    packet      string *  INOUT   String to update with new|*/
/*|                                          binary contents.         |*/
/*|                                                                   |*/
/*| :PARAM.    data        byte *    INPUT   Pointer to binary data   |*/
/*|                                          cat on to packet.        |*/
/*|                                                                   |*/
/*| :PARAM.    len         sint      INPUT   Amount of data to cat.   |*/
/*|                                                                   |*/
/*| :PURPOSE . Concatenate the indicated portion of the data to       |*/
/*|            the end of the packet string.  Len bytes are concaten- |*/
/*|            ated to the end of the used potion of the packet string|*/
/*|            and the used length is corrected.                      |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*| :CALLS.    os_memc(), s_alloc(), s_drop()                         |*/
/*|                                                                   |*/
/*| :ERRORS.   None known.                                            |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     20_May_88   N.R.Voronka Deleted unnecessary #include of|*/
/*|                                    obj.h                          |*/
/*|                                                                   |*/
/*| :MODS.     22_Mar_89   K.J.Walsh   Modified to correct length     |*/
/*|                                    calculation bug.               |*/
/*|                                                                   |*/
/*| :MODS.     17_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For pointer testing          */

sint   s_catb( string *packet, byte *data, sint len )

{

   sint  rc       = 0   ;              /* Our return code              */
   byte *pos      = NIL ;              /* Where in packet we put data  */
   sint  newlen   = 0   ;              /* Resulting USAGE len of packet*/





   if ( len <= 0 )
   {
      rc = S_BAD_LEN;
   }
   else
   {
      newlen = s_len(*packet) + len;   /* Current usage len + data len */

      /*+-------------------------------------------------------------+*/
      /*| s_alloc() will determine if the packet needs to be bigger.  |*/
      /*| If so, then then will allocate an EVEN length string, and   |*/
      /*| copy the existing data.  It will also do some error         |*/
      /*| detection for us.                                           |*/
      /*+-------------------------------------------------------------+*/
      rc = s_alloc(packet,newlen);
      if ( S_BAD_RC(rc) )
      {
      }
      else
      {
         pos = s_buffer( *packet ) + s_len( *packet );

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(data,pos,len);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            os_memc( data, pos, len );

#endif

         s_smp_ul( *packet ) = newlen;
      }

   }

   return(rc);

}
