/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_put.c                                              |
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
  |  :PARAM.   element       byte *   InPUT  Element to put on stack. |
  |  :PARAM.   how           sint     InPUT  One of the following:    |
  |                  0 : Place  on top of stack (LIFO).               |
  |                 -1 : Place on bottom of stack (FIFO).             |
  |                <-1 : Displacement from top of stack.              |
  |                > 0 : Displacement from bottom of stack.           |
  |  :PURPOSE. Selectively place an element on the stack and move all |
  |            current stack elements respectively.                   |
  |  :OUTPUT.  Error if ambiguous, else 0 if could be done.           |
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



sint   stk_put( base, element, how )

STACK          *base;
byte                 *element;
sint                           how;

{

sint   rc = 0;




if( how == 0 )
{

   return(stk_psh( base, element ));

}
if( how == -1 )
{

   return(-1);

}
if( how <-1 )
{

   return(-1);

}
if( how > 0 )
{

   return(-1);

}
   return(1);

}
