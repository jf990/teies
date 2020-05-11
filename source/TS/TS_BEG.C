
/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ts_beg.c                                               |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 20, 1987                                       |
  |                                                                   |
  |  :LEVEL.   TM Scheduler                                           |
  |                                                                   |
  |  :GLOBAL.  TM_master   TMCB      INOUT   TEIES Master control blk |
  |                                          and anchor for process   |
  |                                          records.                 |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. Initialize the scheduler (high level) to allow multi-  |
  |            tasking TEIES operation.                               |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  48001 : TM not operational.                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "exc.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint     ts_beg( )




{

sint           rc   =    0;
tmentry       *temp =  NIL;




    if(  TM_master == NIL )
    {

         except( 48001, FATAL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                                             S_NULL );
    }

   return(rc);

}
