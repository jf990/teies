/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Name:  os_gfe c                                                  |*/
/*|                                                                   |*/
/*|  Author:  Kevin J. Walsh                                          |*/
/*|                                                                   |*/
/*|  Date:  07 May 87                                                 |*/
/*|                                                                   |*/
/*|  Globals:  None                                                   |*/
/*|                                                                   |*/
/*|  Description:   Using a "given" file descriptor element and a     |*/
/*|      "default", it appends to the "result" string.  These rules   |*/
/*|      are used:                                                    |*/
/*|                                                                   |*/
/*|      1. If the "given" has usage length zero, then append the     |*/
/*|         "default";                                                |*/
/*|      2. If the "given" is "*", then append the "default";         |*/
/*|      3. If the "default" has usage length zero, then append a "*";|*/
/*|                                                                   |*/
/*|  Modifications:                                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


sint os_gfde(result,given,defalt)

string   *result  ;                    /* What we give 'em             */
string   given    ;                    /* What they gave us            */
string   defalt   ;                    /* What they might want         */

{
   string   star     = S_NULL ;        /* sometimes we need this       */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  First, see if the given string will do...                        |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   if ( s_smp_ul(given) > 1 )          /* Given is good enough         */
   {
      s_cat(result,*result,given,0,0);
   return(0);
   }

   if ( s_smp_ul(given) == 1 )         /* If not null string ...       */
   {
      if ( s_char_s(given,0) != '*' )  /* ...if not just a "*" ...     */
      {
         s_cat(result,*result,given,0,0);
   return(0);
      }
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  ...it won't, so use the default string (if it exists)...         |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
   if ( s_smp_ul(defalt) != 0 )
   {
      s_cat(result,*result,defalt,0,0);
   return(0);
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  ...it doesn't exist, so return a "*" instead.                    |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
   s_init(&star,"*");
   s_cat(result,*result,star,0,0);
   s_drop(&star);
   return(0);
}
