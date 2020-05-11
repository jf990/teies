/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     S_STOCS C                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    cstr       *byte      IN   Address of C-string pointer.|*/
/*| :PARAM.    tstr       string     IN   Input C string.             |*/
/*| :PARAM.    beg        sint       IN   Beginning to copy from.     |*/
/*| :PARAM.    len        sint       IN   Length to copy.             |*/
/*|                                                                   |*/
/*| :PURPOSE.  Replaces s_eton().                                     |*/
/*|                                                                   |*/
/*|            Copies S1 from BEG for LEN into the indicated Cstr and |*/
/*|            appends a '\0'.                                        |*/
/*|                                                                   |*/
/*|            If cstr is a NIL pointer, memory is allocated of the   |*/
/*|            required length.  If not NIL, it is presumed that the  |*/
/*|            call has already allocated adequate space for the      |*/
/*|            copy.                                                  |*/
/*|                                                                   |*/
/*| :OUTPUT.   Always zero                                            |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   If Cstr is not NIL, then it must point to an allocation|*/
/*|            long enough to contain the copied bytes and a null.    |*/
/*|                                                                   |*/
/*|            It is the caller's responsibility to make sure that    |*/
/*|            the Cstr is eventually deallocated.                    |*/
/*|                                                                   |*/
/*| :MODS.     06_Jul_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"

sint s_stocs( cstr , tstr , beg , len )

byte     **cstr;                       /* Pointer to cstr pointer      */
string   tstr;                         /* Source TEIES string          */
sint     beg;                          /* Where to begin conversion    */
sint     len;                          /* How much to convert          */
{
   sint    used     =  0  ;            /* Used length of source        */
   sint    end      =  0  ;            /* Indicates end of copy op     */
   sint    count    =  0  ;            /* Index into output string     */





   if ( len < 0 )
   {
   return(S_BAD_LEN);
   }

   used = s_len(tstr);                 /* We need the used length.     */
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
   if ( len == 0 )                     /* Null range indicated         */
   {
      if ( *cstr != NIL )
      {
         **cstr = '\0';
      }
   }

   else
   {
      if ( *cstr == NIL )              /* If given a null string then  */
      {
         *cstr = os_mema(len+1);       /* ...allocate it for us.       */
      }
                                       /* fails... not done in PROD    */
      end = beg + len;

      for ( count = 0 , end = beg + len ; beg < end ; beg++)
      {
         (*cstr)[count] = s_char_s(tstr,beg);
         count++;
      }

      (*cstr)[count] = '\0';
   }
   return(0);
}
