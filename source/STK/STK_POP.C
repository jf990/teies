/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_pop.c                                              |
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
  |                                                                   |
  |  :PURPOSE. Knock one element off the stack and return its value.  |
  |            Returns NIL if the stack is empty.                     |
  |                                                                   |
  |  :OUTPUT.  Return a pointer to popped element.                    |
  |                                                                   |
  |  :CALLS.   os_memc()                                              |
  |                                                                   |
  |  :ERRORS.  * none *                                               |
  |                                                                   |
  |  :LIMITS.  Allocates memory for the popped element and copies the |
  |            data in there. Therefore the stack routines do not     |
  |            deallocated memory assigned to popped elements.        |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "stack.h"



byte  *stk_pop( base )

STACK          *base;

{

sint  rc = 0;
byte *holder = NIL;




   if( base->size != 0 && base->used > 0 )
   {

      holder  =  os_mema( base->size );
      (base->used)--;
      os_memc( base->base+(base->size*base->used), holder, base->size );
      base->tos = base->base + (base->size * base->used);
      if( base->tos <  base->base )
      {

         base->tos = NIL;

      }
   }


   return(holder);

}
