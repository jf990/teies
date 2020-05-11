/********************************************************************
<DOC>
<NAME>             T_WAITC.c
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             November 29, 1987

<PARAM>            server    string    INPUT    Server who sent us the
                                                confirmation.
<PARAM>            result    void **   OUTPUT   Pointer to pointer to put
                                                result data of t_do().

<OUTPUT>           sint
<GLOBALS>          - none -

<PURPOSE>          Wait for a particular server to respond with a conf-
                   irmation command or another process request.

<CALLS>            t_do(), t_recv(), os_wint().
<ERRORS>
<LIMITS>
<MODS>
</DOC>
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"




sint     t_waitc( server_id, result_ptr )

string            server_id;
void                       * result_ptr;   /* <<== Pointer to Pointer! */

{


boolean ok      = TRUE;
sint    rc      = 0,
        state   = 0,
        watchdog= 0;
hword   command = 0,
        flags   = 0;
string  request = S_NULL;




if( TM_master->usrtype != MASTER_SERVER )
{

  while(
        (rc = t_recv( server_id, & command, & flags, & state, & request))
                                     != 0
       )
   {

      if( SERVER_NOT_CONNECTED( rc ) )
      {

         ok = FALSE;
         t_cls_t( server_id, CLOSE_CONNECTION );
         break;

      }
      os_wint( 1, 1000 );
      if( ++watchdog > WATCHDOG_LIMIT )
      {

         if( watchdog < WATCHDOG_LIMIT  )
         {

            except( rc, CONTROL_ERROR, server_id, S_NULL, S_NULL,
                                                  S_NULL, S_NULL  );

         }
         rc = Connection_Timeout ;
         ok = FALSE;
         break;

      }

   }
   if( ok )
   {


      if( command > 0 )
      {

         rc = t_do( server_id, command, flags, state, request, result_ptr );

      }
   }
   s_drop( & request );

}

   return(rc);

}
