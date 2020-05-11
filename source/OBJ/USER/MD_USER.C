/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    md_user                                                |
  |  <AUTHOR>  John L. Foster, Rana Ajaz                              |
  |  <DATE>    December 30, 1988.                                     |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   some_ocb    OCB *     not used.                        |
  |  <PARAM>   names_from_cmd_line                                    |
  |                        string    input  ( not a must can be NULL) |
  |  <PARAM>   level       hword     not used                         |
  |  <PARAM>   params      byte *    not used                         |
  |  <PURPOSE> Modify User. If the name of the user is not provided   |
  |            at the command line, then a form 4110 is called        |
  |            where user is requested to provide the id/name to      |
  |            be modified otherwise form 4111 is called on which     |
  |            modifications can be made to the aleady existing       |
  |            entries.                                               |
  |  <OUTPUT>  returns the proper function key return code or         |
  |            the error code if any.                                 |
  |  <LIMITS>  Right now a user can only modify his/her own entry     |
  |            changes are needed to allow the system operator to     |
  |            be able to modify any user entry.                      |
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
#include "obj_prim.h"
#include "exc.h"


sint     md_user( some_ocb, names_from_cmd_line, level, params )

OCB      *some_ocb;
string    names_from_cmd_line;
hword     level;
byte     *params;

{

   sint     rc       =  0,
            rc_form  =  0;

   OCB     *user_ocb =  NIL,
           *list_ocb =  NIL;

   OCD     *ocd      =  NIL,
           *list_ocd =  NIL;

   OID     *current_user_oid,
            user_oid;

   string   quit             =  S_NULL,
            abandon          =  S_NULL,
            form_1           =  S_NULL,
            form_2           =  S_NULL,
            form_3           =  S_NULL,
            user_req_names   =  S_NULL,
            NAMES            =  S_NULL,
            result_str       =  S_NULL,
            err_message      =  S_NULL;

   sint     R                =  0;

   boolean  done             =  FALSE,
            ok_to_search     =  FALSE,
            FROM_CMD_LINE    =  TRUE,
            USERS_TO_MODIFY  =  TRUE;


isv_get( &quit,    ISV_QUIT,    0 );
isv_get( &abandon, ISV_ABANDON, 0 );


while( ! done )
{
  ok_to_search = FALSE;


  if( s_len( names_from_cmd_line ) < 1 )
  {

    FROM_CMD_LINE = FALSE;


    s_init( &form_1, "4110" );
    s_init( &NAMES, "NAMES" );

    rc_form = t_form( form_1, S_NULL, NIL               );

    if( ! t_ifquit( rc_form ) )
    {

      user_req_names = var_get( NAMES );

      if( s_len( user_req_names ) != 0 )
      {

        ok_to_search = TRUE;

      }
      else
      {
        s_init( &err_message, "No Names/IDs provided" );

        except( USER_NUMBER_NOT_PROVIDED, USER_ERROR,
                err_message, S_NULL, S_NULL, S_NULL, S_NULL );
        s_drop( & err_message );

      }

    } /* ! t_ifquit on 4110 */
    else
    {

      f_close( form_1, NO_FLAGS );
      s_drop( & form_1 );
      s_drop( & NAMES  );
      done = TRUE;
      rc   = rc_form;

    }

  } /* s_len names_from_cmd_line < 1 */
  else
  {

    s_copy( & user_req_names, names_from_cmd_line, 0, 0 );
    FROM_CMD_LINE = TRUE;
    ok_to_search  = TRUE;

  }

  if( ok_to_search == TRUE )
  {

    current_user_oid = & ( TM_master->ucb->user_id );


    rc = cr_list( list_ocb, S_NULL, 0, & list_ocb );

    if( rc == 0 )
    {

      rc = ex_user( list_ocb, user_req_names, STRING_TO_OID,
                                              & result_str   );
      if( rc == 0 )
      {

        ocd_new( & list_ocd, "LIST.LIST.OID" );
        list_ocd->bias  = 1;

        USERS_TO_MODIFY = TRUE;



        while( USERS_TO_MODIFY )
        {

          rc = obj_get( list_ocb, list_ocd, & user_oid );
          if( rc == 0 )
          {

            if( oid_pcmp( & user_oid, current_user_oid )  )
            {



             string     number             = S_NULL,
                        USER_NUMBER        = S_NULL,
                        lastname           = S_NULL,
                        USER_LASTNAME      = S_NULL,
                        firstname          = S_NULL,
                        USER_FIRSTNAME     = S_NULL,
                        userid             = S_NULL,
                        NAME               = S_NULL,
                        DATE               = S_NULL,
                        modifier           = S_NULL,
                        packet             = S_NULL,
                        name_holder        = S_NULL,
                        result_str         = S_NULL,

                        date_str           = S_NULL;

             sint       end_function       = 0;


             byte                *i        =    NIL;
             boolean    linked             =  FALSE;
             real                 x        =    0.0,
                                  y        =    0.0;



             user_ocb = obj_opn( &user_oid, OBJ_UPDATE | OBJ_REMOTE );



             while ( ! linked )
             {

               s_init( &form_2, "4111" );

               end_function = t_form( form_2, modifier, user_ocb );

               if( t_ifquit( end_function ) )
               {

                 linked = TRUE;
                 USERS_TO_MODIFY = FALSE;

                 f_close( form_2, NO_FLAGS );
                 s_drop( &form_2           );


                 if( end_function == USER_REQUESTED_ABANDON )
                 {

                   done = TRUE;

                   if( ! FROM_CMD_LINE )
                   {
                      f_close( form_1, NO_FLAGS );
                      s_drop( & form_1 );
                      s_drop( & NAMES  );
                   }

                 }

               } /* t_ifquit on 4111 form */
               else
               {

                 s_init( & USER_NUMBER,  "NAME.NUMBER" );
                 ocd_new( & ocd, "NAME.NUMBER"         );
                 obj_get( user_ocb, ocd, & number      );

                 if( s_len( number ) == 0 )
                 {

                   except( USER_NUMBER_NOT_PROVIDED, USER_ERROR,
                           number, S_NULL, S_NULL, S_NULL, S_NULL  );
                   f_curpos( form_2, NO_FLAGS, USER_NUMBER,
                                                          0.0, 0.0 );

                   continue;

                 }

                 s_init( & USER_LASTNAME, "NAME.LAST" );
                 s_init( &(ocd->name), "NAME.LAST"    );
                 obj_get( user_ocb, ocd, &lastname    );

                 if( s_len( lastname ) == 0 )
                 {


                   except(INCOMPLETE_NAME_SPECIFIED, USER_ERROR,
                          lastname, S_NULL, S_NULL, S_NULL, S_NULL );
                   f_curpos( form_2, NO_FLAGS, USER_LASTNAME,
                                                          0.0, 0.0 );

                   continue;

                 }

                 s_init( & USER_FIRSTNAME, "NAME.FIRST" );
                 s_init( &(ocd->name), "NAME.FIRST"     );
                 obj_get( user_ocb, ocd, &firstname     );

                 if( s_len( firstname ) == 0 )
                 {

                     except( INCOMPLETE_NAME_SPECIFIED, USER_ERROR,
                             S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
                     f_curpos(form_2, NO_FLAGS, USER_FIRSTNAME,
                                                           0.0, 0.0 );

                     continue;
                 }




                 rc = lk_user( user_ocb, packet, LINK_FOR_MODIFY_USER,
                                                      & result_str );

                 switch (  rc  )
                 {

                    case 0   :

                               linked = TRUE;


                               s_init( & form_3,  "4112"      );
                               s_init( & NAME,    "NAME"      );
                               s_init( & DATE,    "DATE"      );
                               var_set(  NAME,    result_str  );
                               t_gtdate( & date_str, NIL      );
                               var_set(  DATE,    date_str    );
                               t_form(   form_3,  S_NULL, user_ocb );
                               f_close(  form_3,  NO_FLAGS    );
                               var_clr(  NAME     );
                               var_clr(  DATE     );
                               s_drop( & form_3   );
                               s_drop( & NAME     );
                               s_drop( & DATE     );
                               s_drop( & date_str );


                               break;

                    case USER_ALREADY_EXISTS :

                               linked = FALSE;
                               break;

                    default  :

                               break;

                 }

                 if( rc > 0 )
                 {

                   except( rc, USER_ERROR, result_str,
                                    S_NULL, S_NULL, S_NULL, S_NULL );

                 }

                 ocd_drp( & ocd );
                 s_drop( & USER_NUMBER   );
                 s_drop( & USER_LASTNAME );
                 s_drop( & USER_FIRSTNAME);

               } /* not t_ifquit on 4111 form */

             }  /* while not linked */

            }  /* if oid_pcmp */
            else
            {

              except( CANNOT_MODIFY_THIS_USER, USER_ERROR,
                     user_req_names, S_NULL, S_NULL, S_NULL, S_NULL );

              USERS_TO_MODIFY = FALSE;
              if( FROM_CMD_LINE )
              {

                  done = TRUE;

              }

            }

          }  /* rc of obj_get for user OID = 0 */
          else
          {

             USERS_TO_MODIFY = FALSE;
             if( FROM_CMD_LINE )
             {

                done = TRUE;

             }

          }

        }   /* end of while users to modify */

        ocd_drp( & list_ocd );
        obj_set_flag( list_ocb, OBJ_DELETE );
        obj_cls( list_ocb );
        list_ocb = NIL;

      }  /* rc of ex_user = 0 */
      else
      {

        except( rc, USER_ERROR, names_from_cmd_line, S_NULL,
                                     S_NULL, S_NULL, S_NULL );
        if( FROM_CMD_LINE )
        {

           done = TRUE;

        }

      }

    } /* rc of cr_list = 0 */
    else
    {

      except( rc, OBJ_LEVEL_ERROR, S_NULL, S_NULL, S_NULL,
                                           S_NULL, S_NULL );
      if( FROM_CMD_LINE )
      {

          done = TRUE;

      }

    }

  }  /* if ok to search */
  else
  {

    done = TRUE;

  }

}


   return(rc);

}
