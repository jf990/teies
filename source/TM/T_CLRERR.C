/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_clrerr                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    May 15, 1990                                           |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   process_id   TM_PID    INPUT  Process to clear error.  |
  |                                                                   |
  |  <PURPOSE> Given the process id clear any pending error state.    |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint   t_clrerr(   TM_PID process_id   )


{

   sint            rc                    =      0;
   tmentry       * process_element       =    NIL,
                 * next_process_element  =    NIL;
   string          message_area          = S_NULL;






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

      s_drop( & (process_element->exc_msg) );
      process_element->except   = NO_ERROR;
      process_element->exc_type =        0;
      s_init( & message_area, "%EXC" );
      var_clr(  message_area         );
      s_drop( & message_area         );

   }
}


   return(rc);

}
