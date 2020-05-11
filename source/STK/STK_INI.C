/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    stk_ini.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 14, 1987                                       |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   max_elements  sint     InPUT  Maximum number of element|
  |                                          on stack                 |
  |  :PARAM.   element_size  sint     InPUT  Size in bytes of 1element|
  |  :PURPOSE. Initialize the stack requested.                        |
  |                                                                   |
  |  :OUTPUT.  Return a pointer to the newly allocated stack.         |
  |                                                                   |
  |  :CALLS.   os_mema()                                              |
  |                                                                   |
  |  :ERRORS.  won't return if no memory                              |
  |                                                                   |
  |  :LIMITS.  see errors.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "stack.h"


STACK  *stk_ini( max_elements,  element_size )

sint             max_elements;
sint                            element_size;

{

byte  *stak = NIL;
STACK *base = NIL;

#D_begin(  stk_ini,  STACK  )


stak         = os_mema(  max_elements * element_size  );
base         = (STACK *) os_mema( sizeof( STACK ));
base->base   = stak;
base->tot_el = max_elements;
base->size   = element_size;
base->tos    = 0;
base->used   = 0;

#D_show( 'New stack at x%x, max size=%d; elmt size=%d','stak,\
          max_elements, element_size'                        );

#D_return( base );

}
