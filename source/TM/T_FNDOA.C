/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    T_FNDOA.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 17, 1987                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. Allocate and initialize the OAMT's (Object Action      |
  |            Matrix Table) for all compiled code.                   |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
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



sint   t_fndoa( )


{


#D_begin(  t_fndoa,  TM);

    TM_master->actions = os_mema( sizeof( OAMT ) * NUMBER_OF_ACTIONS );

#D_return( 0 );

}
