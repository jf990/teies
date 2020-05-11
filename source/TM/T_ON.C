/********************************************************************
:NAME.                 T_ON.C

:LEVEL.                TM

:AUTHOR.               John L. Foster

:DATE.                 June 12, 1987

:INPUT.

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
    Determine, via calling ISCF, who is currently attached to
    the Master Server.
:CALLS.

:ERRORS.

:LIMITS.

:MODS.
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"




sint     t_on(  )


{


sint   rc      = 0,
       level   = 0,
       state   = 0;
string request = S_NULL;


#D_begin ( t_on , TM )


rc = isf_open( TM_master->MS, level );
if( rc ==  25311 )
{

   except( 25311, CONTROL_ERROR, TM_master->MS, S_NULL, S_NULL,
                                                S_NULL, S_NULL );
   #D_leave( 25311, 'Cannot find a master server online.'      );

}
s_init( &request, "*" );

rc = t_send( TM_master->MS, TM_ON, NO_FLAGS_SET, state, request );

s_drop( &request );
#D_return( 0 )

}
