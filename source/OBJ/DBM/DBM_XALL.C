/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    dbm_xall                                               |
  |  <AUTHOR>  Rana Ajaz                                              |
  |  <DATE>    April 16, 1990                                         |
  |  <LEVEL>   DBM                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   None                                                   |
  |                                                                   |
  |  <PURPOSE> This routine exports the objects in SOIDI              |
  |            to an OS file. This routine calls the form "dbm_xall"  |
  |            which asks the user to provide the name of the         |
  |            target file. If filename is not provided the default   |
  |            filename "EXPORT TARGET A" is used. The form also      |
  |            provides the option to view the oid, goid, lfd,        |
  |            and object type on the screen one by one.              |
  |                                                                   |
  |                                                                   |
  |  <OUTPUT>  sint  rc                                               |
  |  <CALLS>   soidi_next, soidi_display.                             |
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




sint   dbm_xall( )
{

   OCB      *soidi_ocb  =  NIL,
            *target_ocb =  NIL;

   OID      *oid_ptr    =  NIL,
             oid;

   string    form_id    =  S_NULL,
             export_all =  S_NULL,
             view_all   =  S_NULL,
             os_file    =  S_NULL,
             quit       =  S_NULL,
             abandon    =  S_NULL,
             file_name  =  S_NULL;


   sint      rc         =  0,
             rc_display =  0,
             goid       =  0;

   hword     obj_type   =  0,
             lfd        =  0,
             rc_form    =  0;

   boolean   DONE           =  FALSE,
             FINISHED       =  FALSE,
             response_export=  FALSE,
             response_view  =  FALSE;


#D_begin( DBM_XALL,  DBM  );

s_init( &form_id,    "DBM_XALL"      );
s_init( &export_all, "EXPORT_ALL"    );
s_init( &view_all,   "VIEW_ALL"      );
s_init( &os_file,    "OS_FILE"       );

isv_get( &quit,    ISV_QUIT,      0  );
isv_get( &abandon, ISV_ABANDON,   0  );


while( ! FINISHED )
{

   rc_form = t_form( form_id, S_NULL, NIL );
   if( ! t_ifquit( rc_form ) )
   {

      response_export = t_valyn( (string) var_get( export_all ) );
      response_view   = t_valyn( (string) var_get( view_all   ) );

      if( (response_export == TRUE) && (response_view == TRUE) )
      {

         #D_say( 'Cannot export and view at the same time'    );
         except( INPUT_EXCEPTION, USER_ERROR, S_NULL, S_NULL,
                                      S_NULL, S_NULL, S_NULL  );
         continue;

      }

      if( response_export == TRUE )
      {

         file_name = var_get( os_file       );
         #D_str( 'file_name', 'target file' );

         if( s_len( file_name ) == 0 )
         {

            s_init( &file_name, "A * EXPORT TARGET"    );
            #D_say( 'Default file EXPORT TARGET A used');

         }

         #D_say( 'Open the export target file' );
         target_ocb = obj_opn( (OID *) file_name, OS_FILE |
                                               OBJ_UPDATE );

         if( OCB_POINTER_OK ( target_ocb ) )
         {

            oid_ptr = & oid;
            #D_oid( '( &oid )', 'fresh oid'      );
            os_memn( oid_ptr, sizeof( OID )      );
            #D_oid( '( & oid )', 'after os_memn' );

            DONE = FALSE;

            while( ! DONE )
            {

               #D_say( 'Calling soidi_next' );
               rc = soidi_next( &oid_ptr, &lfd, &goid, &obj_type );
               #D_show( 'rc of soidi_next = %d', 'rc' );
               #D_oid( '( oid_ptr )', 'what soidi_next gave us' );

               if( rc != NO_DATA_FOUND )
               {

                  soidi_ocb = obj_opn( oid_ptr, OBJ_READ
                                            | OBJ_REMOTE );
                  #D_ocb( 'soidi_ocb' );
                  if( OCB_POINTER_OK( soidi_ocb ) )
                  {

                     #D_ocb( 'soidi_ocb' );

                     D_setvar( "D2OBJ_NEXT", TRUE, " " );
                     D_setvar( "D2OBJ_GREC", TRUE, " " );

                     rc = obj_export( target_ocb, soidi_ocb );
                     #D_show( 'rc of obj_export = %d', 'rc' );

                     D_setvar( "D2OBJ_NEXT", FALSE, " " );
                     D_setvar( "D2OBJ_GREC", FALSE, " " );

                     obj_cls( soidi_ocb );

                  }
                  else
                  {

                     rc = INVALID_OCB;
                     except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL,
                                          S_NULL, S_NULL, S_NULL );
                     DONE = TRUE;

                  }

               }
               else
               {

                  #D_say( 'Time to stop' );
                  DONE = TRUE;

               }

            } /* while not done */

            obj_cls( target_ocb );

         }  /* target_ocb ponter ok */
         else
         {

            rc = INVALID_OCB;
            except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL,
                                 S_NULL, S_NULL, S_NULL );

         }

      } /* response_export == true */
      else if( response_export != FALSE )
      {

         except( INPUT_EXCEPTION, USER_ERROR, S_NULL, S_NULL,
                                      S_NULL, S_NULL, S_NULL );
         continue;

      }

      if( response_view == TRUE )
      {

         oid_ptr = &oid;
         #D_oid( '( &oid )', 'raw oid' );
         os_memn( oid_ptr, sizeof( OID ) );
         #D_oid( '( &oid )', 'oid after os_memn' );

         while( rc != NO_DATA_FOUND )
         {

            goid     = 0;
            obj_type = 0;
            lfd      = 0;

            #D_say( 'Calling  soidi_next' );
            rc = soidi_next( &oid_ptr, &lfd, &goid, &obj_type  );

            if ( rc != 0 )
            {
               goid     = 0;
               obj_type = 0;
               lfd      = 0;
               os_memn( oid_ptr, sizeof( OID ) );
               except( rc, PROGRAM_ERROR, S_NULL, S_NULL, S_NULL, S_NULL,
                                                                  S_NULL );

            }

            #D_say( 'Calling soidi_display' );
            rc_display = soidi_vw( oid_ptr, lfd, goid, obj_type );

            if( rc_display == USER_REQUESTED_QUIT )
            {
               rc = NO_DATA_FOUND;
            }
            if( rc_display == USER_REQUESTED_ABANDON )
            {

               rc = NO_DATA_FOUND;
               FINISHED = TRUE;

            }

         }  /* end while  */

      }
      else if( response_view != FALSE )
      {

         except( INPUT_EXCEPTION, USER_ERROR, S_NULL, S_NULL,
                                      S_NULL, S_NULL, S_NULL );
         continue;

      }

   } /* not quit or abandon */
   else
   {

      #D_say( 'User requested quit or abandon' );
      rc = rc_form;
      FINISHED = TRUE;

   }

} /* while not finished */


s_drop( &form_id     );
s_drop( &export_all  );
s_drop( &view_all    );
s_drop( &os_file     );


#D_return( 'rc' );

}
