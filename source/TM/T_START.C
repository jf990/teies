/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_start()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1988  (rewritten from 07/31/87 version)     |
  |  <LEVEL>   TM                                                     |
  |                                                                   |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   process_type  sint     INPUT   Type of process to start|
  |                                                                   |
  |  <PURPOSE> Start a new Transaction Manager process.               |
  |                                                                   |
  |  <OUTPUT>  sint              restulting PID.                      |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>  Process ID limit is set in TM_COMM header.             |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "exc.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"



sint     t_start( process_type )

sint              process_type;


{

sint           rc           =    0;
tmentry       *last_process =  NIL;
tmentry       *new_process  =  NIL;




    if(  TM_master == NIL )
    {

         rc  =  MASTER_PROCESS_IS_NOT_ACTIVE;

    }
    else
    {

         last_process = t_intab(  -1  );

         if(  last_process  ==  NIL  )
         {

             rc  =  MASTER_PROCESS_IS_NOT_ACTIVE;

         }
         else
         {

            last_process->next = (tmentry *) os_mema( sizeof( tmentry ));
            os_memn( last_process->next, sizeof( tmentry ) );


            last_process->next->prev     =  last_process;

            new_process                  =  last_process->next;
            new_process->tm_pid          =  TM_master->Next_PID;

            TM_master->current           =  new_process;
            TM_master->currentPID        =  new_process->tm_pid;
            TM_master->Next_PID         +=  1;

            if( TM_master->Next_PID >= TM_MAX_PROCESS_ID )
            {

               TM_master->Next_PID = 2;

            }

            if( process_type == USER_PROCESS )
            {

               new_process->conn_entry         = t_conn( (string) -1 );
               new_process->conn_entry->tm_pid = new_process->tm_pid;

            }

         }
     }

   return(new_process->tm_pid);

}
