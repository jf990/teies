/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.             T_WAIT.C

:LEVEL.            TM

:AUTHOR.           John L. Foster

:DATE.             November 23, 1987

:INPUT.            * none *

:OUTPUT.           rc   :

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
                   Wait for a response over ISCF.  In the future, que
                   this action and go do something else while waiting.
:ERRORS.

:CALLS. isf_open(), except(), isf_send()

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "isf.h"
#include "obj.h"
#include "tm.h"



sint     t_wait( )


{


sint     rc        =      0,
         flags     =      0,
         watchdog  =      0,
         state     =      0,
         status    =      0;
string   request   = S_NULL,
         u_id      = S_NULL;
hword    command   =      0,
         tflag     =      0;
boolean  responded =   TRUE;







   s_init( & request, " blah   blah   blah   " );
   while( isf_qry ( S_NULL, flags, &(status), &(u_id) ))
   {

      if( watchdog++ > WATCHDOG_LIMIT )
      {

         rc = Connection_Timeout ;
         except( rc, CONTROL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                    S_NULL                         );
         responded = FALSE;
         break;

      }
      os_wait( 1 );

   }
   if( responded )
   {


      switch( status )
      {

         case NEW_CONNECTION : t_iscf_t( u_id, TM_NETWORK_CONNECTION );
                               break;

         case NEW_MESSAGE    : t_recv( u_id, & command, & tflag,
                                             & state,   & request      );
                               t_do(   u_id,   command,   tflag,  state,
                                               request,   NIL          );
                               s_drop( & request );
                               break;

         default             : break;

      }
      status = 0;
      s_drop( & u_id );

   }

s_drop( & request );


   return(rc);

}
