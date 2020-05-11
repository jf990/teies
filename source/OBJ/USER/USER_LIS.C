/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    user_list()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 4, 1990                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   user_object    OCB *   INPUT  User whose list we need  |
  |                                            to manipulate.         |
  |  <PARAM>   list_component byte *  INPUT  Which list (OID) to use. |
  |  <PARAM>   key            string  INPUT  Key for list entry.      |
  |  <PARAM>   oid            OID *   INPUT  List entry OID.          |
  |  <PARAM>   number         sint    INPUT  List entry number.       |
  |  <PARAM>   data           string  INPUT  List entry data entry.   |
  |  <PURPOSE> Manipulate a list object referenced from a user object |
  |            such as new, old, marked indexes, etc.                 |
  |                                                                   |
  |            The list_component must point to an OID component type.|
  |                                                                   |
  |            The key must be given.  If the OID is NIL then the key |
  |            will be removed, otherwise the key will be added to    |
  |            reference the oid, number and string data (if provided)|
  |            Number and data are not necessary.                     |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "obj_list.h"




sint   user_list(  user_object, component, key, oid, number, data  )

OCB           *    user_object;
byte          *    component;
string             key;
OID           *    oid;
sint               number;
string             data;

{

   sint        rc             =      0;
   OCB      *  list_object    =    NIL;
   OCD      *  list_component =    NIL,
            *  user_component =    NIL;
   ODT      *  odt_entry      =    NIL;
   OID         list_oid               ,
               t_oid                  ;




if( OCB_POINTER_OK( user_object )  &&  user_object->type == USER_OBJ )
{

   if( s_len( key ) > 0 )
   {

      if( DATA_POINTER_OK( component ) )
      {

         ocd_new( & user_component, component );
         odt_entry = odt_find( user_object, user_component );
         if( odt_type( odt_entry ) == OC_OID
         ||  odt_type( odt_entry ) == OC_INHR )
         {

            os_memn( & list_oid, sizeof( OID ) );
            rc = obj_get( user_object, user_component, & list_oid );
            if( ( rc == 0 ) || ( rc == NO_DATA_FOUND ) )
            {

               if( OID_IS_NULL( & list_oid ) )
               {

                  rc = cr_list( NIL, S_NULL, 0, & list_object );
                  if(  rc != 0 ||  ( ! OCB_POINTER_OK( list_object ) )  )
                  {

                     rc = CANNOT_CREATE_OBJECT;

                  }
                  else
                  {

                     rc = obj_rep( user_object, user_component,
                                   & (list_object->oid), SEG_UPDATE );

                  }
               }
               else
               {

                  list_object = obj_opn( & list_oid, OBJ_UPDATE );

               }
               if( OCB_POINTER_OK( list_object ) )
               {

                  ocd_new( & list_component, "LIST.LIST"     );
                  s_copy( & (list_component->key), key, 0, 0 );

                  if( oid == NIL )
                  {

                     rc = obj_rmv( list_object, list_component  );

                  }
                  else
                  {

                     s_init( & (list_component->name), "LIST.LIST.OID" );
                     rc = obj_get( list_object, list_component, & t_oid);
                     if( rc == 0 )
                     {


                     }
                     else
                     {

                        rc = obj_rep( list_object, list_component, oid,
                                                            SEG_UPDATE );

                     }
                     if( data != S_NULL )
                     {

                        s_init( & (list_component->name),
                                                    "LIST.LIST.ENTRY" );
                        rc = obj_rep( list_object, list_component, data,
                                                            SEG_UPDATE );
                        s_init( & (list_component->name),
                                                    "LIST.LIST.NUMBER" );
                        rc = obj_rep( list_object, list_component, & number,
                                                            SEG_UPDATE );

                     }
                     else if( number != 0 )
                     {

                        s_init( & (list_component->name),
                                                    "LIST.LIST.NUMBER" );
                        rc = obj_rep( list_object, list_component, & number,
                                                            SEG_UPDATE );
                     }
                  }
                  ocd_drp( & list_component );
                  obj_cls(   list_object    );

               }
            }
         }
         else
         {

            rc = OCD_NOT_OF_PROPER_TYPE;

         }
         ocd_drp( & user_component );

      }
      else
      {

         rc = INVALID_DATA_POINTER;

      }
   }
   else
   {

      rc = OBJ_LIST_KEY_REQUIRED;

   }
}
else
{

   rc = INVALID_OCB;

}



   return(rc);

}
