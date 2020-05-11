/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     S_MOVE C                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    memptr     *byte      IN/OUT  Pointer to first byte of |*/
/*|                                          the memory buffer.       |*/
/*|                                                                   |*/
/*| :PARAM.    strptr     *string    IN/OUT  A TEIES string that data |*/
/*|                                          will come from or go to. |*/
/*|                                                                   |*/
/*| :PARAM.    beg        sint       IN      Offset into string where |*/
/*|                                          movement will start.     |*/
/*|                                                                   |*/
/*| :PARAM.    len        sint       IN      Total number of bytes to |*/
/*|                                          be moved.                |*/
/*|                                                                   |*/
/*| :PARAM.    dir        sint       IN      Direction to move:  S_IN |*/
/*|                                          moves IN to memory from  |*/
/*|                                          the string; S_OUT moves  |*/
/*|                                          OUT of memory into the   |*/
/*|                                          string.                  |*/
/*|                                                                   |*/
/*| :PURPOSE.  If the direction requested is S_IN, move a section of  |*/
/*|            a TEIES string offset by the length parameter to memory|*/
/*|            pointed to by memptr.  If an offset from memptr is     |*/
/*|            needed add that offset to memptr before calling s_move.|*/
/*|                                                                   |*/
/*|            If the direction requested is S_OUT, move a section of |*/
/*|            memory into a TEIES string offset by the length        |*/
/*|            parameter.  In an offset from memptr memory (in this   |*/
/*|            case the parameter strptr) is needed to add the offset |*/
/*|            before calling s_move.                                 |*/
/*|                                                                   |*/
/*|   memptr                      strptr                              |*/
/*|        +------------------+         +-----------------------+     |*/
/*|        |                  |         |   |...............|   |     |*/
/*|        +--------------+---+         +---+---------------+---+     |*/
/*|        |              |                 |               |         |*/
/*|        +--------------|-----------------+ beg           | len     |*/
/*|                       |                                 |         |*/
/*|                       +---------------------------------+         |*/
/*|                                                                   |*/
/*| :OUTPUT.   Return code is zero if OK, otherwise error listed below|*/
/*|            or return code from failing subroutine.                |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.   S_BAD_PARM, S_BAD_LEN                                  |*/
/*|                                                                   |*/
/*| :LIMITS.   If the destination memory pointer is NILL then         |*/
/*|            S_BAD_PARM is returned and nothing is done.  If the    |*/
/*|            requested number of bytes to move is more than the     |*/
/*|            destination cand heandle, the bytes are moved anyway,  |*/
/*|            possibly overwriting memory it should not be allowed   |*/
/*|            to.  Because this situation cannot be checked, it is   |*/
/*|            the callers responsibility to make the necessary       |*/
/*|            precautions against this error.                        |*/
/*|                                                                   |*/
/*| :MODS.     11_Jun_87   J.L.Foster  Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     29_Jun_87   K.J.Walsh   Fixed incorrect len            |*/
/*|                                    calculations and removed copy  |*/
/*|                                    of 4 str UL/AL bits.           |*/
/*|                                                                   |*/
/*| :MODS.     10_Aug_87   J.L.Foster  Added dir parameter.           |*/
/*|                                                                   |*/
/*| :MODS.     25_Jan_88   K.J.Walsh   Added pointer checking.        |*/
/*|                                                                   |*/
/*| :MODS.     17_Aug_89   K.J.Walsh   Optimized.                     |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For pointer checking         */


sint s_move (byte *memptr, string *strptr, sint beg, sint len, sint dir)

{

   sint      used   = 0;          /* Used length of TEIES string       */
   sint      end    = 0;          /* Beg + Len                         */
   sint      count  = 0;          /* Counter used for copying          */
   sint      rc     = 0;          /* RC from things                    */
   sint      inlen  = 0;          /* Stored for checking in S_OUT      */





   if ( memptr == NIL )
   {
      rc = S_BAD_PARM;
   }

   else
   {
      used   = s_len(*strptr);

      if ( dir == S_IN )
      {
         /*+----------------------------------------------------------+*/
         /*|                                                          |*/
         /*| Process a move from a string IN to memory.               |*/
         /*|                                                          |*/
         /*+----------------------------------------------------------+*/

{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > used )       /* also OK if LEN is too long   */
      {
         len = used;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = used;
      }
      if ( beg > used )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > used )        /* range bleeds right           */
      {
         len = used - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = used + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = used;
         }
         if ( beg > used )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > used )     /* range bleeds right           */
         {
            len = used - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = used;
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

         if (len < 0 )                 /* Check for negative length    */
         {
            rc = S_BAD_LEN;
         }
         else if ( len == 0 )
         {
            rc = S_BAD_PARM;
         }
         else
         {
            end = beg + len;


#if STR_LEVEL_OPTIMIZED

               rc = os_memc((s_buffer(*strptr)+beg),memptr,len);
               if ( rc != 0 )
               {
                  rc = S_OSK_ERROR;
               }

#else

               for ( count = beg   ; count < end   ; count++)
               {
                  *memptr = s_char_s( *strptr , count ) ;
                  memptr++;
               }

#endif

         }
      }

      else if ( dir == S_OUT )
      {
         /*+----------------------------------------------------------+*/
         /*|                                                          |*/
         /*| Process a move OUT of memory into a string.              |*/
         /*|                                                          |*/
         /*+----------------------------------------------------------+*/

         inlen = len;                  /* Save the origional value     */

         if ( used == 0 )              /* If usage length is zero, use */
            used = len;                /* the eventual length          */

{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > used )       /* also OK if LEN is too long   */
      {
         len = used;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = used;
      }
      if ( beg > used )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > used )        /* range bleeds right           */
      {
         len = used - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = used + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = used;
         }
         if ( beg > used )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > used )     /* range bleeds right           */
         {
            len = used - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = used;
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

         if (len < 0 )
         {
            rc = s_nulout(strptr);
            rc = S_BAD_LEN;
         }

         else if ( len == 0 )
         {
            rc = s_nulout(strptr);
            rc = S_BAD_PARM;
         }
         else
         {
            if ( len < inlen )
               len = inlen;            /* Len was improperly adjusted  */

            if (len > S_MAXLEN )
            {
               rc = s_nulout(strptr);
               rc = S_BAD_LEN;
            }

            end = beg + len;

            if ( s_allen(*strptr) < end )
            {
               rc = s_alloc(strptr,end);
            }


#if STR_LEVEL_OPTIMIZED

               rc = os_memc(memptr,s_buffer(*strptr)+beg,len);
               if ( rc != 0 )
               {
                  rc = S_OSK_ERROR;
               }

#else

               for ( count = beg ; count < end ; count++)
               {
                  s_char_s (*strptr , count ) =  *(memptr++);
               }

#endif

            if ( end >= used   )       /* is usage already bigger?     */
               s_smp_ul(*strptr) = (hword) end;  /* No?  Then set it.  */

         }

      }

      else
      {
         /*+----------------------------------------------------------+*/
         /*|                                                          |*/
         /*| If we end up here then a bad direction was given.        |*/
         /*|                                                          |*/
         /*+----------------------------------------------------------+*/
         rc = S_BAD_PARM;
      }
   }
   return(rc);
}
