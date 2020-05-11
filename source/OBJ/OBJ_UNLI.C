/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    obj_unlink()                                           |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    July  2, 1987                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   *ocb1     struct OCB   INPUT  Object Control Block     |
  |  :PARAM.   *ocb2     struct OCB   INPUT  Object Control Block     |
  |  :PARAM.   *ocd1     struct OCD   INPUT  Obj Component Descriptor |
  |                                                                   |
  |  :PURPOSE. Removes links from one object instance to another.     |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   None.                                                  |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  Does not do anything yet.                              |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"

sint        obj_unlink( ocb1, ocb2, ocd1 )

OCB         *ocb1 ;               /* Ptr to Object Control Block    #1 */
OCB         *ocb2 ;               /* Ptr to Object Control Block    #2 */
OCD         *ocd1 ;               /* Ptr to Obj Component Descriptor 1 */



{

   return(0);
}
