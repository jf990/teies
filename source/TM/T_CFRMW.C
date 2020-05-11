/********************************************************************
:NAME.             T_CFRMW.c

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




sint     t_cfrmwa ( server_id, event )

string              server_id;
hword               event;



{


sint   rc      = 0,
       state   = 0,
       watchdog= 0;
hword  command = 0,
       flags   = 0;
string request = S_NULL,
       result  = S_NULL;





  while( t_recv( server_id, &command, &flags, &state, &request)  !=  0 )
  {

     os_wait( 1 );
     if( ++watchdog > WATCHDOG_LIMIT )
     {

        rc = Connection_Timeout;
        break;

     }
  }


  if( command > 0 )
  {

     rc = t_do( server_id, command, flags, state, request, & result );

  }


   return(rc);

}
