/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_UTIL C                                               |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    todo       sint       IN   Indicates what to do.       |*/
/*| :PARAM.    bool_in    bool       IN   Input boolean               |*/
/*| :PARAM.    sint_in    sint       IN   Input sint                  |*/
/*| :PARAM.    cstr_in    byte*      IN   Input c string              |*/
/*| :PARAM.    id         byte*      IN   Caller identification       |*/
/*|                                                                   |*/
/*| :PURPOSE.  Alters and queries internal debugging variables or     |*/
/*|            other misc operations.                                 |*/
/*|                                                                   |*/
/*|            Depending on the value of "todo" it can:               |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|            D_UTIL_SET_MASTER - Change the debugging MASTER SWITCH |*/
/*|               setting according to 'bool_in';                     |*/
/*|                                                                   |*/
/*|            D_UTIL_TRACEBACK - Calls the TEIES exception handler to|*/
/*|               cause a traceback.  This way PPPMACs can get        |*/
/*|               tracebacks done by calling except() without having  |*/
/*|               the functions containing the call being dependant   |*/
/*|               on the except header files.  Variable 'sint_in' is  |*/
/*|               used as the error code to except().                 |*/
/*|                                                                   |*/
/*|            D_UTIL_READY - Indicate how ready the debugging        |*/
/*|               internals are.                                      |*/
/*|                                                                   |*/
/*| :OUTPUT.   D_UTIL_SET_MASTER:  Always zero;                       |*/
/*|            D_UTIL_TRACEBAK:  rcode from except() returned;        |*/
/*|            D_UTIL_READY:  0-> ready; 1-> table off; 2-> no table; |*/
/*|                                                                   |*/
/*| :CALLS.    calls                                                  |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.   Being developed as needed.                             |*/
/*|                                                                   |*/
/*| :MODS.     22_Aug_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"
#include "str.h"
#include "exc.h"
sint D_util(sint todo,bool bool_in,sint sint_in,char *cstr_in,byte *id)
{
   sint  rcode =  0  ;                 /* Value to be returned         */

   #if (NOT PROD_SYS)
      if (D_SWITCH(D2UTIL) || D_SWITCH(D5DEBUG))
         D_pr("!!!!:%s:D_util(A):  todo=%d,bool=%s,sint=%d,cstr=(%s).\n",
            id,todo,
            D_BOOLSTR(bool_in != FALSE),
            sint_in,cstr_in);
   #endif

   switch(todo)
   {

      /*+-------------------------------------------------------------+*/
      /*| Change the debugging MASTER SWITCH setting.                 |*/
      /*+-------------------------------------------------------------+*/
      case D_UTIL_SET_MASTER:
         #if (NOT PROD_SYS)
            if (D_SWITCH(D2UTIL) || D_SWITCH(D5DEBUG))
              D_pr("!!!!:%s:D_util(B):  Setting master from %s to %s.\n",
                  id,
                  D_BOOLSTR(D_SWITCH(D_ok_use)),
                  D_BOOLSTR(bool_in));
         #endif
         /*+----------------------------------------------------------+*/
         /*| The following insures that (1) a boolean value is used   |*/
         /*| and (2) switch is turned on (TRUE) if a non-boolean value|*/
         /*| manages to find it's way in here.                        |*/
         /*+----------------------------------------------------------+*/
         D_INT_SW(D_ok_use) = (bool_in == FALSE   ? FALSE : TRUE );
         rcode = 0;
         break;

      /*+-------------------------------------------------------------+*/
      /*| Call the TEIES exception handler.  Maybe.                   |*/
      /*+-------------------------------------------------------------+*/
      case D_UTIL_TRACEBACK:

         #if (NOT PROD_SYS)
            if (D_SWITCH(D2UTIL) || D_SWITCH(D5DEBUG))
               D_pr("!!!!:%s:D_util(C):  Calling except() with error \
                  code %d to RESET_PROCESS for TRACEBACK.\n",id,sint_in);
         #endif

         rcode = except(sint_in,RESET_PROCESS,
            S_NUL,S_NUL,S_NUL,S_NUL,S_NUL);

         break;

      /*+-------------------------------------------------------------+*/
      /*| See if the debugging table is loaded and running.           |*/
      /*+-------------------------------------------------------------+*/
      case D_UTIL_READY:

         #if (NOT PROD_SYS)
            if (D_SWITCH(D2UTIL) || D_SWITCH(D5DEBUG))
               D_pr("!!!!:%s:D_util(D):  Ready Query.\n",id);
         #endif

         if ( D_INT_SW(D_ok_run) == FALSE )
            rcode = 2;
         else if ( D_INT_SW(D_ok_use) == FALSE )
            rcode = 1;
         else
            rcode = 0;

         break;

      /*+-------------------------------------------------------------+*/
      /*| Don't know what was meant by that!                          |*/
      /*+-------------------------------------------------------------+*/
      default:
         #if (NOT PROD_SYS)
            if (D_SWITCH(D2UTIL) || D_SWITCH(D5DEBUG))
               D_pr("!!!!:%s:D_util(E):  Invalid paramter ignored.\n",
                  id);
         #endif
         rcode = -1;
   }
   return(rcode);
}
