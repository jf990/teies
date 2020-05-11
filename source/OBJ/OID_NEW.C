/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    oid_new                                                |
  |  <AUTHOR>  Nestor Voronka                                         |
  |  <DATE>    July  6, 1987                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   oid         OID  *    INOUT   Ptr to a buffer where to |
  |                                          place the new OID. If    |
  |                                          null, allocate memory.   |
  |  <PURPOSE> This routine makes a new OID based on the date/time    |
  |            structure filled in at this time.                      |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   os_mema (), os_dttm ()                                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"




sint       oid_new (   oid )

OID        *oid        ;          /* Ptr to buffer to contain new OID  */



{


   datetime  dt        ;          /* Structure containing Date/Time    */
   sint      rc     = 0;




   if (oid == NIL)               /* If memory hasn't been allocated   */
   {

       rc = INVALID_OID_POINTER;

   }
   else
   {

      os_dttm (&dt) ;
      os_memc( &dt, oid, sizeof( OID ) );


   }

   return(rc);
}
