/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_lnk                                                |
  |  <AUTHOR>  Nestor Voronka                                         |
  |  <DATE>    July  2, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb1     struct OCD   INPUT  Object Control Block     |
  |  <PARAM>   *ocb2     struct OCD   INPUT  Object Control Block     |
  |  <PARAM>   *ocd1     struct OCB   INPUT  Obj Component Descriptor |
  |  <PARAM>   *ocd2     struct OCB   INPUT  Obj Component Descriptor |
  |  <PURPOSE> Links two object paths together.                       |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"

sint obj_lnk (ocb1, ocb2, ocd1, ocd2)

OCB         *ocb1 ;               /* Ptr to Object Control Block    #1 */
OCB         *ocb2 ;               /* Ptr to Object Control Block    #2 */
OCD         *ocd1 ;               /* Ptr to Obj Component Descriptor 1 */
OCD         *ocd2 ;               /* Ptr to Obj Component Descriptor 2 */



{

   return(0);
}
