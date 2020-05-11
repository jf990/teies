/********************************************************************
<doc>
<name>   t_stop.c
<author> John L. Foster
<level>  TM
<date>   November 9, 1987
<purpose>
         Stop a TM processes running, deallocate its memory, update
         the process tables, notify scheduler.

<param>  process_id  tint  INPUT    Process ID to terminate (!).
<global> TM_master
<errors>
<calls>
<limits>
<mods>
</doc>
 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"


sint     t_stop(  process_id  )



TM_PID            process_id;


{
sint           rc                    =    0;
tmentry       *process_element       =  NIL,
              * next_process_element =  NIL;





    if(  TM_master == NIL )
    {

         rc  =  MASTER_PROCESS_IS_NOT_ACTIVE;

    }
    else
    {

       process_element = t_intab(  process_id  );

       if(  process_element  ==  NIL  )
       {

           rc  =  PROCESS_IS_NOT_ACTIVE;

       }
       else
       {



          next_process_element = process_element->next;

                                                /********************/
                                                /* First, fix what  */
                                                /* points to this   */
                                                /* process to point */
                                                /* to the next one. */
                                                /********************/

          if( process_element->prev == NIL )
          {
             TM_master->first   =  next_process_element ;
          }
          else
          {
             process_element->prev->next   =  next_process_element;
          }

          if( next_process_element != NIL )
          {
             next_process_element->prev   =  process_element->prev;
          }

          if( process_element == TM_master->current )
          {

             TM_master->current      =  NIL;
             TM_master->currentPID   =  TM_GLOBAL_PROCESS_ID;

          }
          (TM_master->processes) -- ;


          if( process_element->exc_msg != NIL )
          {

             s_drop(  &(process_element->exc_msg)  );

          }

          if( process_element->forms != NIL )
          {

             os_memd(   process_element->forms    );

          }

          if( process_element->cmd_line != S_NULL )
          {

             s_drop( & (process_element->cmd_line) );

          }
          if( process_element->f_setfld != S_NULL )
          {

             s_drop( & (process_element->f_setfld) );

          }

          if( process_element->conn_entry != NIL
           && TM_master->usrtype        == USER_SERVER )
          {

             t_cls_t( process_element->conn_entry->serv_name, FALSE );
             process_element->conn_entry = NIL ;

          }
          if( process_element->vars != NIL )
          {

             var_end(   process_element->vars     );
             process_element->vars = NIL;

          }




          os_memd(   process_element           );


      }
   }



   return(0);


}
