/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_chkprv.c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 26, 1988                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   privilege   PRIV *    INPUT   Privilege map to check.  |
  |                                                                   |
  |  :PARAM.   role        sint      INPUT   Role to check if in the  |
  |                                          privilege map.           |
  |                                                                   |
  |  :PURPOSE. Check to see if the role requested is in the privilege |
  |            set passed.                                            |
  |                                                                   |
  |  :OUTPUT.  0 if privilege allowed.                (sint)          |
  |            else highest value privilege available.                |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"
#include "vars.h"



sint   t_chkprv (  privilege, role  )

PRIVS             *privilege;
sint               role;


{


#D_begin(  t_chkprv,  TM  )
#D_mdump( 'privilege', 'sizeof(PRIVS)' )
#D_show(  'Role to check - %d(x%x).', 'role,role' )


#D_say( 'Dummy version - privilege OK. ' )
#D_return( 0 );

}
