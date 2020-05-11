/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_ctos                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 30, 1987                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  - none -                                               |
  |  <PARAM>   *ocb     struct OCB  INPUT    Pointer to the Object's  |
  |                                          Control Block            |
  |            *ocd     struct OCD  INPUT    Pointer to the Object    |
  |                                          Component Descriptor to  |
  |                                          convert.                 |
  |            *buffer  string      OUTPUT   A pointer to a string    |
  |                                          that will contain the    |
  |                                          data which you requested.|
  |                                                                   |
  |  <PURPOSE> This routine will convert the object component data    |
  |            to a string and update the string pointed to by buffer.|
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   ocb_dbp (), odt_find (), dms_sdp (), dms_find (),      |
  |            dms_qd (), dms_qdl (), except ()                       |
  |  <ERRORS>  20023 : data requested was not found.                  |
  |                 20020 if OCD not found in ODT                     |
  |                 20023 if DMS returns null pointer or length       |
  |                 20028 if component is of invalid type             |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"


sint   ocd_ctos(  object, component, buffer  )


OCB             * object     ;
OCD             * component  ;
string          * buffer     ;



{

   sint       rc                  =      0,
              length_of_component =      0;
   hword      component_type      =      0;
   byte      *data                =    NIL;
   ODT       *odtptr              =    NIL;
   string     isv_holder          = S_NULL;
   tint       mode_holder         =      0;






if( OCB_POINTER_OK( object ) )
{


   odtptr = odt_find( object, component );

   if( ODT_POINTER_OK( odtptr ) )
   {

      component_type      = odt_type( odtptr               );
      length_of_component = odt_len(  odtptr, object->type );

      if( (component_type == OC_STR)  ||
          (component_type == OC_ISV   && (! obj_test_mode( object, OBJ_RAW )))
        )
      {

         data = NIL;
         rc = obj_get( object, component, buffer );

      }
      else
      {

         data = (byte *) os_mema( length_of_component );
         mode_holder = object->mode ;
         obj_set_mode( object, OBJ_RAW );
         rc = obj_get( object, component, data );
         object->mode = mode_holder ;
         s_alloc( buffer, 16 );

      }



      if(  rc == 0  )
      {


         switch (  component_type  )
         {

         case OC_BYTE:

            s_put( * buffer, * (byte *) data, 1 );
            break ;


         case OC_HWRD:

            s_htos( buffer, * (hword *) data   );
            break ;


         case OC_INT :

            s_sitos( buffer, * (sint *) data   );
            break ;


         case OC_REAL:

            s_rtos( buffer, * (real *)  data   );
            break ;


         case OC_STR :

            break ;


         case OC_D_T :

            if( ! obj_test_mode( object, OBJ_RAW ) )
            {

               s_dttos( buffer, (datetime *) data,
                                                TM_master->ucb->dtform );

            }
            else
            {

               s_mtoxs( buffer, (byte *) data, sizeof( datetime ) );

            }
            break ;


         case OC_OID :

            s_mtoxs( buffer, (byte *) data, sizeof( OID ) );
            break ;


         case OC_PRIV:

            s_mtoxs( buffer, (byte *) data, sizeof( PRIVS ) );
            break ;


         case OC_ISV :

            if( obj_test_mode( object, OBJ_RAW ) )
            {

               s_htos( buffer, * (hword *) data   );

            }
            break ;


         default     :

            except ( INVALID_OCD_TYPE, INFO,   S_NULL,
                                       S_NULL, S_NULL, S_NULL, S_NULL);
            rc = INVALID_OCD_TYPE;
            break ;

         }

         os_memd(   data   );

      }
   }
   else
   {

      rc = ODT_DOES_NOT_EXIST;

   }
}
else
{

   rc = INVALID_OCB;

}

   return(rc);
}
