/******************************************************************
   <DOC>

   <NAME>      ski_prse.c

   <LEVEL>     SKI

   <AUTHOR>    Pamela Cham

   <DATE>      May 24, 1989

   <PARAMS>    target          string pointer    INOUT
               primary_key     string pointer    OUTPUT
               secondary_key   string pointer    OUTPUT

   <PURPOSE>   This routine parses the given string target to
               locate the primary_key and secondary_key strings,
               which appear in front of target.  Target is given
               back with the remainder of the input string.
               A string format of /word/ specifies a single keyword
               which is assigned to primary_key.  /.word/ specifies
               only a secondary_key.  /word./ specifies only a
               primary_key, and means the same as the /word/ string
               format.  Lastly, /word.word/ specifies both the
               primary_key and secondary_key.

   <OUTPUT>    sint rc

   <ERRS>      SKI_INVALID_STRING            28331
               SKI_IMPROPER_KEYWORD_FORMAT   28332

   </DOC>

******************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"

sint ski_prse (target, primary_key, secondary_key)
string *target;
string *primary_key, *secondary_key;
{
   sint strlength=0;                   /* length of target             */
   sint delimpos=0;                    /* position of delimiting slash */
   sint dotpos=0;                      /* position of dot, if any      */
   sint keyend=0;                      /* ending pos of first key in   */
                                       /*   target                     */
   string strbuff=S_NULL;              /* temp string buffer           */
   string front_key=S_NULL;            /* stores first key string from */
                                       /*   target                     */
   string dot=S_NULL;
   string slash=S_NULL;
   sint rc=0;


   s_init (&slash, "/");
   s_init (&dot, ".");

   if (target != NIL && *target != S_NULL)
   {
      strlength = s_len(*target);
      s_upper (*target, 1, strlength); /* standardize : convert key    */
                                       /*   to upper case              */

/* ------------------------------------------------------------------- */
/* Find the position of first delimiting slash in target to extract    */
/* the key string in front of target.                                  */
/* ------------------------------------------------------------------- */
      delimpos = s_first(slash, *target, 2, strlength);
      keyend = delimpos-1;
      if (delimpos==0)                 /* no delimiting slash found    */
      {
         rc = SKI_IMPROPER_KEYWORD_FORMAT;
      }
      else                             /* delimiting slash found       */
      {                                /* extract first key enclosed   */
         s_copy (&front_key, *target, 2, keyend-1);

/* ------------------------------------------------------------------- */
/* store remaining portion of key string in target                     */
/* ------------------------------------------------------------------- */
         s_clip (&strbuff, target, 1, keyend);
         if (s_len(*target) < 2)
            s_nulout (target);

/* ------------------------------------------------------------------- */
/* Process extracted key string to obtain the primary and/or secondary */
/* keys                                                                */
/* ------------------------------------------------------------------- */
         keyend = keyend-1;
         dotpos = s_first(dot, front_key, 1, keyend);
         if (dotpos==0 || dotpos==keyend)
                                       /* just primary key was given   */
                                       /* /word/ or /word./ format     */
         {
            s_nulout (secondary_key);
            if (dotpos==0)             /* /word/ format                */
               s_copy (primary_key, front_key, 1, keyend);
            else                       /* /word./ format               */
               s_copy (primary_key, front_key, 1, keyend-1);
         }
         else if (dotpos==1)           /* just secondary key was given */
                                       /* /.word/ format               */
         {
            s_nulout (primary_key);
            s_copy (secondary_key, front_key, 2, keyend-1);
         }
         else                         /* both primary and secondary    */
                                      /*   keys were specified         */
                                      /* /word.word/ format            */
         {                            /* extract each of these         */
            s_clip (primary_key, &front_key, 1, dotpos-1);
            s_clip (&strbuff, &front_key, 1, 1);
            s_copy (secondary_key, front_key, 1, 0);
         }
         s_drop (&strbuff);
         s_drop (&front_key);
      }
   }
   else
   {
      rc = SKI_INVALID_STRING;
   }
   return(rc);
}
