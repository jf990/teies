/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    dbm_xind                                               |
  |  <AUTHOR>  Rana Ajaz                                              |
  |  <DATE>    April 5, 1990                                          |
  |  <LEVEL>   DBM                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   None                                                   |
  |                                                                   |
  |  <PURPOSE> This routine exports the objects to an OS file         |
  |            given the object ids (separated by commas) or          |
  |            an individual OID. It calls a form (dbm_xind form)     |
  |            which asks the user to provide either ids or OID       |
  |            and the name of an OS file to which object has to      |
  |            be xeported. If file name is not provided the          |
  |            default file name is used EXPORT TARGET A              |
  |                                                                   |
  |                                                                   |
  |  <OUTPUT>  sint  rc                                               |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "debug.h"
#include "isv.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"
#include "exc.h"
#include "form.h"
#include "vars.h"




sint   dbm_xind( )
{

   sint     rc          =  0,
            R           =  0;

   string   form_id     =  S_NULL,
            quit        =  S_NULL,
            abandon     =  S_NULL,

            object_id   =  S_NULL,
            number      =  S_NULL,
            object_oid  =  S_NULL,
            oid_str     =  S_NULL,
            os_file     =  S_NULL,
            file_name   =  S_NULL;



   hword    rc_form     =  0;

   boolean  DONE        =  FALSE;





#D_begin( DBM_XIND,  DBM  );

   s_init ( &form_id,    "DBM_XIND"  );
   s_init ( &object_id,  "OBJECT_ID" );
   s_init ( &object_oid, "OBJECT_OID");
   s_init ( &os_file,    "OS_FILE"   );


   isv_get( &quit,    ISV_QUIT,    0 );
   isv_get( &abandon, ISV_ABANDON, 0 );

   while( ! DONE )
   {

      rc_form = t_form( form_id, S_NULL, NIL );
      if( ! t_ifquit( rc_form ) )
      {

         number    = var_get( object_id  );
         oid_str   = var_get( object_oid );
         file_name = var_get( os_file    );

         #D_str( 'number',    'id  of object' );
         #D_str( 'oid_str',   'oid of object' );
         #D_str( 'file_name', 'target file  ' );

         if( s_len( number  ) > 0 &&
             s_len( oid_str ) > 0      )
         {

            #D_say( 'Cannot process id and oid at the same time' );
            except( UNEXPECTED_RESPONSE, USER_ERROR, S_NULL,
                                   S_NULL, S_NULL,S_NULL, S_NULL );

            continue;

         }

         if( s_len( file_name ) == 0 )
         {

            s_init( &file_name, "A * EXPORT TARGET"     );
            #D_say( 'Default file EXPORT TARGET A used' );

         }

         if( s_len( number ) > 0 )
         {

            OCB      * list_ocb  =  NIL;
            string   bad_items   =  S_NULL;
            sint     num_bad     =  0;


            rc =  obj_item( number, OBJ_NEED_MANY_OID, NIL,
                            &list_ocb, &bad_items, &num_bad, ANY_OBJ );

            if ( rc == 0 )
            {


               OCB      * target_ocb_1 =  NIL,
                        * some_ocb     =  NIL;
               OID      oid;
               OCD      * list_ocd     =  NIL;
               boolean  FINISHED       =  FALSE;

               #D_say( 'Open the export target file' );
               target_ocb_1 = obj_opn( (OID*) file_name, OS_FILE |
                                                      OBJ_UPDATE );
               if( OCB_POINTER_OK ( target_ocb_1 ) )
               {
                  ocd_new( & list_ocd, NIL  );
                  s_init( &(list_ocd->name), "LIST.LIST.OID" );
                  list_ocd->bias = 1;

                  while ( ! FINISHED )
                  {

                     rc = obj_get( list_ocb, list_ocd, & oid );
                     #D_oid ( '( & oid )', 'source object' );

                     if( rc == 0 )
                     {

                        some_ocb = obj_opn( & oid, OBJ_READ |
                                                 OBJ_REMOTE );
                        if( OCB_POINTER_OK ( some_ocb ) )
                        {

                           #D_ocb( 'some_ocb' );

                           rc = obj_export( target_ocb_1, some_ocb );
                           #D_show( 'rc of obj_export = %d', 'rc'  );
                           obj_cls( some_ocb );

                        }
                        else
                        {

                           FINISHED = TRUE;
                           rc = INVALID_OCB;
                           except( rc, OBJ_LEVEL_ERROR, S_NULL,
                                S_NULL, S_NULL, S_NULL, S_NULL );

                        }

                     }
                     else
                     {

                        #D_say( 'All objects exported' );

                        FINISHED = TRUE;

                     }

                  } /* end while not finished */

                  ocd_drp( & list_ocd   );
                  obj_cls( target_ocb_1 );

               } /* ocb ptr ok target_ocb_1 */
               else
               {

                  rc = INVALID_OCB;
                  except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL,
                                       S_NULL, S_NULL, S_NULL  );

               }

            } /* rc == 0 for obj_item */
            else
            {

               except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL, S_NULL,
                                                    S_NULL, S_NULL );

            }

            obj_cls( list_ocb );

         }  /* id is provided */
         else if( s_len( oid_str ) > 0 )
         {

            OID      *oid_ptr      =  NIL;

            OCB      *target_ocb_2 =  NIL,
                     *source_ocb   =  NIL;

            s_sxtom( (byte **) &oid_ptr, oid_str, 1, 16 );
            #D_oid( 'oid_ptr', 'requested oid' );

            #D_say( 'Open the export target file' );
            target_ocb_2 = obj_opn( (OID*) file_name, OS_FILE |
                                                   OBJ_UPDATE );

            if( OCB_POINTER_OK ( target_ocb_2 ) )
            {

               source_ocb = obj_opn( oid_ptr, OBJ_READ |
                                            OBJ_REMOTE );
               #D_ocb( 'source_ocb' );

               if( OCB_POINTER_OK ( source_ocb ) )
               {

                  #D_say( 'Calling obj_export' );

                  rc = obj_export( target_ocb_2, source_ocb );
                  #D_show( 'rc of obj_export = %d', 'rc'    );

                  obj_cls( target_ocb_2 );
                  obj_cls( source_ocb   );

               }
               else
               {

                  rc = INVALID_OCB;
                  except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL,
                                       S_NULL, S_NULL, S_NULL );

               }

            }
            else
            {

               rc = INVALID_OCB;
               except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL,
                                    S_NULL, S_NULL, S_NULL );

            }

         } /* oid is provided */

      } /* ! t_ifquit */
      else
      {

         #D_say( 'user requested quit or abndon' );
         rc = rc_form;
         DONE = TRUE;

      } /* else ! t_ifquit */

   } /* while ! done */


   s_drop ( &form_id    );
   s_drop ( &object_id  );
   s_drop ( &object_oid );
   s_drop ( &os_file    );


#D_return( 'rc' );

}
