/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_LOOKUP C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_DEBUGHASH    #define   IN   Debugging switch.        |*/
/*|                                                                   |*/
/*| :PARAM.    buffer         byte*     IN   Variable to be checked.  |*/
/*| :PARAM.    id             byte*     IN   String identifying caller|*/
/*|                                                                   |*/
/*| :PURPOSE.  Does a query of a debugging switch given the string    |*/
/*|            name rather than the hash value of the switch.         |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns RC from D_query(), which will return TRUE if   |*/
/*|            the switch is set or FALSE if not found or not set.    |*/
/*|                                                                   |*/
/*| :CALLS.    D_hashit(), D_query()                                  |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     16_Jun_88   K.J.Walsh   Implemented, based on D_qry2(),|*/
/*|                                    written by Anoop K. Srivastava |*/
/*|                                    on June 3, 1988.               |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdio.h>
#include "debug_i.h"

bool D_lookup(buffer,id)
byte     *buffer;                      /* Input character to be checked*/
byte     *id;
{
   hword hvalue   =  0  ;              /* Calculated hash value        */
   bool rcode = FALSE;                 /* Expected return value        */

   /*+----------------------------------------------------------------+*/
   /*| Just in case a variable arrives without the the "D" stripped...|*/
   /*+----------------------------------------------------------------+*/
   if ( buffer(:0:) == 'D'
      & buffer(:1:) >= '0'
      & buffer(:1:) <= '9' )
   {
      buffer++;
      #if (NOT PROD_SYS)
         if ( D_SWITCH(D2LOOKUP) || D_SWITCH(D5DEBUG) )
            D_pr("!!!!:%s:D_lookup(A):  {D%s} shortened to {%s}.\n",
              id,buffer,buffer);
      #endif
   }

   /*+----------------------------------------------------------------+*/
   /*|  Calculate the hash value of the variable.                     |*/
   /*+----------------------------------------------------------------+*/
   hvalue = D_hashit(buffer,id);

   rcode = D_query(buffer,hvalue,id);

   #if (NOT PROD_SYS)
      if ( D_SWITCH(D2LOOKUP) || D_SWITCH(D5DEBUG) )
         D_pr("!!!!:%s:D_lookup(B):  RETURN(%d), checked {%s} (hv=%d)\n",
           id,rcode,buffer,hvalue);
   #endif

   return(rcode);

}
