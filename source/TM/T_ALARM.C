/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    T_alarm()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 1, 1988                                           |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   process_id     TM_PID  InPUT  Process to set alarm for.|
  |                                                                   |
  |  <PURPOSE> Indicate that there is an alarm pending on the process |
  |            requested.  Currently what this does is set a flag in  |
  |            TM_master to indicate an alarm is pending.  The next   |
  |            screen or window update for the process will sound an  |
  |            alarm or display necessary status information.  This   |
  |            can also be acheived by calling except() with a inter- |
  |            face (06xxx) level error code.                         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   None                                                   |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  None known.                                            |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "exc.h"
#include "tm.h"



sint   t_alarm( process_id )

TM_PID          process_id;

{


sint     rc              =   0;
tmentry *process_entry   = NIL;







   if( process_id < 1 || process_id > TM_MAX_PROCESS_ID )
   {

      process_id = TM_master->current->tm_pid;

   }
   process_entry = t_intab( process_id );
   if( process_entry == NIL || process_entry == (tmentry *) 0xFFFFFFFF )
   {

      rc = PROCESS_IS_NOT_ACTIVE;

   }
   else
   {

      rc = 0;
      process_entry->flags |= PROCESS_ALARM_PENDING;

   }



   return(rc);

}
