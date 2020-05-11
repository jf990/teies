/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_tmend()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 13, 1988                                          |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |  <PARAM>   * none *                                               |
  |                                                                   |
  |  <PURPOSE> Deallocate all TM structures and data areas except for |
  |            TM_master itself.                                      |
  |                                                                   |
  |  <OUTPUT>  Sint                                                   |
  |  <CALLS>                             s_drop(), os_memd()          |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  Performs no checking on pointers, so it is possible to |
  |            deallocate random memory.                              |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"





sint     t_tmend()

{

sint       rc   = 0,
           pid  = 0;





if( TM_master != NIL )
{

    if( TM_master->version != S_NULL )
    {
       s_drop( &(TM_master->version)         );
    }
    if( TM_master->MS != S_NULL     )
    {
       s_drop( &(TM_master->MS)              );
    }


/*+-------------------------------------------------------------------+
  |    Shut down all active processes by going through the list of    |
  |    active processes and stopping each one.  t_stop() deallocates  |
  |    the necessary elements for each.                               |
  +-------------------------------------------------------------------+*/

    while (  TM_master->first != NIL  )
    {

       pid = TM_master->first->tm_pid ;
       rc  = t_stop( pid );
       if(  ( rc != 0 )  ||  ( pid == TM_master->first->tm_pid )  )
       {

          break;

       }
    }

    if( TM_master->ucb != NIL )
    {

       if( TM_master->ucb->os_id    != S_NULL )
       {
          s_drop( &(TM_master->ucb->os_id)      );
       }
       if( TM_master->ucb->help_rqc != S_NULL )
       {
          s_drop( &(TM_master->ucb->help_rqc)   );
       }
       if( TM_master->ucb->help_rqs != S_NULL )
       {
          s_drop( &(TM_master->ucb->help_rqs)   );
       }
       if( TM_master->ucb->editor != S_NULL )
       {
          s_drop( &(TM_master->ucb->editor  )   );
       }
       if( TM_master->ucb->editor_profile != S_NULL )
       {
          s_drop( &(TM_master->ucb->editor_profile));
       }
       if( TM_master->ucb->name           != S_NULL )
       {
          s_drop( &(TM_master->ucb->name)          );
       }
       if( TM_master->ucb->dtform         != S_NULL )
       {
          s_drop( &(TM_master->ucb->dtform)        );
       }
       os_memd( TM_master->ucb );

    }
}


   return(0);

}
