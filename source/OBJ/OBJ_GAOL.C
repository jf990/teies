/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_gaol()              ( Get All Object Links )       |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 16, 1988                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb      struct OCB* INPUT    Pointer to the Object's  |
  |                                          Control Block where we   |
  |                                          get all the linking info |
  |                                          necessary to perform this|
  |                                          task.                    |
  |                                                                   |
  |  <PARAM>   component  byte *    INPUT    Component to get the link|
  |                                          information from.        |
  |                                                                   |
  |  <PARAM>   oid        OID **    INOUT    Pointer to an array of   |
  |                                          OID's where we will store|
  |                                          the resulting OID's that |
  |                                          make up the links.       |
  |                                                                   |
  |  <PARAM>   array_size    sint * INOUT    How large is the array?  |
  |                                                                   |
  |  <PARAM>   type_of_link  hword  INPUT    Which links to get?      |
  |                                                                   |
  |  <PURPOSE> Return all the links under the %LINKS index.           |
  |                                                                   |
  |            If the oid array pointer is not NIL and size is valid, |
  |            then this routine will use that information to fill in |
  |            an existing array.  No more than array_size elements   |
  |            will be returned.                                      |
  |                                                                   |
  |            If the oid pointer is NIL and size is valid then an    |
  |            array will be allocated of that size and only up to the|
  |            size specified will be returned.                       |
  |                                                                   |
  |            If size is 0 then all the links will be returned.  If  |
  |            the oid pointer is given it is assumed valid and large |
  |            enough to hold the result.                             |
  |                                                                   |
  |            If either the oid array pointer or size pointer cannot |
  |            store the result nothing is done.                      |
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

#define WORK_OID_ARRAY_SIZE                      50



sint obj_gaol( object, component,oid_array, oid_array_size, type_of_link)

OCB                  * object;
byte                 * component;
OID                 ** oid_array;
sint                 * oid_array_size;
hword                  type_of_link;



{

   sint    rc              =      0,
           oid_data_type_size =   0,
           number_of_oids  =      0;
   hword   data_buffer     =      0,
           link_object_type=      0,
           link_link_type  =      0;
   OCB    *target_ocb      =    NIL;
   OCD    *ocd             =    NIL;
   boolean done            =  FALSE;
   string  component_name  = S_NULL,
           work_component  = S_NULL;
   OID     work_oid_array  [ WORK_OID_ARRAY_SIZE ];





if( OCB_POINTER_OK( object ) )
{

   if( oid_array != NIL )
   {

      ocd_new( & ocd, "%LINKS." );
      s_init(  & work_component, component );
      s_cat(   & (ocd->name), ocd->name, work_component, 0, 0 );
      s_init(  & work_component, ".LINK_TYPE" );
      s_cat(   & (ocd->name), ocd->name, work_component, 0, 0 );
      ocd->bias = 1;


      while ( ! done )
      {

         rc = obj_get( object, ocd, & link_link_type );

         switch( rc )
         {

            case 0 :

                  if(  type_of_link == ANY_TYPE_OF_LINK    ||
                    ( (type_of_link & link_link_type) != 0 ) )
                  {

                     os_memc(     s_buffer( ocd->key ),
                              & ( work_oid_array[ number_of_oids ] ),
                                  oid_data_type_size                 );
                     number_of_oids ++ ;

                     if( number_of_oids > WORK_OID_ARRAY_SIZE )
                     {

                        rc   = DATA_OVERFLOW;
                        done =          TRUE;

                     }
                  }
                  break;

            case NO_DATA_FOUND :

                  done = TRUE;
                  rc   =    0;
                  break;

            default             :

                  done = TRUE;
                  break;

         }
      }

      if ( rc == 0 )
      {

         if( * oid_array == NIL )
         {

            * oid_array = (OID *) os_mema
                          ( oid_data_type_size * number_of_oids );

         }

         os_memc( & ( work_oid_array[ 0 ] ), * oid_array,
                          ( oid_data_type_size * number_of_oids ) );
         * oid_array_size = number_of_oids;

      }
      ocd_drp( & ocd );
      s_drop(  & work_component );

   }
   else
   {

      rc = INVALID_OID_POINTER;

   }
}
else
{

   rc = INVALID_OCB;

}


   return(rc);
}
