/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_psh.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 14, 1987                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   base          byte *   InPUT  Location or stack id.    |
  |                                          on stack                 |
  |  :PARAM.   element       byte *   InPUT  Element to stack.        |
  |                                                                   |
  |  :PURPOSE. Push a new element on to the top of the stack.         |
  |                                                                   |
  |  :OUTPUT.  Error code if stack over flow, else 0.                 |
  |                                                                   |
  |  :CALLS.   os_memc()                                              |
  |                                                                   |
  |  :ERRORS.  20097 : Stack overflow.                                |
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





sint   stk_psh( base,  element )

STACK          *base;
byte                  *element;

{

sint  rc = 0;




   if( base->used == base->tot_el )
   {

   return(20097);

   }
   os_memc( element, base->base + (base->used*base->size), base->size );
   base->tos = base->base + (base->size * base->used);
   (base->used)++;


   return(0);

}
