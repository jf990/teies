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
   #D_main('dtest','debug');

   #D_master(off);
   #D_say('This should not show because of D_master(off) call');
   #D_master(on);

   #D_show('about to call A',"");
   printf("Hello from DTEST!\n");
   A(5);
   #D_msgs('after calling A',"");

   #D_exit(0);

}



A(buf)
byte buf;
{
   char *str = "Hello from DTEST's subroutine \"A\"!\n";
   #pppwarn(OFF);
   #D_begin('A','stest');
   #pppwarn(ON);

   printf(str);

   #D_return(0);
}
