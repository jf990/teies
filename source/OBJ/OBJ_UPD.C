/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |                                                                   |
  |  <NAME>    obj_upd                                                |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    June 16, 1988                                          |
  |                                                                   |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   ocb         OCB *      INPUT   Object to update        |
  |  <PARAM>   ocdname     byte *     INPUT   Name of OCD to update   |
  |  <PARAM>   data        byte *     INPUT   Data for update         |
  |                                                                   |
  |  <PURPOSE> Convert the parameters for a call to obj_rep() to      |
  |            update and object's ocd with the passed data.          |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>   ocd_new(), obj_rep(), ocd_drp()                        |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
  |                                                                   |
  |  <LIMITS>  None known.                                            |
  |                                                                   |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   obj_upd( ocb, ocdname, data )

OCB            *ocb;
byte           *ocdname;
void           *data;



{

OCD   *temp_ocd = NIL;
sint   rc       =   0;



   if( ocdname != NIL )
   {

      ocd_new( & temp_ocd, ocdname );
      rc = obj_rep( ocb, temp_ocd, data, SEG_UPDATE );
      ocd_drp( & temp_ocd );

   }


   return(rc);

}
