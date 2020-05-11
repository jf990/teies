/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    oid_key.c                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 09, 1987                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocd         struct * OCD  INOUT  Object component      |
  |                                             descriptor structure  |
  |                                             to get the oid key.   |
  |  <PARAM>   oid         OID *         INPUT  Object ID             |
  |  <PURPOSE> Copy the OID to be used as a key for an ocd.           |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   s_init(), os_memc().                                   |
  |  <LIMITS>  No correction or error reporting if either pointer is  |
  |            NULL.                                                  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"



sint   oid_key( ocd, oid )


OCD            *ocd;
OID                 *oid;


{




if(  (oid != NULL)  &&  (ocd != NULL)  )
{

   s_init( &(ocd->key), "====----"        );
   os_memc(  oid,       s_buffer(ocd->key), sizeof( OID ) );

}


   return(0);

}
