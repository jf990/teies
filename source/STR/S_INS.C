/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.      s_ins.c                                               |*/
/*|                                                                   |*/
/*| :LEVEL.     STR                                                   |*/
/*|                                                                   |*/
/*| :PARAM.     result    string*    OUT  String to be created;       |*/
/*|                                                                   |*/
/*| :PARAM.     source    string     IN   Initial string;             |*/
/*|                                                                   |*/
/*| :PARAM.     pos       sint       IN   Where to insert in source;  |*/
/*|                                                                   |*/
/*| :PARAM.     insert    string     IN   String to be inserted;      |*/
/*|                                                                   |*/
/*| :PARAM.     beg       sint       IN   Where in "insert" string to |*/
/*|                                       start copy;                 |*/
/*|                                                                   |*/
/*| :PARAM.     len       sint       IN   How much of "insert" to copy|*/
/*|                                                                   |*/
/*| :PURPOSE.  Inserts the portion of "insert" indicated by beg and   |*/
/*|            len into "source", creating "result".                  |*/
/*|                                                                   |*/
/*| :OUTPUT.   Zero or RC of falling routine.                         |*/
/*|                                                                   |*/
/*| :CALLS.    s_copy, s_cat, s_len                                   |*/
/*|                                                                   |*/
/*| :MODS.     10_Mar_88   Ing_Long Wu  Implemented.                  |*/
/*|                                                                   |*/
/*| :MODS.     23_Jun_88   A.K.Mehta    Updated to check condition    |*/
/*|                                     before copying first part     |*/
/*|                                     sous1                         |*/
/*|                                                                   |*/
/*| :MODS.     17_Aug_89   K.J.Walsh    Reimplemented as optimized.   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "str_i.h"
#include "osk.h"                       /* For pointer checking         */

sint s_ins(string *result, string source, sint pos,
   string insert, sint beg, sint len)

{
   string   holder      =  S_NULL;
   sint     result_len  =  0     ;
   sint     source_len  =  0     ;
   sint     insert_len  =  0     ;
   sint     rc          =  0     ;
   sint     until       =  0     ;
   sint     dummy       =  0     ;




   /*+--------------------------------------------------------------+*/
   /*| Check LEN                                                    |*/
   /*+--------------------------------------------------------------+*/
   if (len < 0)
   {
      rc = S_BAD_LEN;
   }

   else
   {

      /*+-----------------------------------------------------------+*/
      /*| Get existing lengths, calculate resulting length.         |*/
      /*+-----------------------------------------------------------+*/
      source_len = s_len(source);
      insert_len = s_len(insert);

{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > insert_len )       /* also OK if LEN is too long   */
      {
         len = insert_len;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = insert_len;
      }
      if ( beg > insert_len )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > insert_len )        /* range bleeds right           */
      {
         len = insert_len - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = insert_len + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = insert_len;
         }
         if ( beg > insert_len )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > insert_len )     /* range bleeds right           */
         {
            len = insert_len - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = insert_len;
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

      result_len = source_len + len;

      /*+-----------------------------------------------------------+*/
      /*| Calculate the "real" pos by tricking #s_calc() into doing |*/
      /*| "beg" for the pos.                                        |*/
      /*+-----------------------------------------------------------+*/
{
   if ( pos == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( dummy == 0 || dummy > source_len )       /* also OK if LEN is too long   */
      {
         dummy = source_len;
      }
   }
   else if ( pos > 0 )                  /* When B is positive...        */
   {
      pos--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( dummy == 0 )
      {                                /* LEN zero indicates entire str*/
         dummy = source_len;
      }
      if ( pos > source_len )                  /* BEG past last byte           */
      {
         dummy = 0;
      }
      else if ( pos + dummy > source_len )        /* range bleeds right           */
      {
         dummy = source_len - pos;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      pos = source_len + pos;
      if ( pos >= 0 )
      {
         if ( dummy == 0 )
         {                             /* LEN zero indicates entire str*/
            dummy = source_len;
         }
         if ( pos > source_len )               /* BEG past last byte           */
         {
            dummy = 0;
         }
         else if ( pos + dummy > source_len )     /* range bleeds right           */
         {
            dummy = source_len - pos;             /* Cut length                   */
         }
      }
      else if ( dummy == 0 )              /* Default says use it all      */
      {
         pos = 0;
         dummy = source_len;
      }
      else if ( pos + dummy < 0 )          /* Completely out of range      */
      {
         dummy = 0;
      }
      else
      {                                /* Range bleeds left            */
         dummy = dummy + pos;                 /* NB -- adding a negative      */
         pos = 0;
      }
   }
}

      /*+-----------------------------------------------------------+*/
      /*| Allocate a temporary string if the resultet is (1) not big|*/
      /*| enough, (2) equal to source, or (3) equal to insert.      |*/
      /*+-----------------------------------------------------------+*/
      if
      (
         s_allen(*result) < result_len ||
         *result == source ||
         *result == insert
      )
      {
         holder = *result;
         *result = S_NULL;
         rc = s_alloc(result,result_len);
      }
      s_smp_ul(*result) = result_len;

      /*+-----------------------------------------------------------+*/
      /*| Copy over the first part.                                 |*/
      /*+-----------------------------------------------------------+*/
      until = pos;
      if ( (rc == 0) && (until > 0))
      {

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(source),s_buffer(*result),until);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < until; count++)
            {
               s_char_s(*result,count) = s_char_s(source,count);
            }

#endif
      }


      /*+-----------------------------------------------------------+*/
      /*| Copy in the part being inserted.                          |*/
      /*+-----------------------------------------------------------+*/
      if ( (rc == 0) && (len > 0) )
      {

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(insert)+beg,s_buffer(*result)+pos,len);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < len; count++)
            {
               s_char_s(*result,pos++) = s_char_s(source,count+beg);
            }

#endif
      }

      /*+-----------------------------------------------------------+*/
      /*| Copy on the remainder of the source.                      |*/
      /*+-----------------------------------------------------------+*/
      until = result_len - (pos + len);
      if ( (rc == 0) && (until > 0))
      {

#if STR_LEVEL_OPTIMIZED

            rc = os_memc(s_buffer(source)+beg+len,
               s_buffer(*result)+pos+len,until);
            if ( rc != 0 )
            {
               rc = S_OSK_ERROR;
            }

#else

            for ( count = 0 ; count < until; count++)
            {
               s_char_s(*result,pos++) = s_char_s(source,count+beg);
            }

#endif
      }

      if ( holder != S_NULL)
      {
         sint  rc2 = 0;
         rc2 = s_drop(&holder);


         if ( rc == 0 )
         {
            rc = rc2;
         }
      }

   }
   return(rc);
}
