
/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ts_end.c                                               |
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
  |  :PURPOSE. Shutdown the scheduler (high level).                   |
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




sint     ts_end( )




{

sint           rc   =    0;
tmentry       *temp =  NIL;




    if(  TM_master == NIL )
    {

         except( 48001, FATAL_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                                             S_NULL  );
    }

   return(rc);

}
