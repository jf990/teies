/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>              T_SYSERV.C
<NAME>             T_SYSERV.C
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             November 23, 1987
<PARAMS>           * none *
<OUTPUT>           sint
<GLOBAL>           TM_master

<PURPOSE>
           Poll ISCF receive queues for incomming communication requests.

<ERRORS>
              20007 : A master server was not found.
              20008 : MS connection established but cannot send data.

<CALLS>       isf_open(), except(), isf_send()
<LIMITS>
<MODS>
</DOC>

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "isf.h"
#include "obj.h"
#include "tm.h"





sint     t_syserv( )
{

sint     rc        =      0,
         flags     =      0,
         sync      =      0,
         state     =      0,
         status    =      0;
string   request   = S_NULL,
         user_id   = S_NULL,
         u_id      = S_NULL;
hword    command   =      0,
         tflag     =      0;
ISCF_TAB *con_entry=    NIL;






 while( ! (TM_master->flags & SHUT_DOWN_REQUESTED) )
 {


   flags = NEW_SOMETHING;

   while( 0 == (rc = isf_qry ( S_NULL, flags, & status, & u_id ) ) )
   {

      s_copy( & user_id, u_id, 0, 0 );
      s_trim( & user_id, 'B'        );


      switch( status )
      {

         case NEW_CONNECTION :

                               rc = t_iscf_t( user_id, PENDING_SIGNON );
                               if( rc == 0 )
                               {

                                  t_start( USER_PROCESS );

                               }
                               break;

         case NEW_MESSAGE    :

                               rc =
                               t_recv( user_id, & command,
                                       & tflag, & state,   & request );
                               if((rc == 0) && (command != CONFIRMATION))
                               {

                                  rc = t_do( user_id, command, tflag,
                                               state, request, NIL    );

                               }
                               s_drop( & request );
                               break;

         case NEW_DISCONNECTION :

                               con_entry = t_conn( user_id );
                               if( ISF_TAB_POINTER_OK( con_entry ) )
                               {

                                  t_stop( con_entry->tm_pid );

                               }
                               rc = t_cls_t( user_id, CLOSE_CONNECTION );
                               break;



         default             :

                               break;

      }
      status = 0;
      s_drop( & user_id );
      sync = 0;
      TM_master->flags |= FLUSH_OPEN_OBJECTS;

   }

   if( TM_master->flags & FLUSH_OPEN_OBJECTS )
   {

      if( sync > 5 )
      {

         sync = 0 ;
         obj_sync();
         TM_master->flags &= ~FLUSH_OPEN_OBJECTS;

      }
      else
      {

         sync ++ ;

      }
   }

   os_wint( 1, 1000 );

 }



   return(0);

}
