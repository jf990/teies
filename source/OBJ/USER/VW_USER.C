/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    vw_user                                                |
  |  <AUTHOR>  John L. Foster, Rana Ajaz                              |
  |  <DATE>    December 30, 1988                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   some_ocb       OCB *   INPUT   Any object, verified    |
  |                                            based on a combination |
  |                                            of level and search_str|
  |  <PARAM>   search_string  String  INPUT   Input string to find a  |
  |                                            matching name(s) based |
  |                                            on type of search.     |
  |  <PARAM>   type_of_search HWord   INPUT   One of the following:   |
  |                                                                   |
  |                     USER_OBJECT_ONLY : The first parameter is to  |
  |                                        be a user OCB and only view|
  |                                        that user.                 |
  |                                                                   |
  |                     SEARCH_FOR_USERS_ON_STRING : The search_string|
  |                                        contains pieces of names to|
  |                                        be searched in the SNI. All|
  |                                        other input arguments are  |
  |                                        ignored.                   |
  |                                                                   |
  |                     ALL_USERS_IN_SNI : A list is created of all   |
  |                                        users in the SNI.          |
  |                                                                   |
  |                     ALL_USERS_LINKED_TO_OBJECT : All user objects |
  |                                        that are linked to the input
  |                                        OCB are returned.          |
  |                                                                   |
  |                     USERS_LINKED_TO_OBJECT_MATCHING_STRING : All  |
  |                                        users that are linked to the
  |                                        input object whose name also
  |                                        matches the search string  |
  |                                        are returned.              |
  |                                                                   |
  |  <PARAM>   params         Byte *  INPUT   Not Used in this version|
  |  <PURPOSE> View user based on the above conditions.               |
  |  <OUTPUT>  sint                                                   |
  |  </doc>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "obj_prim.h"



sint     vw_user( some_ocb, names_from_cmd_line, type_of_search, params )

OCB              *some_ocb;
string            names_from_cmd_line;
hword             type_of_search;
byte             *params;

{

   sint       rc          =  0;

   OCB       *user_ocb    =  NIL,
             *list_ocb    =  NIL;

   OCD       *ocd         =  NIL,
             *list_ocd    =  NIL;

   OID        oid,
              user_oid;

   string
              form_id         =  S_NULL,
              user_req_names  =  S_NULL,
              result_str      =  S_NULL,
              NAMES           =  S_NULL;

   boolean    done            =  FALSE,
              ok_to_search    =  FALSE,
              USERS_TO_VIEW   =  TRUE,
              FROM_CMD_LINE   =  FALSE;


#D_begin(  vw_user,  OBJ  )
#D_str(   'names_from_cmd_line', '' );
#D_show(  'Search Type=%d; OCB=x%x', 'type_of_search, some_ocb' );


   switch( type_of_search )
   {

      case ALL_USERS_IN_SNI :

                           break;

      case SEARCH_FOR_USERS_MATCHING_STRING :

            ocd_new ( & list_ocd, NIL );
            while( ! done )
            {

               if( s_len( names_from_cmd_line ) < 1 )
               {

                  FROM_CMD_LINE  =  FALSE;
                  #D_say( 'No names provided at command line' );

                  s_init( &form_id,  "1110"  );
                  s_init( &NAMES,    "NAMES" );

                  rc = t_form( form_id, S_NULL, NIL         );

                  if( t_ifquit( rc ) )
                  {
                      done = TRUE;

                      f_close( form_id, NO_FLAGS );
                      s_drop ( & form_id );
                      s_drop ( & NAMES   );
                  }
                  else
                  {

                      user_req_names = var_get( NAMES );
                      #D_str( 'user_req_names', 'Ids to look for' );
                      if( s_len( user_req_names ) != 0 )
                      {

                          ok_to_search = TRUE;
                      }
                      else
                      {

                          except( USER_NUMBER_NOT_PROVIDED, USER_ERROR,
                                  S_NULL, S_NULL, S_NULL, S_NULL,
                                                               S_NULL  );

                      }

                  }
               }
               else
               {

                  s_copy( & user_req_names, names_from_cmd_line, 0, 0 );
                  ok_to_search   = TRUE;
                  FROM_CMD_LINE  = TRUE;

               }

               if( ok_to_search == TRUE )
               {

                  rc = cr_list( list_ocb, S_NULL, 0, & list_ocb );


                  if( rc == 0 )
                  {

                     rc = ex_user( list_ocb, user_req_names,
                                           STRING_TO_OID, & result_str );

                     if( rc == 0 )
                     {

                        s_init( &(list_ocd->name), "LIST.LIST.OID" );
                        list_ocd->bias = 1;

                        USERS_TO_VIEW = TRUE;


                        while( USERS_TO_VIEW )
                        {
                           rc = obj_get( list_ocb, list_ocd, &user_oid );
                           #D_oid( '( & user_oid )', 'User OID' );

                           if( rc == 0 )
                           {
                              user_ocb = obj_opn( & user_oid,
                                               OBJ_READ | OBJ_REMOTE   );
                              #D_show('user_ocb_ptr = 0x%X', 'user_ocb');


                              rc = gt_user( user_ocb, S_NULL, (hword)0,
                                                                params );

                              obj_cls( user_ocb );

                              if( rc == USER_REQUESTED_ABANDON )
                              {

                                 #D_say( 'Stop the view process' );
                                 USERS_TO_VIEW = FALSE;
                                 done = TRUE;

                                 if( ! FROM_CMD_LINE )
                                 {

                                   f_close( form_id, NO_FLAGS );
                                   s_drop ( &form_id );
                                   s_drop ( &NAMES   );

                                 }

                              }
                              else if( rc == USER_REQUESTED_QUIT )
                              {

                                 USERS_TO_VIEW = FALSE;

                                 if( ! FROM_CMD_LINE )
                                 {
                                    var_clr( NAMES );
                                    f_rstfld( form_id, NO_FLAGS, NAMES );
                                 }
                                 else
                                 {
                                    done = TRUE;
                                 }

                              }

                           }
                           else
                           {

                              #D_say( 'NO MORE USERS TO VIEW' );
                              #D_say('Possibly, get failed as DESIRED');
                              #D_show( 'rc of obj_get = %d', 'rc' );

                              USERS_TO_VIEW = FALSE;

                              if(  FROM_CMD_LINE  )
                              {
                                 done = TRUE;
                              }
                              else
                              {
                                 var_clr( NAMES );
                                 f_rstfld( form_id, NO_FLAGS, NAMES );
                              }


                           }

                        } /* end of while for USERS_TO_VIEW */

                        obj_set_flag( list_ocb, OBJ_DELETE );
                        obj_cls( list_ocb );

                        list_ocb = NIL;

                     }
                     else
                     {

                        #D_say( 'Bad user names detected.' );
                        except( rc, USER_ERROR, result_str, S_NULL,
                                             S_NULL, S_NULL, S_NULL );
                        if(  FROM_CMD_LINE  )
                        {
                            done = TRUE;
                        }
                     }
                  }
                  else
                  {

                     #D_say( 'CR_LIST failed' );
                     except( rc, OBJ_LEVEL_ERROR, user_req_names,
                                       S_NULL, S_NULL, S_NULL, S_NULL );
                     if(  FROM_CMD_LINE  )
                     {
                         done = TRUE;
                     }
                  }

               }
               else
               {

                  #D_say( 'Returning without even trying to find user' );

               /* done = TRUE; */
               /* should give some error message */

               }

            }

            ocd_drp( & list_ocd  );

            break;

      case ALL_USERS_LINKED_TO_OBJECT :

                           #D_say( 'Link traversal not operational' );

                           break;

      case USERS_LINKED_TO_OBJECT_MATCHING_STRING :

                           #D_say( 'Link match not operational' );

                           break;

      case USER_OBJECT_ONLY :

                           gt_user( some_ocb, S_NULL, (hword)0, params );
                           break;

      default :

                           #D_say( 'Invalid search type requested' );
                           rc = FUNCTION_NOT_SUPPORTED;
                           break;


   }
   s_drop( & result_str     );
   s_drop( & form_id        );
   s_drop( & NAMES          );
   s_drop( & user_req_names );


#D_return( 'rc' );

}
