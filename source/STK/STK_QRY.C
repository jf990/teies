/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_qry.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 14, 1987                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   base          STACK *  InPUT  Location or stack ID.    |
  |                                                                   |
  |  :PURPOSE. Return number of elements stacked.                     |
  |                                                                   |
  |  :OUTPUT.  rc sint.                                               |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.                                                         |
  |                                                                   |
  |  :LIMITS.                                                         |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "stack.h"



sint   stk_qry( base )

STACK          *base;

{





   return(base->used);

}
