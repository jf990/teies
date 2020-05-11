/*+------------------------------------------------------------------+*/
/*|                                                                  |*/
/*|  :NAME.      odt_dini ()                                         |*/
/*|                                                                  |*/
/*|  :LEVEL.     OBJ                                                 |*/
/*|                                                                  |*/
/*|  :GLOBAL.    odt_t_r, TM_master->odt_t_r                         |*/
/*|                                                                  |*/
/*|  :PARAM.                                                         |*/
/*|                                                                  |*/
/*|  :PURPOSE.   To deallocate odt_t_r for use with odt_make.        |*/
/*|                                                                  |*/
/*|  :OUTPUT.    odt_t_r                                             |*/
/*|                                                                  |*/
/*|  :CALLS.     s_drop                                              |*/
/*|                                                                  |*/
/*|  :ERRORS.                                                        |*/
/*|                                                                  |*/
/*|  :LIMITS.                                                        |*/
/*|                                                                  |*/
/*|  :MODS.     22 Jun 88    John L. Foster                          |*/
/*|                                                                  |*/
/*|  :END.                                                           |*/
/*|                                                                  |*/
/*+------------------------------------------------------------------+*/

#include "defs.h"
#include "isv.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"



sint odt_dini(  )

{


   sint cntr = 0;







   if( TM_master->odt_t_r != NIL )
   {


   }
   for (  cntr=0;  cntr < ODT_TYPES+1;  cntr++  )
   {

      s_drop(  &( odt_t_r [cntr].name )   );

   }
   TM_master->odt_t_r = NIL;


   return(0);

}
