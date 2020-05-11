/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_siend()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    May 3, 1989                                            |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |  <PARAM>   - none -                                               |
  |                                                                   |
  |  <PURPOSE> Deallocate and clean up the TM System Initialization   |
  |            structure, assuming after this point it is no longer   |
  |            needed.                                                |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint   t_siend( )


{

   sint              rc            =   0;
   struct _init_   * sys_init_data = NIL;







if( TM_master->sys_init != NIL )
{

    sys_init_data = TM_master->sys_init;

    s_drop(  & (sys_init_data->lfd_name)         );
    s_drop(  & (sys_init_data->access )          );

    s_vdrop( & (sys_init_data->_lfd_name)        );
    s_vdrop( & (sys_init_data->_lfd_access_mode) );
    if( sys_init_data->_lfd_number_of_blocks != NIL )
    {

       os_memd( sys_init_data->_lfd_number_of_blocks );

    }
    if( sys_init_data->_lfd_block_size != NIL )
    {

       os_memd( sys_init_data->_lfd_block_size );

    }
    s_drop( &(sys_init_data->ISV_name) );
    s_drop( &(sys_init_data->printer)  );
    s_drop( &(sys_init_data->error  )  );

    TM_master->ucb->editor         = sys_init_data->editor;
    TM_master->ucb->editor_profile = sys_init_data->editor_profile;
    TM_master->ucb->dtform         = sys_init_data->datefmt;
    TM_master->ucb->time_zone      = sys_init_data->time_zone;

    os_memd(  TM_master->sys_init  );

    TM_master->sys_init  = NIL;

}

   return( 0 );

}
