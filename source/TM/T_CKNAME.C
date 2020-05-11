/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     T_CKNAME C                                             |*/
/*| <AUTHOR>   Kevin Walsh                                            |*/
/*| <LEVEL>    TM                                                     |*/
/*| <PARAM>    name       string     IN   Name to be validated.       |*/
/*|                                                                   |*/
/*| <PARAM>    level      hword      IN   Type of check requested.    |*/
/*|                                                                   |*/
/*| <PARAM>    ind        *sint      IN   Indicates where bad part of |*/
/*|                                       input name is; 0=entire name|*/
/*|                                       bad; POS num=where bad char |*/
/*|                                       is located, 1=1st.          |*/
/*|                                                                   |*/
/*| <PURPOSE>  Checks to see if the given name is valid.              |*/
/*|                                                                   |*/
/*| <OUTPUT>   Zero=OK, otherwise TM error indicating reason for      |*/
/*|            rejection, else other standard error.                  |*/
/*| <LIMITS>   What characters are not permitted in a name are        |*/
/*|            defined in code here.                                  |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "isv.h"
#include "osk.h"
#include "tm.h"
#include "debug.h"

#define CHARACTERS_NOT_ALLOWED_IN_NAME       ", ;\""

sint t_ckname(string name, hword level, sint *ind)
{

   sint     rc    =     0;             /* Presume no error             */
   sint     dummy =     0;             /* Temporary variable           */
   string   temp  =     S_NULL;        /* Characters not allowed.      */




   /*+----------------------------------------------------------------+*/
   /*| First, we look for a bad character anywhere in the string.     |*/
   /*+----------------------------------------------------------------+*/
   rc = s_type(name,0,0,'P');          /* Is it all printable?         */
   if ( rc == S_NULL_STR )
   {
      rc = TM_BAD_NAME;
      *ind = 0;
   }
   else if ( rc > 0 )                  /* Not all printable--ERROR     */
   {
      *ind = rc;
      rc = TM_NONPRINTABLE_CHAR_IN_NAME;
   }
   else if ( rc < 0 )
   {
      *ind = -1;
   }
   else                                /* All printable--anything else?*/
   {
      if( level != T_CKNAME_NONPRINTABLE_ONLY )
      {

         s_init(&temp,CHARACTERS_NOT_ALLOWED_IN_NAME);
         rc = s_verify(name,temp,0,0,S_DISALLOW);
         s_drop(&temp);
         if ( rc > 0 )                 /* Printable bad char--ERROR    */
         {
            *ind = rc;
            rc = TM_BAD_CHAR_IN_NAME;
         }
         else if ( rc < 0 )
         {
            *ind = -1;
         }
         else
         {

            temp = S_NULL;
            rc = isv_get(&temp,ISV_LAST,0);
            if ( rc != 0 )
            {
               *ind = -1;
            }
            else
            {
               rc = s_comp(temp,name,0,0,'F',&dummy);
               if ( rc == 0 )
               {
                  *ind = 0;
                  rc = TM_BAD_NAME;
               }
               else if ( rc < 0 )
               {
                  *ind = -1;
               }
               else
               {
                  temp = S_NULL;
                  rc = isv_get(&temp,ISV_ALL,0);
                  if ( rc != 0 )
                  {
                     *ind = -1;
                  }
                  else
                  {
                     rc = s_comp(temp,name,0,0,'F',&dummy);
                     if ( rc == 0 )
                     {
                        *ind = 0;
                        rc = TM_BAD_NAME;
                     }
                     else if ( rc < 0 )
                     {
                        *ind = -1;
                     }
                     else
                     {
                        rc = 0;
                        if ( s_len(name) == 0 )
                        {
                           rc = TM_BAD_NAME;
                        }
                     }
                  } /* End of section that checks ISV_ALL */
               } /* End of section that checks ISV_LAST */
            } /* End of section that checks ISV_LAST isv_get() rc */
         } /* End of section that checks s_verify() rc */
      }
   } /* End of section that checks for disallowed characters */

   return(rc);
}
