/********************************************************************

:name.    T_INTAB .C
:date.    January 07, 1986
:author.  JF
:level.   TM
:purpose:
          Traverse the TM process table and return a pointer to the
          entry requested. If no processes exist, a NIL pointer is
          returned.

:PARAM.  TM_PID  sint  Process ID to look for. If -1 then return the last
                       entry.

:OUTPUT. rc  * tmentry     Pointer to requested process id.

:ERRORS.

:GLOBALS.

         TM_master Structure containing the pointer to first entry.
         TM_entry  Process entry structure definition.

:CALLS.
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



tmentry            *t_intab( pid )

TM_PID                       pid;


{

tmentry  * rc         =    NIL,
         * tm_process =    NIL;






    if( (tm_process  =  TM_master->first)  ==  NIL )
    {

         rc  =  NIL;

    }
    else
    {

         while(  tm_process->tm_pid != pid
             &&  tm_process->next   != NIL  )
         {

              tm_process = tm_process->next;

         }

         if( (tm_process->next   == NIL && pid == -1)
          ||  tm_process->tm_pid == pid                )
         {

              rc  =  tm_process;

         }
         else
         {

              rc  =  NIL;

         }
    }


   return(rc);

}
