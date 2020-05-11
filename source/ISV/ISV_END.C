/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_end()                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  TM_master->ISV byte* IN/OUT   The ISV root pointer     |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Removes all ISV tables from memory, intended for use   |*/
/*|            during system shutdown.                                |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or a standard ISV error RC                |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    22_Jun_88   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv_i.h"
#include "isv.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"

sint isv_end()

{
   sint     rcode    =  0        ;     /* The RC from isv_rem()        */
   #D_begin('isv_end','isv')

   /*+----------------------------------------------------------------+*/
   /*| We loop around until isv_rem() complains that we're removing   |*/
   /*| DEFAULT ISV, which is normally unloadable.                     |*/
   /*+----------------------------------------------------------------+*/
   do
   {
      rcode = isv_rem(S_NULL);
   }
   while ( ! ISV_BAD_RC(rcode) );

   /*+----------------------------------------------------------------+*/
   /*| Make sure that isv_rem() got upset about us trying to unload   |*/
   /*| DEFAULT ISV--maybe some other error occurred.                  |*/
   /*+----------------------------------------------------------------+*/
   if ( rcode != ISV_DIC )
   {
      #D_leave(rcode,'Bad RC from isv_rem()');
   }

   /*+----------------------------------------------------------------+*/
   /*| OK, just chuck that default!                                   |*/
   /*+----------------------------------------------------------------+*/

   os_memd(TM_master->ISV);

   TM_master->ISV = NIL;

   #D_leave(0,'discarded DEFAULT ISV');
}

