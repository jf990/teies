/********************************************************************

:NAME.      OBJ_OWN.C
:LEVEL.     OBJ
:AUTHOR.    John L. Foster
:DATE.      February 11, 1988
:INPUT.     ocb   OCB *            INPUT    OCB of object to locate owner
                                            of.

:OUTPUT.    OCB *                           Pointer to OCB of the owner
                                            object.  NIL if no owner.

:PURPOSE.
            Return the OCB of the object that owns the object in question
:CALLS.
:ERRORS.
:LIMITS.
 ********************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "debug.h"





OCB        *obj_own ( ocb )

OCB                  *ocb;



{



   return(NIL);

}
