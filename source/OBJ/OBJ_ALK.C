/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_alk()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 19, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   *ocb     struct OCB  INPUT    Pointer to the Object's  |
  |                                          Control Block where we   |
  |                                          get all the linking info |
  |                                          necessary to perform this|
  |                                          task.                    |
  |                                                                   |
  |  <PARAM>   ocd_name   string    INPUT    Component of the %LINKS  |
  |                                          structure that will be   |
  |                                          the target of the link   |
  |                                          operation.               |
  |                                                                   |
  |  <PARAM>   ocb_to_link   OCB *  INPUT    The OCB to add to the    |
  |                                          link array.              |
  |                                                                   |
  |  <PARAM>   link_type     hword  INPUT    The type of link associ- |
  |                                          ation to render between  |
  |                                          the two objects.         |
  |                                                                   |
  |  <PURPOSE> Add a link from the object in question to the object   |
  |            described by the OID input.  The link is added to the  |
  |            specific %LINKS component described by the second arg. |
  |                                                                   |
  |            The link set of routines will only operate on the      |
  |            following Components of an object inheriting system    |
  |            parts:                                                 |
  |                                                                   |
  |                 %LINKS.ACCESS.OID                                 |
  |                 %LINKS.ACCESS.OBJECT_TYPE                         |
  |                 %LINKS.ACCESS.LINK_TYPE                           |
  |                 %LINKS.LINKS.OID                                  |
  |                 %LINKS.LINKS.OBJECT_TYPE                          |
  |                 %LINKS.LINKS.LINK_TYPE                            |
  |                 %LINKS.REVERSE_LINKS.OID                          |
  |                 %LINKS.REVERSE_LINKS.OBJECT_TYPE                  |
  |                 %LINKS.REVERSE_LINKS.LINK_TYPE                    |
  |                                                                   |
  |            Therefore the Component parameter string can only      |
  |            contain either "ACCESS", "LINKS", or "REVERSE_LINKS"   |
  |            as the suffixes are always computed in the routines.   |
  |                                                                   |
  |            Link types can be of the following set:                |
  |                                                                   |
  |               STANDARD_LINK                                       |
  |                                                                   |
  |            The logic of this routine is:                          |
  |                                                                   |
  |             a) Set the key of the Object Component to replace to  |
  |                the OID of the object to be linked.                |
  |                                                                   |
  |             b) Replace the data segment with the object type      |
  |                and the link type.                                 |
  |                                                                   |
  |  <OUTPUT>  sint                           standard OBJ result code|
  |  <Limits>                                                         |
  |            Reverse links are not maintained.  They must be handled|
  |            separately.                                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include <string.h>
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"


sint        obj_alk(  ocb, component, ocb_to_link, link_type  )

OCB                  *ocb;
byte                 *component;
OCB                  *ocb_to_link;
hword                 link_type;



{

   sint    rc                    =      0;
   OCB    *target_ocb            =    NIL;
   OCD    *ocd                   =    NIL;
   string  object_type_component = S_NULL,
           component_prefix      = S_NULL,
           component_string      = S_NULL,
           link_type_component   = S_NULL;
   byte   *reverse_links         = "REVERSE_LINKS";




if( OCB_POINTER_OK( ocb ) && OCB_POINTER_OK( ocb_to_link ) )
{

   s_init(   & object_type_component, ".OBJECT_TYPE" );
   s_init(   & link_type_component,   ".LINK_TYPE"   );
   ocd_new ( & ocd,                   "%LINKS."      );
   s_init(   & component_string, component           );

   s_cat(    & component_prefix, ocd->name, component_string, 0, 0 );
   s_cat( & ( ocd->name ), component_prefix, object_type_component, 0,0);
   oid_key(    ocd, & ( ocb_to_link->oid ) );

   rc = obj_rep(   ocb, ocd, & ( ocb_to_link->type ), SEG_HOLD );

   if( rc == 0 )
   {

      s_cat( & ( ocd->name ), component_prefix, link_type_component,0,0);
      obj_rep(   ocb, ocd, & ( link_type ), SEG_UPDATE );

   }
                                     /* * * * * * * * * * * * * * * * *\
                                      * If this is not a reverse link,*
                                      * then add the reverse link by  *
                                      * swapping the objects.         *
                                     \* * * * * * * * * * * * * * * * */

   if( strcmp( component, reverse_links ) != 0 )
   {

      rc = obj_alk(  ocb_to_link, reverse_links, ocb, link_type  );

   }

   ocd_drp ( & ocd                   );
   s_drop  ( & object_type_component );
   s_drop  ( & link_type_component   );
   s_drop  ( & component_prefix      );
   s_drop  ( & component_string      );

}
else
{

   rc = INVALID_OCB;

}


   return(rc);
}
