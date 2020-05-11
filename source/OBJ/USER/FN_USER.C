/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <LEVEL>   OBJ                                                    |
  |  <NAME>    fn_user                                                |
  |  <AUTHOR>  Rana Ajaz-Ur-Rehman                                    |
  |  <DATE>    November 14, 1989                                      |
  |  <PARAM>   name        type      OUTPUT  description              |
  |                                                                   |
  |  <PURPOSE> Find   User                                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "form.h"
#include "isv.h"
#include "exc.h"
#include "obj_prim.h"


sint     fn_user( list_object, user_name_parts, level, params )


OCB            *list_object;
string          user_name_parts;
hword           level;
byte           *params;



{



   sint     rc             =  0,
            R              =  0,
            len            =  0,
            list_size      =  0;

   string   comma          =  S_NULL,
            garbage        =  S_NULL,
            dummy_str      =  S_NULL,
            name_list      =  S_NULL,
            source         =  S_NULL,
            part_of_name   =  S_NULL,
            last           =  S_NULL,
            first          =  S_NULL,
            oid_str        =  S_NULL;

   hword    searches_to_do =  0;





s_init( & comma,     ","  );

if( TM_master->usrtype == MASTER_SERVER )
{

   s_copy( & source, user_name_parts, 0, 0  );

   len = s_first( comma, source, 0, 0       );
   if( len != 0 )
   {
      s_clip( & part_of_name, & source, 0, len );
      s_clip( & garbage, & part_of_name, -1, 1 );
      s_trim( & part_of_name, 'B'              );
   }

   len = s_first( comma, source, 0, 0       );
   if( len != 0 )
   {
      s_clip( & last, & source, 0, len         );
      s_clip( & garbage, & last, -1, 1         );
      s_trim( & last, 'B'                      );
   }

   if( s_len( source ) > 0 )
   {

      s_copy( & first, source, 0, 0         );
      s_trim( & first, 'B'                  );

   }


   rc = t_fname( part_of_name, last, first, & list_object );

   /**********************************************/
   /*                                            */
   /* obj_find is not needed here just for test  */
   /* purposes.  (temporary)                     */
   /*                                            */
   /**********************************************/

   obj_find( list_object, "LIST.SIZE", & list_size );

   if( OCB_POINTER_OK( list_object )  )
   {

      rc = t_m_obj( t_invoker(), list_object, &dummy_str, FALSE, 0 );
      if( rc == 0 )
      {

         s_mtoxs( & oid_str, (byte*) &( list_object->oid ),
                                             sizeof( OID ) );

         t_csend( t_invoker(), rc, oid_str );

      }
      else
      {

         t_csend( t_invoker(), rc, dummy_str );

      }


      /*+---------------------------------------------+*/
      /*|                                             |*/
      /*|  No. The caller will delete the list when   |*/
      /*|  done with it.                              |*/
      /*|                                             |*/
      /*|   obj_set_flag( list_object, OBJ_DELETE );  |*/
      /*|   obj_cls( list_object );                   |*/
      /*|   list_object = NIL;                        |*/
      /*|                                             |*/
      /*+---------------------------------------------+*/


   } /* t_fname gave us a good list */
   else
   {

      t_csend( t_invoker(), rc, dummy_str );

   }

} /* if master server */
else
{

   OID      user_oid,
           *list_oid       =  NIL;

   OCB      *user_ocb      =  NIL,
            *list_ocb      =  NIL;

   OCD      *list_ocd      =  NIL;


   sint     size           =  0,
            rc_rm_list     =  0;

   real     x              =  0.0,
            y              =  0.0;

   string   form1_id       =  S_NULL,
            form2_id       =  S_NULL,
            found1         =  S_NULL,
            found2         =  S_NULL,
            size_str       =  S_NULL,

            quit           =  S_NULL,
            abandon        =  S_NULL,
            YorN           =  S_NULL,

            nm_part        =  S_NULL,
            name_part      =  S_NULL,
            lst_name       =  S_NULL,
            last_name      =  S_NULL,
            Ist_name       =  S_NULL,
            first_name     =  S_NULL,
            ok_to_view     =  S_NULL,
            clear_list     =  S_NULL,
            want_to_view   =  S_NULL,
            result         =  S_NULL;

   hword    rc1_form       =  0,
            rc2_form       =  0;


   boolean  USERS_TO_VIEW  =  FALSE,
            DONE           =  FALSE,
            FINISHED       =  FALSE,
            ERRORS         =  TRUE,
            OK_TO_SEARCH   =  FALSE,
            FROM_CMD_LINE  =  FALSE,
            START_VIEW_PROCESS =  FALSE,
            answer         =  TRUE,
            new_list       =  TRUE;


   s_init( & YorN,      "yes or no"    );
   isv_get( & quit,     ISV_QUIT,    0 );
   isv_get( & abandon,  ISV_ABANDON, 0 );



   while (  !  DONE  )
   {

      size = 0;
      OK_TO_SEARCH = FALSE;
      START_VIEW_PROCESS = FALSE;
      ERRORS = TRUE;

      if( s_len( user_name_parts ) < 1 )
      {

         FROM_CMD_LINE = FALSE;

         s_init ( & form1_id,       "7110"         );
         s_init ( & nm_part,       "NAME_PART"    );
         s_init ( & lst_name,      "LAST_NAME"    );
         s_init ( & Ist_name,      "FIRST_NAME"   );
         s_init ( & found1,        "ITM_FOUND"    );
         s_init ( & clear_list,    "CLEAR_LIST"   );
         s_init ( & ok_to_view,    "OK_TO_VIEW"   );


         while( ERRORS )
         {

            rc1_form = t_form( form1_id, S_NULL, NIL );
            if ( ! t_ifquit( rc1_form ) )
            {

               searches_to_do = 0;

               name_part  = var_get( nm_part );
               if( s_len( name_part ) != 0   )
               {

                  searches_to_do = ( NICK_NAME | NUMBER_NAME
                                               | PEN_NAME    );

               }

               last_name  = var_get( lst_name );
               if( s_len( last_name ) != 0    )
               {

                  searches_to_do = LAST_NAME;

               }

               first_name = var_get( Ist_name );
               if( s_len( first_name ) != 0   )
               {

                  searches_to_do = FIRST_NAME;

               }

               if( searches_to_do )
               {

                  OK_TO_SEARCH = TRUE;

               }

               if( USERS_TO_VIEW == TRUE )
               {

                  answer   = t_valyn( var_get( ok_to_view ));
                  new_list = t_valyn( var_get( clear_list ));

                  if( answer == UNEXPECTED_RESPONSE )
                  {

                     except( INPUT_EXCEPTION, USER_ERROR,
                             var_get( ok_to_view ), YorN, S_NULL,
                                                 S_NULL, S_NULL );
                     f_curpos( form1_id, NO_FLAGS, ok_to_view,
                                                           x, y );

                     continue;

                  }

                  else if( new_list == UNEXPECTED_RESPONSE )
                  {

                     except( INPUT_EXCEPTION, USER_ERROR,
                             var_get( clear_list ), YorN, S_NULL,
                                                 S_NULL, S_NULL );
                     f_curpos( form1_id, NO_FLAGS, clear_list,
                                                           x, y );

                     continue;

                  }

                  if( (answer == TRUE) && (new_list == TRUE) )
                  {

                     /* decide on a proper error code */
                     except( INPUT_EXCEPTION, USER_ERROR, S_NULL,
                                 S_NULL, S_NULL, S_NULL, S_NULL );

                     continue;

                  }
                  else if( (answer == TRUE) && (new_list == FALSE) )
                  {

                     ERRORS = FALSE;
                     OK_TO_SEARCH = FALSE;
                     START_VIEW_PROCESS = TRUE;

                  }
                  else if( (answer == FALSE) && (new_list == FALSE))
                  {



                  }
                  else if( (answer == FALSE) && (new_list == TRUE) )
                  {


                     var_clr( nm_part      );
                     var_clr( lst_name     );
                     var_clr( Ist_name     );
                     var_clr( clear_list   );
                     var_clr( ok_to_view   );
                     var_clr( found1       );

                     f_rstfld( form1_id, NO_FLAGS, nm_part     );
                     f_rstfld( form1_id, NO_FLAGS, lst_name    );
                     f_rstfld( form1_id, NO_FLAGS, Ist_name    );
                     f_rstfld( form1_id, NO_FLAGS, clear_list  );
                     f_rstfld( form1_id, NO_FLAGS, ok_to_view  );
                     f_rstfld( form1_id, NO_FLAGS, found1      );

                     if( OCB_POINTER_OK ( list_ocb ) )
                     {

                        rc = rm_list( list_ocb, TM_master->MS,
                                      OBJ_DELETE_REMOTE_LIST, NIL );
                        if( rc == 0 )
                        {

                           rc = rm_list( NIL, S_NULL,
                                 OBJ_DELETE_LOCAL_LIST, &list_ocb );
                           if( rc == 0 )
                           {

                              USERS_TO_VIEW = FALSE;
                              OK_TO_SEARCH  = FALSE;

                           }

                        }

                        if( rc != 0 )
                        {

                           except( rc, USER_INFO, S_NULL, S_NULL,
                                            S_NULL, S_NULL, S_NULL );

                        }

                     }

                  }

               } /* users to view */

               if( ( searches_to_do ) && ( OK_TO_SEARCH ) )
               {


                  s_nulout( & name_list );

                  s_cat( &name_list, name_list, name_part,  0, 0 );
                  s_cat( &name_list, name_list, comma,      0, 0 );
                  s_cat( &name_list, name_list, last_name,  0, 0 );
                  s_cat( &name_list, name_list, comma,      0, 0 );
                  s_cat( &name_list, name_list, first_name, 0, 0 );


                  ERRORS = FALSE;

               }

            } /* if ! t_ifquit */
            else
            {

               rc     = rc1_form;

               ERRORS = FALSE;
               OK_TO_SEARCH = FALSE;
               START_VIEW_PROCESS = FALSE;

               DONE   = TRUE;

            }

         } /* while errors */

      } /* names from cmd line < 1 */
      else
      {

         FROM_CMD_LINE = TRUE;

         if( USERS_TO_VIEW )
         {

            OK_TO_SEARCH = FALSE;
            s_init( & form2_id,     "7111"         );
            s_init( & want_to_view, "WANT_TO_VIEW" );

            while( ERRORS )
            {

               rc2_form = t_form( form2_id, S_NULL, NIL );
               if( ! t_ifquit( rc2_form ) )
               {
                  if( (answer = t_valyn( var_get( want_to_view )))
                                                         == TRUE )
                  {

                     ERRORS = FALSE;
                     START_VIEW_PROCESS = TRUE;

                  }
                  else if( answer == FALSE )
                  {

                     ERRORS = FALSE;
                     START_VIEW_PROCESS = FALSE;
                     DONE = TRUE;

                  }
                  else if( answer != FALSE )
                  {

                     except( INPUT_EXCEPTION, USER_ERROR,
                             var_get( want_to_view ), YorN, S_NULL,
                                                    S_NULL, S_NULL );
                     f_curpos( form2_id, NO_FLAGS, want_to_view, x, y );

                     continue;

                  }

               } /* if ! t_ifquit */
               else
               {

                  rc     = rc2_form;
                  ERRORS = FALSE;
                  OK_TO_SEARCH = FALSE;
                  START_VIEW_PROCESS = FALSE;

                  DONE   = TRUE;

               }

            } /* while errors */

         } /* if users to view */
         else
         {
            s_copy( & name_list, user_name_parts, 0, 0 );
            OK_TO_SEARCH  = TRUE;

         }

      }


      if( OK_TO_SEARCH )
      {


         rc = t_remote( TM_master->MS, USER_OBJ, FIND_ACT,
                        level, list_ocb, name_list, params );
         if( rc == 0 )
         {

            rc = t_waitc( TM_master->MS, & result );
            if( rc == 0 )
            {

               s_sxtom( (byte **) &list_oid, result, 1, 16 );

               if( ! OID_IS_NULL ( list_oid ) )
               {

                  /*+------------------------------------------+*/
                  /*| Hopefully we have received a new list obj|*/
                  /*| from MS, so we must delete the existing  |*/
                  /*| one before start using the new one.      |*/
                  /*+------------------------------------------+*/

                  if( OCB_POINTER_OK ( list_ocb ) )
                  {

                     rc = rm_list( NIL, S_NULL, OBJ_DELETE_LOCAL_LIST,
                                                           &list_ocb );

                  }

                  list_ocb = obj_opn( list_oid, OBJ_UPDATE );
                  if( OCB_POINTER_OK( list_ocb ) )
                  {

                     obj_find( list_ocb, "LIST.SIZE", & size );

                     if( size != 0 )
                     {

                        USERS_TO_VIEW = TRUE;

                        s_sitos( & size_str, size );

                        if( FROM_CMD_LINE )
                        {

                           s_init( & found2, "USERS_FOUND" );
                           var_set( found2, size_str );

                        }
                        else
                        {

                           var_set( found1, size_str );
                           f_setfld( form1_id, found1, size_str );

                        }

                     } /* size of list != 0  */
                     else
                     {

                        /* Call except */
                        except( NAME_NOT_FOUND, USER_INFO, name_list,
                                     S_NULL, S_NULL, S_NULL, S_NULL );
                        if( FROM_CMD_LINE )
                        {

                           DONE = TRUE;

                        }

                     }

                  } /* if ocb ok list_ocb */
                  else
                  {

                     except( INVALID_OCB, PROGRAM_ERROR, S_NULL,
                                    S_NULL, S_NULL, S_NULL,S_NULL );
                     if( FROM_CMD_LINE )
                     {

                        DONE = TRUE;

                     }

                  }

               } /* oid is not null */
               else
               {

                  except( INVALID_OID_POINTER, PROGRAM_ERROR, S_NULL,
                                      S_NULL, S_NULL, S_NULL, S_NULL );

                  if( FROM_CMD_LINE )
                  {

                     DONE = TRUE;

                  }

               }

            } /* rc of t_waic for result = 0  */
            else
            {

               except( NAME_NOT_FOUND, USER_INFO, name_list, S_NULL,
                                             S_NULL, S_NULL, S_NULL );
               if( FROM_CMD_LINE )
               {

                  DONE = TRUE;

               }

            }

         } /* rc of t_remote = 0 */
         else
         {

            except( NAME_NOT_FOUND, USER_INFO, name_list,
                               S_NULL, S_NULL, S_NULL, S_NULL  );

            if( FROM_CMD_LINE )
            {

               DONE = TRUE;

            }

         }

      } /* if ok to search */

      if( START_VIEW_PROCESS )
      {

         ocd_new( & list_ocd, "LIST.LIST.OID" );
         list_ocd->bias = 1;

         FINISHED  =  FALSE;

         while( ! FINISHED  )
         {

            rc = obj_get( list_ocb, list_ocd, & user_oid );
            if( rc == 0 )
            {

               user_ocb = obj_opn( & user_oid,
                                   OBJ_READ | OBJ_REMOTE );

               rc = gt_user( user_ocb, S_NULL, (hword)0,
                                                 params  );
               obj_cls( user_ocb );

               if( rc == USER_REQUESTED_ABANDON  ||
                    ( rc == USER_REQUESTED_QUIT  &&
                                      FROM_CMD_LINE ) )
               {

                  FINISHED = TRUE;
                  DONE = TRUE;

               }
               else if( ( rc == USER_REQUESTED_QUIT ) &&
                                     ( ! FROM_CMD_LINE ) )
               {

                  FINISHED = TRUE;

               }

            } /* rc = 0 of obj_get for oid from list */
            else
            {


               FINISHED = TRUE;

               if( FROM_CMD_LINE )
               {
                  DONE = TRUE;
               }

            }

            if( ( FINISHED == TRUE ) && ( ! FROM_CMD_LINE ) )
            {

               var_clr( nm_part    );
               var_clr( lst_name   );
               var_clr( Ist_name   );
               var_clr( ok_to_view );

               f_rstfld( form1_id, NO_FLAGS, nm_part    );
               f_rstfld( form1_id, NO_FLAGS, lst_name   );
               f_rstfld( form1_id, NO_FLAGS, Ist_name   );
               f_rstfld( form1_id, NO_FLAGS, ok_to_view );

            }


         } /* while ! finished */

      } /* if start view process */

   } /* while ! done */


   if( OCB_POINTER_OK ( list_ocb ) )
   {

      rc_rm_list = rm_list( list_ocb, TM_master->MS,
                    OBJ_DELETE_REMOTE_LIST, NIL );
      if( rc_rm_list == 0 )
      {

         rc_rm_list = rm_list( NIL, S_NULL, OBJ_DELETE_LOCAL_LIST,
                                                      &list_ocb );
      }

      if( rc_rm_list != 0 )
      {

         rc = rc_rm_list;
         except( rc, USER_INFO, S_NULL, S_NULL,
                          S_NULL, S_NULL, S_NULL );

      }


   }


   f_close( form1_id, NO_FLAGS );
   f_close( form2_id, NO_FLAGS );

   ocd_drp( & list_ocd );

   s_drop ( & form1_id  );
   s_drop ( & form2_id  );
   s_drop ( & nm_part   );
   s_drop ( & lst_name  );
   s_drop ( & Ist_name  );
   s_drop ( & found1    );
   s_drop ( & found2    );
   s_drop ( & ok_to_view   );
   s_drop ( & want_to_view );

   s_drop ( & comma     );

} /* user server */

   return(rc);

}
