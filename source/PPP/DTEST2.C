/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     DTEST C                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Verify that debugging routines or PPP is working.      |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     17_Jun_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "teies.h"
extern printf();
extern exit();
main()
{
   #D_main('dtest2','debug');


   printf("****************************************Hello from DTEST!\n");
   D_init("DTEST2A HASHDATA *","DTEST2A RROUT","DTEST2(A)");
   printf("****************************************Hello from DTEST!\n");

   #D_exit(0);

}



