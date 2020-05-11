/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_active()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1988 (rewritten from Oct_26_86 version)     |
  |  <LEVEL>   TM                                                     |
  |                                                                   |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   process_id    tint     INPUT  Process to activate.     |
  |                                                                   |
  |  <PURPOSE> Activate a dormant process.                            |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  None known.                                            |
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




sint     t_active( process_id )

TM_PID             process_id;


{

sint           rc   =    0;
tmentry *new_process=  NIL;




if(  (process_id > 0)  &&  (process_id < TM_MAX_PROCESS_ID)  )
{

    if(  TM_master == NIL )
    {

         rc  =  MASTER_PROCESS_IS_NOT_ACTIVE;

    }
    else
    {

         new_process = t_intab( process_id );

         if( new_process  ==  NIL  )
         {

             rc  =  PROCESS_IS_NOT_ACTIVE;

         }
         else
         {

            TM_master->current    = new_process;
            TM_master->currentPID = new_process->tm_pid;

         }
     }
}
else
{

     rc = INVALID_PROCESS_ID;

}
   return(rc);

}
