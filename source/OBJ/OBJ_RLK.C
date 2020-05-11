/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_rlk()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 19, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb     struct OCB  INPUT    Pointer to the Object's  |
  |                                          Control Block where we   |
  |                                          get all the linking info |
  |                                          necessary to perform this|
  |                                          task.                    |
  |  <PARAM>   ocd_name   string    INPUT    Component of the %LINKS  |
  |                                          structure that will be   |
  |                                          the target of the link   |
  |                                          operation.               |
  |  <PARAM>   oid_to_link   OID *  INPUT    The OID to remove from   |
  |                                          the link index.          |
  |                                                                   |
  |  <PURPOSE> Remove a link from the object in question to the object|
  |            described by the OID input.  The link is taken from the|
  |            specific %LINKS component described by the second arg. |
  |                                                                   |
  |            The logic is to make the ocd, set the key to the OID   |
  |            to remove and then just call obj_rmv() to delete the   |
  |            index.                                                 |
  |                                                                   |
  |  <OUTPUT>  sint                           standard OBJ result code|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"


sint        obj_rlk(  ocb, component, oid_to_remove  )

OCB                  *ocb;
string                component;
OID                  *oid_to_remove;



{

   sint    rc              =    0;
   OCB    *target_ocb      =  NIL;
   OCD    *ocd             =  NIL;




if( OCB_POINTER_OK( ocb ) )
{

   if( OID_POINTER_OK( oid_to_remove ) )
   {

      ocd_new ( & ocd, "%LINKS." );
      s_cat(    &(ocd->name), ocd->name, component, 0, 0 );
      oid_key(    ocd, oid_to_remove );


      rc = obj_rmv( ocb, ocd );

      ocd_drp ( & ocd );

   }
   else
   {

      rc = INVALID_OID;

   }
}
else
{

   rc = INVALID_OCB;

}


   return(rc);
}
