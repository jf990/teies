/*+--------------------------------------------------------------+*/
/*|                                                              |*/
/*|  :NAME.    s_rep                                             |*/
/*|                                                              |*/
/*|  :AUTHOR.  Ing-Long Wu                                       |*/
/*|                                                              |*/
/*|  :DATE.    March 10, 1988                                    |*/
/*|                                                              |*/
/*|  :LEVEL.   str                                               |*/
/*|                                                              |*/
/*|  :PARAM.   sous1, sous2              the strings INPUT       |*/
/*|                                                              |*/
/*|  :PURPOSE. replace sous1 string indicated B and L by sous2   |*/
/*|            string indicated B and L to create the target     |*/
/*|            composition string.                               |*/
/*|                                                              |*/
/*|  :OUTPUT.  return the target string                          |*/
/*|                                                              |*/
/*|  :CALLS.   s-copy, s_cat, s_len                              |*/
/*|                                                              |*/
/*|  :MODS.    06_23_88   A.K.Mehta   Updated to check condition |*/
/*|                                   before copying first part  |*/
/*|                                   of sous1                   |*/
/*|  :END.                                                       |*/
/*|                                                              |*/
/*+--------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"
#include "osk.h"
#include "str_x.h"

sint s_rep (targ, sous1, beg1, len1, sous2, beg2, len2)

string    *targ;              /* string to be created        */
string    sous1, sous2;       /* strings to be replaced      */
sint      beg1, len1;         /* beg. and len. for sous1 string */
sint      beg2, len2;         /* beg. and len. for sous2 string */

{
   sint   ul_sous1 = 0;      /* used length of sous1           */
   sint   ul_sous2 = 0;      /* used length of sous2           */
   sint   ul_res   = 0;      /* used length of resulting string */
   sint   rc       = 0;      /* return code                     */
   sint   rc1      = 0;      /* return code                     */
   string res      = S_NULL; /* resulting string                */
   string temps1   = S_NULL; /* temporary usage for operation   */
   string temps2   = S_NULL; /* temporary usage for operation   */



/*+-----------------------------------------------------------------+*/
/*|  use the standard procedures to check the length                |*/
/*+-----------------------------------------------------------------+*/

   if ((len1 < 0) || (len2 < 0))
     {
   return(S_BAD_LEN);
     }

   ul_sous1 = s_len(sous1);       /* get used length of sous1       */
{
   if ( beg1 == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len1 == 0 || len1 > ul_sous1 )       /* also OK if LEN is too long   */
      {
         len1 = ul_sous1;
      }
   }
   else if ( beg1 > 0 )                  /* When B is positive...        */
   {
      beg1--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len1 == 0 )
      {                                /* LEN zero indicates entire str*/
         len1 = ul_sous1;
      }
      if ( beg1 > ul_sous1 )                  /* BEG past last byte           */
      {
         len1 = 0;
      }
      else if ( beg1 + len1 > ul_sous1 )        /* range bleeds right           */
      {
         len1 = ul_sous1 - beg1;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg1 = ul_sous1 + beg1;
      if ( beg1 >= 0 )
      {
         if ( len1 == 0 )
         {                             /* LEN zero indicates entire str*/
            len1 = ul_sous1;
         }
         if ( beg1 > ul_sous1 )               /* BEG past last byte           */
         {
            len1 = 0;
         }
         else if ( beg1 + len1 > ul_sous1 )     /* range bleeds right           */
         {
            len1 = ul_sous1 - beg1;             /* Cut length                   */
         }
      }
      else if ( len1 == 0 )              /* Default says use it all      */
      {
         beg1 = 0;
         len1 = ul_sous1;
      }
      else if ( beg1 + len1 < 0 )          /* Completely out of range      */
      {
         len1 = 0;
      }
      else
      {                                /* Range bleeds left            */
         len1 = len1 + beg1;                 /* NB -- adding a negative      */
         beg1 = 0;
      }
   }
}
   if (len1 == 0)
     {
   return(S_NUL_STR);
     }

   ul_sous2 = s_len(sous2);        /* get used length of sous2 */
{
   if ( beg2 == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len2 == 0 || len2 > ul_sous2 )       /* also OK if LEN is too long   */
      {
         len2 = ul_sous2;
      }
   }
   else if ( beg2 > 0 )                  /* When B is positive...        */
   {
      beg2--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len2 == 0 )
      {                                /* LEN zero indicates entire str*/
         len2 = ul_sous2;
      }
      if ( beg2 > ul_sous2 )                  /* BEG past last byte           */
      {
         len2 = 0;
      }
      else if ( beg2 + len2 > ul_sous2 )        /* range bleeds right           */
      {
         len2 = ul_sous2 - beg2;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg2 = ul_sous2 + beg2;
      if ( beg2 >= 0 )
      {
         if ( len2 == 0 )
         {                             /* LEN zero indicates entire str*/
            len2 = ul_sous2;
         }
         if ( beg2 > ul_sous2 )               /* BEG past last byte           */
         {
            len2 = 0;
         }
         else if ( beg2 + len2 > ul_sous2 )     /* range bleeds right           */
         {
            len2 = ul_sous2 - beg2;             /* Cut length                   */
         }
      }
      else if ( len2 == 0 )              /* Default says use it all      */
      {
         beg2 = 0;
         len2 = ul_sous2;
      }
      else if ( beg2 + len2 < 0 )          /* Completely out of range      */
      {
         len2 = 0;
      }
      else
      {                                /* Range bleeds left            */
         len2 = len2 + beg2;                 /* NB -- adding a negative      */
         beg2 = 0;
      }
   }
}
   if (len2 == 0)
     {
   return(S_NUL_STR);
     }


/*+----------------------------------------------------------------+*/
/* calculate the length of resulting string and allocate the string */
/* based on the length which we just calculate                      */
/*+----------------------------------------------------------------+*/

   ul_res = ul_sous1 - len1 + len2;  /* get the len. of res. string */
   rc = s_alloc(&res, ul_res);       /* allocate the res. string    */


/*+----------------------------------------------------------------+*/
/*| replace sous1 string indicated, beg1 and len1, by sous2 string |*/
/*| indicated, beg2 and len2, to create the resulting string       |*/
/*+----------------------------------------------------------------+*/

   if ( beg1 != 0)
   {
   rc=s_copy(&temps1, sous1, 1, beg1);/*copy the first part of sous1*/
   }

   /*--------------------------------------------------------------*/
   /* append the part of sous2 string indicated by beg2 and len2   */
   /* into the first part of sous1 string indicated by 1 and beg1  */
   /*--------------------------------------------------------------*/
   s_cat(&temps2,temps1, sous2, beg2+1, len2);

   /*--------------------------------------------------------------*/
   /* append the rest of sous1 string  into resulting string       */
   /*--------------------------------------------------------------*/
   rc = s_cat(&res,temps2, sous1, beg1+len1+1, 0);


   rc = s_drop(targ);    /* completely deallocate targ string      */

   *targ = res;  /* assign res string to targ string which we want */
   return(0);

}
