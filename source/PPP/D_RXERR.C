/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_rxerr()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   rc          int      INPUT    RC from REXX operation   |*/
/*|  :PARAM.   str         char*    INPUT    string from REXX (if any)|*/
/*|  :PARAM.   ident       char*    INPUT    identification of caller |*/
/*|  :PARAM.   varname     char*    INPUT    name of REXX var in quest|*/
/*|                                                                   |*/
/*|  :PURPOSE. Centralizes interpretation of failed REXX set/fetch    |*/
/*|            operations.                                            |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns                                                |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  This version can only handle REXX fetch operations.    |*/
/*|                                                                   |*/
/*|  :MODS.    05_Feb_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Moved from debugi.h            |*/
/*|                                    to STR txtlib                  |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "rexxdef.h"
#include "defs.h"
#include "debug.h"
extern byte *D_gin();
sint D_rxerr ( rc , str, ident, varname )

   int   rc        ;  /* The return code from the REXX function */
   char  *str      ;  /* The return string (if any)             */
   char  *ident    ;  /* Identification string for output       */
   char  *varname  ;  /* Name of the REXX variable acted upon   */
{
#if D_OSIDEBUG
   D_pr("####:%s:  (%d) returned getting {%s}, value of {%s}.\n",
         ident,rc,str,varname);
#endif
switch ( rc )
{
   case SHVCLEAN:                      /* From "rexxdef.h"             */
        if ( *str == '0')
           D_pr("%s%s:ERROR:  Called D_rxerr() even though all OK.\n",
               D_gin(0,-99),ident);
        else if ( *str == '1')
           D_pr("%s%s:ERROR:  Call to D_rxerr() even though all OK.\n",
               D_gin(0,-99),ident);
        else
           D_pr("%s%s:WARNING:  \"%s\" set to \"%s\" by REXX caller.\n",
              D_gin(0,-99),ident,varname,str);
        break;

#if 0
   case SHVNEWV:                       /* From "rexxdef.h"             */
        D_pr("%s%s:WARNING:  \"%s\" was uninitialized.\n",
              D_gin(0,-99),ident,varname);
        break;
#endif

   case SHVTRUNC:                      /* From "rexxdef.h"             */
        D_pr("%s%s:WARNING:  Value of \"%s\" exceeded %d chars.\n",
            D_gin(0,-99),ident,varname,D_SLEN);
        break;

   case SHVBADN:                       /* From "rexxdef.h"             */
        D_pr("%s%s:WARNING:  \"%s\" is not a valid REXX variable.\n",
            D_gin(0,-99),ident,varname);
        break;

   case -1:
        D_pr("%s%s:ERROR:  Error occurred during REXX interface.\n",
             D_gin(0,-99),ident);
        break;

   case -3:
        D_pr("%s%s:ERROR:  Not called as a REXX function.\n",
             D_gin(0,-99),ident);
        break;

   default:
        D_pr("%s%s:ERROR:  Unexpected value %d returned by rexxfetch.\n",
             D_gin(0,-99),ident,rc);
        break;
}
return(0);
}

