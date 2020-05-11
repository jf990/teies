/********************************************************************

:NAME.      OBJ_LNKS.C
:LEVEL.     OBJ
:AUTHOR.    John L. Foster
:DATE.      February 11, 1988
:INPUT.     ocb   OCB *            INPUT    OCB of object to locate all
                                            links of.

:OUTPUT.    OID **                          Pointer to an array of OIDs
                                            that are all the Objects that
                                            the object in question ref-
                                            erences.

:PURPOSE.
            Return an array of OIDs that are all of the objects that
            the object requested is linked to.  Either forward or back-
            ward linkages are resolved.  Returns NIL if there are no
            links.

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





OID       **obj_lnks( ocb )

OCB                  *ocb;



{



   return(NIL);

}
