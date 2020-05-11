/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_rot.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 14, 1987                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   base          STACK *  InPUT  Location or ID of stack. |
  |  :PARAM.   position      sint     InPUT  Element number to rotate.|
  |  :PURPOSE. Rotate stack by bringing element at position to TOS and|
  |            pushing everyone else down.                            |
  |  :OUTPUT.  Error for empty stack, else 0.                         |
  |                                                                   |
  |  :CALLS.   os_memc()                                              |
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



sint   stk_rot( base, position )

STACK          *base;
sint                  position;

{

sint  rc = 0;






   return(rc);

}
