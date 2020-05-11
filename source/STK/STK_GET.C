/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_get.c                                              |
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
  |  :PURPOSE. Return top stack element without popping it.           |
  |                                                                   |
  |  :OUTPUT.  Return a pointer to copied element.                    |
  |                                                                   |
  |  :CALLS.   os_memc()                                              |
  |                                                                   |
  |  :ERRORS.  0     : Stack empty.                                   |
  |                                                                   |
  |  :LIMITS.  Allocates memory for the popped element and copies the |
  |            data in there.                                         |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "stack.h"



byte  *stk_get( base )

STACK          *base;

{

sint  rc = 0;
byte *holder = NIL;


if( base != NIL && base != (STACK *) -1 )
{

   if( base->used > 0 )
   {

      holder  =  os_mema( base->size );
      os_memc( base->base+(base->size*(base->used-1)),holder,base->size);

   }
}
else
{


}


   return(holder);

}
