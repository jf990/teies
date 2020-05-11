/********************************************************************
:NAME.             T_CONFIRM.c

:LEVEL.            TM

:AUTHOR.           John L. Foster

:DATE.             November 29, 1987

:PARAM.            server    string    INPUT    Server who sent us the
                                                confirmation.

:OUTPUT.           rc        sint

:GLOBALS.          - none -

:PURPOSE.          Wait for a particular server to respond with a conf-
                   irmation command or another process request.

:CALLS.            t_do(), t_recv().

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




sint     t_confirm( server_id  )

string              server_id;

{


sint   rc      = 0,
       state   = 0,
       watchdog= 0;
hword  command = 0,
       flags   = 0;
string request = S_NULL,
       result  = S_NULL;


#D_begin ( t_confirm , TM )
#D_str( 'server_id','Server who sent the confirmation.' );



  while( t_recv( server_id, & command, & flags, & state, & request) != 0 )
  {

     #D_say( 'Waiting for confirmation from server. ' );
     os_wait( 1 );
     if( ++watchdog > WATCHDOG_LIMIT )
     {

        rc = Connection_Timeout ;
        break;

     }
  }

  #D_show('From MS: Cmd=%d, Flags=%d; State=%d.','command,flags,state');
  #D_str( 'request','data packet sent back from MS' );

  if( command > 0 )
  {

     rc = t_do( server_id, command, flags, state, request, & result );

  }


#D_return( rc )

}
