/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_init()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ISV byte  IN/OUT root ISV tables pointer    |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Loads the default ISV table for the entire system.     |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a standard ISV return code indicating status   |*/
/*|                                                                   |*/
/*|  :CALLS.   isv_add()                                              |*/
/*|                                                                   |*/
/*|  :ERRORS.  ISV_WAR                                                |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    04_Apr_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42;             |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"

extern   sint  isv_add  () ;

sint isv_init()

{

   sint     src      ;     /* Return code from TEIES stuff (signed)    */
   sint     add_rc   ;     /* Return code from TEIES isv_add()         */
   string   fd_str = S_NUL;/* Name of default ISV file


   if ( TM_master->ISV != NIL )
   {
   return(ISV_WAR);
   }

   s_init(&fd_str,"* * DEFAULT ISV$OBJ");

   add_rc = isv_add(fd_str);           /* suprise!                     */

   s_drop( & fd_str );

   return(add_rc);
}
