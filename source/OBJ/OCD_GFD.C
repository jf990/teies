/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_gfd()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 7, 1989                                           |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  Object to get components.|
  |  <PARAM>   component   OCD *      INPUT  Component to copy.       |
  |  <PARAM>   gfd         OCB *      INPUT  Generic OS file.         |
  |                                                                   |
  |  <PURPOSE> Copy the contents of a component to an OS File described
  |            by the GFD.                                            |
  |                                                                   |
  |            Works best for Index segments formatted in a contents  |
  |            type object or Array segments.                         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include <stdio.h>
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "bse.h"




sint   ocd_gfd( object, component, gfd )

OCB           * object;
OCD           * component;
OCB           * gfd;


{

   sint        rc               =      0;
   boolean     end_of_component =  FALSE;
   ODT       * odt_entry        =    NIL;
   string      result_string    = S_NULL;





if( OCB_POINTER_OK( object ) && OCB_POINTER_OK( gfd ) )
{

   odt_entry = odt_find( object, component );

   if( odt_type( odt_entry ) < OC_SEG )
   {

      rc = ocd_ctos( object, component, & result_string );
      if( rc == 0 )
      {

         rc = obj_rep( gfd, component, result_string, SEG_UPDATE );
         s_drop( & result_string );

         if( rc != 0 )
         {

            rc = FILE_FUNCTION_ERROR ;

         }
      }
   }
   else
   {

      switch( odt_type( odt_entry ) )
      {

          case OC_SEG   :

               break;

          case OC_NDX :


               if( obj_test_mode( object, OBJ_RAW ) )
               {


               }
               else
               {

                  string line         = S_NULL;
                  sint   len          =      0,
                       * txt_line_num =    NIL;
                  OCD  * txt_ocd      =    NIL;


                  ocd_new( & txt_ocd, NIL );

                  len = s_len( component->name );

                  s_alloc( & ( txt_ocd->name ), len + 5 );
                  os_memc( s_buffer( component->name ),
                                       s_buffer( txt_ocd->name ), len );
                  os_memc( ".TEXT", s_buffer( txt_ocd->name )+ len, 5 );
                  s_smp_ul( txt_ocd->name ) = len + 5;


                  s_alloc(       & ( txt_ocd->key ) , BSE_MAX_KEY_SIZE );
                  os_memn( s_buffer( txt_ocd->key ) , BSE_MAX_KEY_SIZE );
                  s_smp_ul(          txt_ocd->key ) = BSE_MAX_KEY_SIZE  ;

                  txt_line_num = (sint *) s_buffer( txt_ocd->key ) ;


                  while( rc != NO_DATA_FOUND )
                  {

                     *(txt_line_num) += 1;



                     rc = obj_get( object, txt_ocd, & line );
                     if( rc == 0 )
                     {

                        s_trim( & line, 'T' );
                        rc = obj_rep( gfd, component, line, SEG_UPDATE );
                        if( rc != 0 )
                        {

                           break;

                        }
                     }
                     else
                     {

                        if( rc == NO_DATA_FOUND )
                        {

                           rc = 0;

                        }
                        break;

                     }
                  }

                  obj_set_mode( gfd, OBJ_FLUSH );
                  obj_cls(      gfd            );

                  s_drop(  & line    );
                  ocd_drp( & txt_ocd );

               }
               break;

          case OC_ARY :


               if( obj_test_mode( object, OBJ_RAW ) )
               {


               }
               else
               {


               }
               break;

          default       :

               break;

      }
   }
}
else
{

   rc = INVALID_OCB ;

}

   return(rc);

}
