/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>d   cfr_gtreq()                                            |
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman                    |
  |  <DATE>    June 23, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INOUT  Pointer to Conference    |
  |                                          Object Control Block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_CR()     |
  |  <PARAM>   modifier    string     INPUT  not  used.               |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   cr_params   struct *   INOUT  defined in obj_prm1.h.   |
  |                                          Only the "control" field |
  |                                          of this structure is in  |
  |                                          use here                 |
  |                                          (i.e. cr_params->control)|
  |                                                                   |
  |                                                                   |
  |  <PURPOSE> Create Conference Root. This routine handles the       |
  |            adding of REQURED components to the Conference, like   |
  |            Conference ID, LABEL, TYPE, CATEGORY, STATUS,...etc.   |
  |            It calls up the "3141 form", validates the user inputs,|
  |            checks if the ID and LABEL of conference wanted by     |
  |            user are already in use or not, implements function    |
  |            keys, and returns a proper code depending upon the     |
  |            value of cr_params->control so that the next routine   |
  |            or form could be called in the process.                |
  |                                                                   |
  |  <OUTPUT>  returns  1  if QUIT requested, so that CFR_INFO calld  |
  |            returns -1  if ABANDON is requested                    |
  |            returns  1  if "control" is 8, so that CFR_INFO calld  |
  |            returns  5  if "control" is 9, so that t_jmpst() called|
  |                                           which calls cr_memb()   |
  |            returns  4  if "control" is 10 or 11                   |
  |                                     so that CFR_GTDES is called   |
  |                                                                   |
  |                                                                   |
  |  <CALLS>   s_init(), t_form(), isv_get(), isv_match(), s_comp(),  |
  |            f_setfld(), f_rstfld(), ocd_new(), obj_rep(), obj_enc()|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_cr.h"
#include "obj_user.h"
#include "obj_list.h"




sint cfr_gtreq( conf_ocb, modifier, level, params )

OCB      *      conf_ocb;
string          modifier;
hword           level;
byte     *      params;

{

   sint     rc             = NO_ERR,
            rc2            = NO_ERR,
            var_yn         = 0,
            last_id        = 0,
            junk           = 0;
   sint     rc_form        = 0;

   hword    pub_info       = 0,
            type           = 0;

   boolean  show_form      = TRUE;

   string   MEM_GR_ID      =  S_NULL,   /* Define fields that appear on*/
            CONF_OWNER     =  S_NULL,   /* form "3141"                 */
            CONF_ID        =  S_NULL,
            CONF_LABEL     =  S_NULL,
            CONF_TYPE      =  S_NULL,
            ACCESS_TYPE    =  S_NULL,
            CATEGORY       =  S_NULL,
            STATUS_CODE    =  S_NULL,
            TITLE_PUBLIC   =  S_NULL,
            DESC_PUBLIC    =  S_NULL,
            MEMBER_PUBLIC  =  S_NULL,
            DEFAULT_STRUCT =  S_NULL,
            DEFAULT_ACTVTY =  S_NULL,

            conference_id  =  S_NULL,
            conference_label= S_NULL,
            tmp_str        =  S_NULL,
            conf_id        =  S_NULL,
            conf_owner     =  S_NULL,
            dot            =  S_NULL,
            suffix         =  S_NULL,
            packet         =  S_NULL,
            form_id        =  S_NULL,
            nickname       =  S_NULL,
            user_number    =  S_NULL,
            bad_owner_name =  S_NULL;

   OCB    * owner_ocb      = NULL,
          * list_ocb       = NULL;

   OCD     *conf_ocd       = NULL;
   OCD     *user_ocd       = NULL;
   OCD     *list_ocd       = NULL;

   OID     *conf_oid       = NULL,
            user_oid       = NULL;


   s_init( & form_id,          "314A"              );
   s_init( & MEM_GR_ID,        "MEM_GR_ID"         );
   s_init( & CONF_OWNER,       "CONF_OWNER"        );
   s_init( & CONF_ID,          "CONF_ID"           );
   s_init( & CONF_LABEL,       "CONF_LABEL"        );
   s_init( & CONF_TYPE,        "CONF_TYPE"         );
   s_init( & ACCESS_TYPE,      "ACCESS_TYPE"       );
   s_init( & CATEGORY,         "CATEGORY"          );
   s_init( & STATUS_CODE,      "STATUS_CODE"       );
   s_init( & TITLE_PUBLIC,     "TITLE_PUBLIC"      );
   s_init( & DESC_PUBLIC,      "DESC_PUBLIC"       );
   s_init( & MEMBER_PUBLIC,    "MEMBER_PUBLIC"     );
   s_init( & DEFAULT_STRUCT,   "DEFAULT_STRUCT"    );
   s_init( & DEFAULT_ACTVTY,   "DEFAULT_ACTVTY"    );

   ocd_new( & conf_ocd, "CONFERENCE.ID" );
   ocd_new( & user_ocd, NIL             );

   owner_ocb = TM_master->ucb->directory;
   if( OCB_POINTER_OK( owner_ocb ) )
   {

      s_init( &(conf_ocd->name), "NAME.NICKNAME" );
      if( obj_get( owner_ocb, conf_ocd, & nickname ) == 0 )
      {

         var_set( MEM_GR_ID,  nickname );

      }
   }

   s_init(  & (conf_ocd->name), "NAME.NUMBER" );
   obj_get( owner_ocb, conf_ocd, & user_number);
   var_set( CONF_OWNER, user_number           );
   s_copy(  & conf_id,  user_number,     0, 0 );
   s_init(  & dot, "."                        );
   s_cat(   & conf_id, conf_id, dot,     0, 0 );
   s_init(  & (conf_ocd->name), "MEMBERSHIPS.LAST_CONF_ID" );
   obj_get( owner_ocb, conf_ocd, & last_id    );
   last_id++;
   s_sitos( & suffix, last_id                 );
   s_cat(   & conf_id, conf_id, suffix,  0, 0 );
                            /* = = = = = = = = = = = = = = = = = */
                            /* we should chk to see if the conf  */
                            /* already exists, if so increment   */
                            /* last_id till we get which does not*/
                            /* exist..put it in a while loop     */
                            /* = = = = = = = = = = = = = = = = = */

   var_set( CONF_ID, conf_id );

   s_drop( & conf_id );
   s_drop( & dot     );
   s_drop( & suffix  );


   while( show_form && (rc == NO_ERR) )
   {

      pub_info = 0;

      rc_form = t_form( form_id, modifier, conf_ocb );

      switch(  rc_form  )
      {


         case FORM_NOT_FORMED        :
         case USER_REQUESTED_ABANDON :
         case USER_REQUESTED_QUIT    :

            rc = rc_form;
            break;

         default:

            conf_owner = var_get( CONF_OWNER );
            if( s_len( conf_owner ) == 0 )
            {

               except( CONF_OWNER_INVALID, USER_ERROR,
                             conf_owner,S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form_id, NO_FLAGS, CONF_OWNER, 0.0, 0.0 );
               continue;

            }
            else
            {

               if( s_comp( user_number, conf_owner, 0, 0, 'M', NIL) == 0)
               {

                  os_memc( & ( TM_master->ucb->user_id ),
                                             & user_oid, sizeof( OID ) );

               }
               else
               {

                  rc = t_exobj( & list_ocb, USER_OBJ,
                           conf_owner, STRING_TO_OID, & bad_owner_name );
                  if ( rc == NO_ERR )
                  {

                     s_init( &(conf_ocd->name), "LIST.LIST.OID" );
                     conf_ocd->bias = 1;
                     conf_ocd->key = NULL;

                     obj_get( list_ocb, conf_ocd, & user_oid );
                     conf_ocd->bias = 0;

                  }
                  else
                  {

                     except( rc, USER_ERROR, conf_owner,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
                     f_curpos( form_id, NO_FLAGS, CONF_OWNER, 0.0, 0.0 );
                     rc = NO_ERR;
                     continue;

                  }
               }
               s_init( &(conf_ocd->name), "%LINKS.OWNER" );
               obj_rep( conf_ocb, conf_ocd, & user_oid, SEG_UPDATE );

            }
                                 /* = = = = = = = = = = = = = = = = = */
                                 /* should check for correct format of*/
                                 /* the conference ID.                */
                                 /* only digits and dots are allowed. */
                                 /* = = = = = = = = = = = = = = = = = */

         conference_id = var_get( CONF_ID );
         if( s_len( conference_id ) == 0 )
         {

            except( CONF_ID_NOT_PROVIDED, USER_ERROR,
                                S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, CONF_ID, 0.0, 0.0 );
            continue;

         }
         else
         {

            s_init( &(conf_ocd->name), "CONFERENCE.ID" );
            obj_rep( conf_ocb, conf_ocd, conference_id, SEG_UPDATE );

         }

         conference_label = var_get( CONF_LABEL );
         if( s_len( conference_label ) == 0 )
         {

            except( CONF_LABEL_NOT_PROVIDED, USER_ERROR,
                                S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, CONF_LABEL, 0.0, 0.0 );
            continue;

         }
         else
         {

            s_init( &(conf_ocd->name), "CONFERENCE.LABEL" );
            obj_rep( conf_ocb, conf_ocd, conference_label,
               SEG_UPDATE );

         }


         tmp_str = var_get( CONF_TYPE );
         type = isv_match( tmp_str, ISV_DISCUSSI, ISV_SIMULATI );
         if( type <= 0 )
         {

            except( UNEXPECTED_RESPONSE, USER_ERROR, tmp_str,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, CONF_TYPE, 0.0, 0.0 );
            continue;

         }
         else
         {

            s_init( &(conf_ocd->name), "CONFERENCE.STATUS.TYPE" );
            obj_rep( conf_ocb, conf_ocd, &type, SEG_UPDATE );

         }


         tmp_str = var_get( ACCESS_TYPE );
         type = isv_match( tmp_str, ISV_PRIVATE, ISV_PUBLIC );
         if( type <= 0 )
         {
            except( UNEXPECTED_RESPONSE, USER_ERROR, tmp_str,
               S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, ACCESS_TYPE, 0.0, 0.0 );

            continue;
         }
         else
         {

            s_init( &(conf_ocd->name), "CONFERENCE.STATUS.ACCESS" );
            obj_rep( conf_ocb, conf_ocd, &type, SEG_UPDATE );

         }


         tmp_str = var_get( CATEGORY);
         type = isv_match( tmp_str, ISV_PERSONAL, ISV_MONITORI );
         if( type <= 0 )
         {
            except( UNEXPECTED_RESPONSE, USER_ERROR, tmp_str,
               S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, CATEGORY, 0.0, 0.0 );

            continue;
         }
         else
         {

            s_init( &(conf_ocd->name), "CONFERENCE.STATUS.CATEGORY" );
            obj_rep( conf_ocb, conf_ocd, &type, SEG_UPDATE );

         }


         tmp_str = var_get( STATUS_CODE );
         type = isv_match( tmp_str, ISV_OPEN, ISV_CLOSED );
         if( type <= 0 )
         {
            except( UNEXPECTED_RESPONSE, USER_ERROR, tmp_str,
               S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, STATUS_CODE, 0.0, 0.0 );

            continue;
         }
         else
         {

            s_init( &(conf_ocd->name), "CONFERENCE.STATUS.STATUS" );
            obj_rep( conf_ocb, conf_ocd, &type, SEG_UPDATE );

         }


         tmp_str = var_get( TITLE_PUBLIC );
         var_yn = t_valyn( tmp_str );
         if( var_yn == UNEXPECTED_RESPONSE )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, tmp_str,
               S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, TITLE_PUBLIC, 0.0, 0.0 );

            continue;
         }
         else if( var_yn == TRUE )
         {

            pub_info |= PUBLIC_TITLE;

         }


         tmp_str = var_get( DESC_PUBLIC );
         var_yn = t_valyn( tmp_str );
         if( var_yn == UNEXPECTED_RESPONSE )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, tmp_str,
               S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, DESC_PUBLIC, 0.0, 0.0 );

            continue;
         }
         else if( var_yn == TRUE )
         {

            pub_info |= PUBLIC_DESC;

         }


         tmp_str = var_get( MEMBER_PUBLIC );
         var_yn = t_valyn( tmp_str );
         if( var_yn == UNEXPECTED_RESPONSE )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, tmp_str,
                S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, MEMBER_PUBLIC, 0.0, 0.0 );

            continue;
         }
         else if( var_yn == TRUE )
         {

            pub_info |= PUBLIC_MEMBER;
            s_init( &(conf_ocd->name),
               "CONFERENCE.STATUS.PUBLIC_INFO" );
            obj_rep( conf_ocb, conf_ocd, &pub_info, SEG_UPDATE );

         }


         tmp_str = var_get( DEFAULT_STRUCT );
         var_yn = t_valyn( tmp_str );
         if( var_yn == UNEXPECTED_RESPONSE )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, tmp_str,
              S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, DEFAULT_STRUCT, 0.0, 0.0 );

            continue;
         }
         else if( var_yn == TRUE )
         {
#if 0
            s_init( &(conf_ocd->name),
               "CONFERENCE.STATUS.STRUCTURES" );
            I don't know what code should go here
#endif
         }


         tmp_str = var_get( DEFAULT_ACTVTY );
         var_yn = t_valyn( tmp_str );
         if( var_yn == UNEXPECTED_RESPONSE )
         {
            except( Yes_Or_No_Was_Expected, USER_ERROR, tmp_str,
             S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, DEFAULT_ACTVTY, 0.0, 0.0 );

            continue;
         }
         else if( var_yn == TRUE )
         {

#if 0
            s_init( &(conf_ocd->name),
                "CONFERENCE.STATUS.ACTIVITIES" );
            I don't know what code should go here
#endif

         }
                            /* = = = = = = = = = = = = = = = = = */
                            /* Whew! Now that we are done making */
                            /* sure all the input is correct,    */
                            /* find out from MS that if ID and   */
                            /* LABEL are not already taken       */
                            /* = = = = = = = = = = = = = = = = = */

         tmp_str = S_NULL;
         s_catx( & tmp_str, conference_id,    0, 0 );
         s_catx( & tmp_str, conference_label, 0, 0 );

         rc  = ex_cr( NULL, tmp_str, CHK_CONF_ID|CHK_CONF_LABEL, NULL );

         if( rc == CONF_ID_AND_LABEL_NOT_IN_CRI )
         {
            show_form = FALSE;      /* We're done...so get out         */
            rc = NO_ERR;            /* Say we can create our conf now! */

         }
         else
         if(    (rc == CONF_ID_AND_LABEL_ALREADY_IN_CRI)
             || (rc == CONF_ID_ALREADY_IN_CRI          )   )
         {

            except(rc,USER_ERROR,conference_id,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, CONF_ID, 0.0, 0.0 );
            rc = NO_ERR;
            continue;

         }
         else
         if( rc == CONF_LABEL_ALREADY_IN_CRI )
         {

            except( CONF_LABEL_ALREADY_IN_CRI, USER_ERROR,
                      conference_label, S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, CONF_LABEL, 0.0, 0.0 );
            rc = NO_ERR;
            continue;

         }
         break;

      }  /* switch of rc_form  */
   }  /* while */

   f_close(   form_id, STATE_FORM );
   s_drop(  & form_id        );
   s_drop(  & MEM_GR_ID      );
   s_drop(  & CONF_OWNER     );
   s_drop(  & CONF_LABEL     );
   s_drop(  & CONF_TYPE      );
   s_drop(  & ACCESS_TYPE    );
   s_drop(  & CATEGORY       );
   s_drop(  & STATUS_CODE    );
   s_drop(  & TITLE_PUBLIC   );
   s_drop(  & DESC_PUBLIC    );
   s_drop(  & MEMBER_PUBLIC  );
   s_drop(  & DEFAULT_STRUCT );
   s_drop(  & DEFAULT_ACTVTY );
   s_drop(  & bad_owner_name );
   s_drop(  & nickname       );
   s_drop(  & user_number    );
   ocd_drp( & conf_ocd       );
   ocd_drp( & list_ocd       );
   ocd_drp( & user_ocd       );

   if( OCB_POINTER_OK(  list_ocb  ) )
   {

      obj_set_flag( list_ocb, OBJ_DELETE );
      obj_cls( list_ocb );

   }


   return(rc);

}
