/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_setvar()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   toggle      byte*    INPUT    name of REXX variable to |*/
/*|                                          be set                   |*/
/*|  :PARAM.   value       byte*    INPUT    will usually be either   |*/
/*|                                          "0" or "1" to indicate   |*/
/*|                                          "on" or "off", but is    |*/
/*|                                          capable if settting to   |*/
/*|                                          any value.               |*/
/*|  :PARAM.   ident       byte*    INPUT    identifier for errors    |*/
/*|                                                                   |*/
/*|  :PURPOSE. Sets or adds a value to the debugging table.  It is    |*/
/*|            prefereed that D_hset() be used, as the call to it     |*/
/*|            (if resolved by PPP) can have the hash value already   |*/
/*|            computed.                                              |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.   D_hashit(), D_hset()                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  Switch name must be in upper case                      |*/
/*|                                                                   |*/
/*|  :LIMITS.  "value" should be boolean, not a string.               |*/
/*|                                                                   |*/
/*|  :MODS.    30_Jun_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    22_Jun_88   K.J.Walsh   Made into a dummy interface    |*/
/*|                                    to D_hset() for back compatab. |*/
/*|  :MODS.    23_Jun_88   K.J.Walsh   Made into permanent function.  |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"

void D_setvar(byte *toggle, bool value, byte *ident)
{
   bool setting   =  FALSE;           /* Resulting value               */

   /*+----------------------------------------------------------------+*/
   /*| It is intended that D_SETVAR be set with a boolean value, but  |*/
   /*| we'll process whatever we get.                                 |*/
   /*+----------------------------------------------------------------+*/

   switch(value)
   {

      case TRUE:
         #if (NOT PROD_SYS)
            if (D_SWITCH(D2SETVAR) || D_SWITCH(D5DEBUG))
              D_pr("!!!!:%s:D_setvar(A): {%s} to be set TRUE.\n",
                 ident,toggle);
         #endif
         setting = TRUE;
         break;

      case FALSE:
         #if (NOT PROD_SYS)
            if (D_SWITCH(D2SETVAR) || D_SWITCH(D5DEBUG))
              D_pr("!!!!:%s:D_setvar(B): {%s} to be set FALSE.\n",
                 ident,toggle);
         #endif
         /* setting = FALSE;  Initialized as FALSE */
         break;

      default:
         #if (NOT PROD_SYS)
            if (D_SWITCH(D2SETVAR) || D_SWITCH(D5DEBUG))
              D_pr("!!!!:%s:D_setvar(C): {%s} unknown value 0x%X, \
setting FALSE.\n",ident,toggle,value);
         #endif
         /* setting = FALSE;  Initialized as FALSE */
         break;
   }


   /*+----------------------------------------------------------------+*/
   /*| Just in case a variable arrives without the the "D" stripped...|*/
   /*+----------------------------------------------------------------+*/
   if ( toggle(:0:) == 'D'
      & toggle(:1:) >= '0'
      & toggle(:1:) <= '9' )
   {
      D_hset(toggle+1,  D_hashit(toggle+1,ident),  setting,ident);
   }
   else
   {
      D_hset(toggle  ,  D_hashit(toggle  ,ident),  setting,ident);
   }
}
