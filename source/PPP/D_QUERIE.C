/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_QUERIES C                                            |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_master   bool       IN   If FALSE, return FALSE      |*/
/*| :GLOBAL.   D_htable   D_htt(::)  IN   Debugging hash value table  |*/
/*| :GLOBAL.   D_flags.   D_FLAGS    IN   Debugging debugging table   |*/
/*| :GLOBAL.   D_ALLHVAL  PPC#define IN   MAGIC NUMBER--hashed value  |*/
/*|                                       of string "D9ALL"           |*/
/*|                                                                   |*/
/*| :PARAM.    var1       *byte      IN   String of switch #1         |*/
/*| :PARAM.    hval1      hword      IN   Hash value of var1          |*/
/*| :PARAM.    var2       *byte      IN   String of switch #2         |*/
/*| :PARAM.    hval2      hword      IN   Hash value of var2          |*/
/*| :PARAM.    var3       *byte      IN   String of switch #3         |*/
/*| :PARAM.    hval3      hword      IN   Hash value of var3          |*/
/*| :PARAM.    var4       *byte      IN   String of switch #4         |*/
/*| :PARAM.    hval4      hword      IN   Hash value of var4          |*/
/*| :PARAM.    ident      *byte      IN   Identity string from caller |*/
/*|                                                                   |*/
/*| :PURPOSE.  Performs multiple queries to see if a particular type  |*/
/*|            of debugging is on.  For example, to see if type 0     |*/
/*|            (basic) debugging is on for routine "NAME" of level    |*/
/*|            "LEV", then this routine should be passed the          |*/
/*|            values "0NAME", "2NAME", "3LEV", and "5LEV" as the     |*/
/*|            varN arguments and the corresponding hashed values.    |*/
/*|                                                                   |*/
/*|            This routine also checks to see if "D9ALL" is set.     |*/
/*|            For efficiency, the hashed value of this string is     |*/
/*|            provided in a #defined variable set in DEBUG_I.H       |*/
/*|                                                                   |*/
/*|            The regular boolean variable "D_master" is also        |*/
/*|            checked; if FALSE then FALSE is immediately returned.  |*/
/*|                                                                   |*/
/*| :OUTPUT.   output                                                 |*/
/*|                                                                   |*/
/*| :CALLS.    calls                                                  |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     23_Jun_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     13_Jul_88   K.J.Walsh   Added "D_master" feature.      |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"

boolean D_queries(var1,hval1,var2,hval2,var3,hval3,var4,hval4,ident)
byte     *var1,*var2,*var3,*var4;      /* Character string versions    */
hword    hval1,hval2,hval3,hval4;      /* Already hashed switchs       */
byte     *ident;                       /* Caller identity string       */
{
   bool  rcode =  TRUE  ;              /* Presume we return TRUE       */

   if ( D_INT_SW(D_ok_use) == FALSE )
   {
      rcode = FALSE;
   }
   else
   {

      if ( D_query(var1,hval1,ident) == FALSE )
      {
         if ( D_query(var2,hval2,ident) == FALSE )
         {
            if ( D_query(var3,hval3,ident) == FALSE )
            {
               if ( D_query(var4,hval4,ident) == FALSE )
               {
                  if ( D_query("9ALL",D_ALLHVAL,ident) == FALSE )
                  {
                     rcode = FALSE;
                  }
               }
            }
         }
      }
   }

   #if (NOT PROD_SYS)
      if ( D_SWITCH(D2QUERIE) || D_SWITCH(D5DEBUG) )
      {
         D_pr("!!!!:%s:D_queries(A):  RETURN(%d)\n",ident,rcode);
      }
   #endif

   return(rcode);
}
