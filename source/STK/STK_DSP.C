/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_dsp.c                                              |
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
  |  :PURPOSE. Dump out the contents of the stack without harming any |
  |            current elements.                                      |
  |  :OUTPUT.  RC = error code                                        |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  20096 : Stack empty.                                   |
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



sint   stk_dsp( base )

STACK          *base;

{

sint  rc = 0,
       i = 0;




   if( base->size != 0 )
   {

      for(i=0; i<base->used; i++ )
      {


      }
   }
   else
   {

      rc = 20095;

   }
   return(rc);

}
