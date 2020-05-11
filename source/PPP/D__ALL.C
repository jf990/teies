/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D__all.c                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   type        int      INPUT    Permitted types:         |*/
/*|                                          0,1,'0','1','2','3','4', |*/
/*|                                          '5','6','7','8','9','*', |*/
/*|                                          and '!'.  Note that the  |*/
/*|                                          numbers 0 and 1 have     |*/
/*|                                          different meanings than  |*/
/*|                                          the characters '0' and   |*/
/*|                                          '1'                      |*/
/*|  :PARAM.   name        char*    INPUT    name of calling routine  |*/
/*|                                          MUST BE PREFIXED WITH    |*/
/*|                                          TWO BLANKS               |*/
/*|  :PARAM.   level       char*    INPUT    level of calling routine |*/
/*|                                          MUST BE PREFIXED WITH    |*/
/*|                                          TWO BLANKS               |*/
/*|  :PARAM.   ident       char*    INPUT    identifier for errors    |*/
/*|                                                                   |*/
/*|  :PURPOSE. Debugs the debugging--turns on almost all switchs.     |*/
/*|            Do not include in regular TEIES TXTLIBs.               |*/
/*|                                                                   |*/
/*|  :OUTPUT.  return TRUE or FALSE if a particular switch or set     |*/
/*|            on or off; FALSE if error detected during fetch.       |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  Generates mundo-jumbo output.                          |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"

boolean D_queries(var1,hval1,var2,hval2,var3,hval3,var4,hval4,ident)
char     *var1,*var2,*var3,*var4;      /* Character string versions    */
hword    hval1,hval2,hval3,hval4;      /* Already hashed switchs       */
char     *ident;                       /* Caller identity string       */
{

   return(TRUE);
}
