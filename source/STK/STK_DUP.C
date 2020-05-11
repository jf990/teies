/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_dup.c                                              |
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
  |  :PURPOSE. Duplicate the top stack element, increasing the stack  |
  |              size by one element.                                 |
  |  :OUTPUT.  0 unless stack full or invalid base.                   |
  |                                                                   |
  |  :CALLS.   stk_psh()                                              |
  |                                                                   |
  |  :ERRORS.  20096 : Stack empty.                                   |
  |            20096 : Stack full                                     |
  |            20096 : Stack base invalid                             |
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



sint   stk_dup( base )

STACK          *base;

{

sint  rc = 0;




   if( base->size != 0 )
   {

      rc = stk_psh( base, base->tos  );

   }


   return(rc);

}
