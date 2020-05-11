/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_drp                                                |
  |                                                                   |
  |  :AUTHOR.  JF                                                     |
  |                                                                   |
  |  :DATE.    November 30, 1987                                      |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.    ocd_ptr  struct OCD** INOUT  Pointer to the OCD that  |
  |                                          will get the new OCD.    |
  |                                                                   |
  |  :PURPOSE. deallocate an Object Component Descriptor.  If the     |
  |            pointer is NIL then do nothing.  Otherwise             |
  |            the variable is assumed to be a valid pointer to an    |
  |            already existing OCD that is to be cleared out.        |
  |                                                                   |
  |  :OUTPUT.  * struct OCD                                           |
  |                                                                   |
  |  :CALLS.   os_memd(), s_drop().                                   |
  |                                                                   |
  |  :ERRORS.  0 if we cannot make sense out of the pointer.          |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"

OCD       *ocd_drp ( ocd_ptr )


OCD       **ocd_ptr;


{

OCD       *hold = NIL;



hold = *ocd_ptr;
if( hold != NIL )
{

    s_drop( &(hold->name) );
    s_drop( &(hold->key)  );
    os_memd( hold );

}

* ocd_ptr = NIL ;

   return(NIL);

}
