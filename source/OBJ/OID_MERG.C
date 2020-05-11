/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    oid_merge()                                            |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 24, 1988                                       |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.                                                          |
  |                                                                   |
  |  :PURPOSE. Merge two arrays of OID's and eliminate any duplication|
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   oid_merge( target_oid_array, length_of_target_oid_array,
                  source_oid_array, length_of_source_oid_array,
                  limit, flags                                  )

OID             **target_oid_array,
                 *source_oid_array;
sint             *length_of_target_oid_array,
                  length_of_source_oid_array,
                  limit, flags;



{

   sint    rc          =         0;
   OID     oid_holder             ;





   return(rc);

}
