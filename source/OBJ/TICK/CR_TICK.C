/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    cr_tick                                                |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 13, 1987                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |  :PARAM.   name        type      OUTPUT  description              |
  |  :PARAM.   name        type      OUTPUT  description              |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. Create Ticket                                          |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"

sint    cr_tick( tick_ocb, modifier, level, params )

OCB             *tick_ocb;
string           modifier;
hword            level;
byte            *params;

{


   return(0);

}
