/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_shutdn.c                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |  <PARAM>   * none *                                               |
  |  <PURPOSE> Shut TEIES down properly, either in case of emergency  |
  |            or as the last stage of a normal end.                  |
  |  <OUTPUT>  Sint                                                   |
  |  <CALLS>   isf_end(), obj_end().                                  |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  Performs no checking.                                  |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "isv.h"
#include "ti.h"
#include "form.h"





sint     t_shutdn( )

{

sint    rc              = 0,
        current_process = 0;




    f_mend  ( );

    ti_end  ( );

    t_isfend( );

    obj_end ( );

    var_end ( TM_master->current->vars );
    TM_master->current->vars = NIL;

    current_process       = TM_master->currentPID;
    TM_master->currentPID = 0;
    mem_tend( (struct memnode *) TM_master->uicmdtab, FALSE );
    TM_master->uicmdtab   = NIL;
    TM_master->currentPID = current_process;

    isv_end ( );

    t_tmend ( );

    mem_end ( );

    os_memd( TM_master        );
    TM_master = NIL;



   return(0);

}
