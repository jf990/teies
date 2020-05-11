/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_find                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 16, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT   Object to find in       |
  |  <PARAM>   ocdname     byte *     INPUT   Name of OCD to find     |
  |  <PARAM>   data        byte *     INPUT   Where to put results    |
  |                                                                   |
  |  <PURPOSE> Convert the parameters for a call to obj_get() to      |
  |            get data out of an object.                             |
  |                                                                   |
  |  <OUTPUT>  sint RC from obj_rep().                                |
  |  <CALLS>   ocd_new(), obj_rep()                                   |
  |  <LIMITS>  Does not work for segments, array segments, offsets    |
  |            or indexes.  These require direct calls to obj_get()   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   obj_find( ocb, ocdname, data )

OCB            *ocb;
byte           *ocdname;
byte           *data;



{

OCD   *temp_ocd = NIL;
sint   rc       =   0;



   if( ocdname != NIL )
   {

      ocd_new( &temp_ocd, ocdname );
      rc = obj_get( ocb, temp_ocd, data );
      ocd_drp( &temp_ocd );

   }


   return(rc);

}
