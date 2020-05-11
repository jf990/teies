/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_end.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 14, 1987                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   base          byte *   InPUT  Location of stack        |
  |  :PURPOSE. Kill the stack.                                        |
  |                                                                   |
  |  :OUTPUT.  Return 0.                                              |
  |                                                                   |
  |  :CALLS.   os_memd()                                              |
  |                                                                   |
  |  :ERRORS.  20098 : Bad stack base.                                |
  |                                                                   |
  |  :LIMITS.  Tries to deallocate the stack.  If cannot we assume    |
  |            the programmer passed us a bad stack pointer.          |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "stack.h"



sint   stk_end( base )

STACK          *base;

{

sint       rc = 0;




rc = os_memd(  base->base );
rc = os_memd(  base       );
if(  rc != 0  )
{

     rc = 20098;

}
   return(rc);

}
