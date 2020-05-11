/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_ini                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 26, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  AOM      struct _act_obj[]    Action Object table.     |
  |  <PURPOSE> Any one-time object level structure initialization.    |
  |                                                                   |
  |            If the number of objects or actions change, BOTH obj.h |
  |            and obj_act.h must be updated.                         |
  |                                                                   |
  |  <OUTPUT>  rc  sint       Always 0.                               |
  |  <CALLS>   obj_tini() : Initialize Object name table.             |
  |  <LIMITS>  Due to conflicting definitions in the obj.h header file|
  |            it could not be included here.  Therefore a dummy OCB  |
  |            definition has to be made because the primitives ref-  |
  |            erence object control blocks.  The definition was      |
  |            chosen to reference a date/time structure for the OSK  |
  |            since the first OCB element, the OID, is of the same   |
  |            structure.                                             |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "priv.h"
                             /* ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! */
typedef  datetime   OCB;     /* Tweek since we cannot include obj.h */
typedef  datetime   OID;     /* ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! */


                                         /******************************/
extern  struct _OCD_ { sint dummy; };    /* Dummy definition so the    */
typedef struct _OCD_             OCD;    /* Primitives can be included */
                                         /* so the OBJECT/ACTION table */
#include "obj_prim.h"                    /* can be built.              */
#include "obj_act.h"                     /******************************/

extern sint obj_tini();




sint obj_ini()


{





  obj_tini();


   return(0);

}
