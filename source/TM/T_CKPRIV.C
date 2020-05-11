/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_ckpriv                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 26, 1988                                      |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   privilege   PRIV *    INPUT   Privilege map to check.  |
  |                                                                   |
  |  <PURPOSE> Check to see if the role requested is in the privilege |
  |            set passed.                                            |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>  None known.                                            |
  |  <LIMITS>  None known.                                            |
  |  <MODS>                                                           |
  |  </DOC >                                                          |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"
#include "vars.h"



sint   t_ckpriv (  process_id, privilege  )

TM_PID             process_id;
PRIVS            * privilege;


{




if( process_id < 1 )
{

   process_id = TM_master->current->tm_pid;

}



   return(0);

}
