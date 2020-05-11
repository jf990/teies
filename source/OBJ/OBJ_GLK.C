/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_glk()                                              |
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
  |  <PARAM>   oid_to_link   OID ** INOUT    Where to store the OID   |
  |                                          if we find it.           |
  |                                                                   |
  |  <PARAM>   position      sint   INPUT    Specific position to get |
  |                                          the link from the object |
  |                                          component.               |
  |                                                                   |
  |  <PARAM>   link_type     hword  INPUT    Return only links of the |
  |                                          specified type.          |
  |                                                                   |
  |  <PARAM>   object_type * hword  OUTPUT   Return the object type of|
  |                                          the link if found.       |
  |                                                                   |
  |  <PURPOSE> Get a link from the object in question from the        |
  |            specific %LINKS component described by the second arg. |
  |                                                                   |
  |            If link_type is non zero then only links of the        |
  |            specified type are returned.  Otherwise any matching   |
  |            link is returned.                                      |
  |                                                                   |
  |            Position is used as an offset indicator.  If Position  |
  |            is 0 the first link is returned.  If position is -1 the|
  |            last link is returned.  Any positive value returns that|
  |            link within the index unless past the last index where |
  |            the last index will be returned.                       |
  |                                                                   |
  |            If the oid pointer points to a valid oid that is not   |
  |            all NULL then it will be used as the seed key for the  |
  |            index search.                                          |
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


sint     obj_glk(  ocb, component, oid_to_return, position, link_type,
                                                            object_type )

OCB                  *ocb;
string                component;
OID                 **oid_to_return;
sint                  position;
hword                 link_type,
                     *object_type;



{

   sint    rc              =    0;
   hword   data_buffer     =    0,
           link_object_type=    0,
           link_link_type  =    0;
   OCB    *target_ocb      =  NIL;
   OCD    *ocd             =  NIL;
   OID     null_oid              ;
   boolean working         = TRUE;
   string  component_name  =S_NUL,
           object_component=S_NUL;






if( OCB_POINTER_OK( ocb ) )
{

   if( oid_to_return != NIL )
   {

      os_memn( & null_oid, sizeof( OID ) );

      ocd_new ( & ocd, "%LINKS."                                       );
      s_cat(    & component_name, ocd->name, component,          0, 0  );
      s_init(   & object_component, ".OBJECT_TYPE"                     );
      s_cat(    & (ocd->name), component_name, object_component, 0, 0  );
      ocd->bias = position                                              ;
      s_alloc(  & (ocd->key), sizeof( OID )                            );
      os_memn(    s_buffer( ocd->key ), sizeof( OID )                  );
      s_smp_ul(   ocd->key ) = sizeof( OID )                            ;


      if(   (  oid_to_return != NIL  )                       &&
            ( NOT oid_pcmp( (* oid_to_return), (& null_oid) ) )       )
      {

         oid_key( ocd, * oid_to_return );

      }

      rc = obj_get( ocb, ocd, (byte *) & link_object_type );

      if ( rc == 0 )
      {

         while ( working )
         {

            if ( link_type != ANY_TYPE_OF_LINK )
            {

               s_init(& object_component, ".LINK_TYPE" );
               s_cat(&(ocd->name), component_name, object_component,0,0);

               obj_get( ocb, ocd, (byte *) & link_link_type );

               if( link_type != link_link_type )
               {

                  rc = LINK_EXISTS_BUT_TYPE_MISMATCH;
                  break;                               /* We are done. */

               }
            }

            if ( object_type != NIL )
            {

               * object_type = link_object_type;

            }

            if ( oid_to_return == NIL )
            {

               * oid_to_return = (OID *) os_mema( sizeof( OID ) );

            }

            os_memc( s_buffer( ocd->key ), * oid_to_return, sizeof( OID ) );
            rc = 0;
            break;

         }
      }
      ocd_drp ( & ocd              );
      s_drop(   & object_component );
      s_drop(   & component_name   );

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
