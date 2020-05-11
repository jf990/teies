
/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_que.c                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 20, 1987                                       |
  |  <LEVEL>   TM Scheduler                                           |
  |  <GLOBAL>  TM_master   TMCB      INOUT   TEIES Master control blk |
  |                                          and anchor for process   |
  |                                          records.                 |
  |  <PARAM>   event       sint      INPUT   Event type to wait on.   |
  |  <PARAM>   server_id   string    INPUT   Server who casued the wait
  |  <PARAM>   result      void *    OUTPUT  Store any results.       |
  |                                                                   |
  |  <PURPOSE> Queue a new scheduler element for the current process. |
  |            An event to wait on is scheduled and when that event   |
  |            occurs this process will start up again at the instruc-|
  |            tion after the t_que() call.                           |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <ERRORS>  28001 : TM not operational.                            |
  |            28009 : Scheduler queue full.                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "exc.h"




sint     t_que(  event, server_id, result_ptr  )

hword            event;
string           server_id;
void           * result_ptr;


{

tmentry   * temp    =  NIL;
boolean     ok      = TRUE;
sint        rc      = 0,
            state   = 0,
            watchdog= 0;
hword       command = 0,
            flags   = 0;
string      request = S_NULL;




    if(  TM_master == NIL )
    {

         except( 48001, FATAL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                                             S_NULL  );
    }
    switch (  event  )
    {

       case  NO_WAIT:
             {

                break;

             }
       case  TASK_COMPLETE:
             {

                break;

             }
       case  TASK_EXCEPTION:
             {

                break;

             }
       case  TASK_PAUSE:
             {

                break;

             }
       case  SHUT_DOWN:
             {

                break;

             }
       case  DISK_WRITE:
             {

                break;

             }
       case  DISK_READ:
             {

                break;

             }
       case  DISK_ERROR:
             {

                break;

             }
       case  TIMER:
             {

                break;

             }
       case  ISCF_SEND:
             {

                break;

             }
       case  ISCF_RECEIVE:
             {

                while( (  rc = t_recv( server_id, & command,
                                     & flags, & state, & request  ))
                            != 0 )
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

                         except( rc, CONTROL_ERROR, server_id, S_NULL,
                                                S_NULL, S_NULL, S_NULL );

                      }
                      ok = FALSE;
                      break;

                   }

                }
                if( ok )
                {

                   if( command > 0 )
                   {

                      rc = t_do( server_id, command, flags, state,
                                                   request, result_ptr );

                   }
                }
                break;

             }
       case  ISCF_ERROR:
             {

                break;

             }
       case  CONSOLE_READ:
             {

                break;

             }
       case  CONSOLE_WRITE:
             {

                break;

             }
       case  CONSOLE_ERROR:
             {

                break;

             }
       default:
             {

                rc = TM_EVENT_NOT_DEFINED ;

             }
    }








   return(rc);

}
