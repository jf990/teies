/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    grp_req()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster, Rana Ajaz-Ur-Rehman,  Tanmay S. Kumar  |
  |                                                                   |
  |  :DATE.    June 23, 1988                         August 15, 1989  |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   group_ocb   OCB *      INOUT  Pointer to Group         |
  |                                          Object Control Block     |
  |                                          to add components to.    |
  |                                          Allocated in CR_GRUP()   |
  |                                                                   |
  |  :PARAM.   modifier    string     INPUT  not  used.               |
  |                                                                   |
  |  :PARAM.   level       hword      INPUT  not used.                |
  |                                                                   |
  |  :PARAM.   params      byte   *   INOUT                           |
  |                                                                   |
  |                                                                   |
  |  :PURPOSE. Create GROUP      Root. This routine handles the       |
  |            adding of REQURED components to the GROUP     , like   |
  |            GROUP      ID, LABEL, TYPE, CATEGORY, STATUS,...etc.   |
  |            It calls up the "3121 form", validates the user inputs,|
  |            checks if the ID and LABEL of GROUP      wanted by     |
  |            user are already in use or not.                        |
  |                                                                   |
  |  :OUTPUT.  Sint.                                                  |
  |                                                                   |
  |  :CALLS.   s_init(), t_form(), isv_get(), isv_match(), s_comp(),  |
  |            f_setfld(), f_rstfld(), ocd_new(), obj_rep(), obj_enc()|
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_group.h"
#include "obj_user.h"
#include "obj_list.h"




sint grp_req( group_ocb, modifier, level, params )

OCB      *group_ocb;
string    modifier;
hword     level;
byte     *params;

{

sint     rc             = NO_ERR,
         rc2            = NO_ERR,
         var_yn         = 0,
         last_id        = 0,
         junk           = 0;

hword    pub_info       = 0,
         search_type    = 0,
         type           = 0;

boolean  show_form      = TRUE;

string                                  /* Define fields that appear on*/
         GROUP_OWNER    =  S_NULL,      /* form "3121"                 */
         GROUP_ID       =  S_NULL,
         GROUP_LABEL    =  S_NULL,
         GROUP_TYPE     =  S_NULL,
         ACCESS_TYPE    =  S_NULL,
         STATUS_CODE    =  S_NULL,
         TITLE_PUBLIC   =  S_NULL,
         DESC_PUBLIC    =  S_NULL,
         MEMBER_PUBLIC  =  S_NULL,
         CONF_LST_PUB   =  S_NULL,

         group_id       =  S_NULL,
         group_label    =  S_NULL,
         tmp_str        =  S_NULL,
         group_owner    =  S_NULL,
         dot            =  S_NULL,
         suffix         =  S_NULL,
         packet         =  S_NULL,

         form_id        =  S_NULL,
         nickname       =  S_NULL;

OCB     *userocb        = NULL,
        *list_ocb       = NULL;

OCD     *group_ocd      = NULL;

OID     *group_oid      = NULL,
        *grp_oid1       = NULL,
        *grp_oid2       = NULL,
         owner_oid;


#if 0  /* when we've priv function in */
if( prv_chk( SYS_ADMIN) )         /* If system admin then display form */
{                                 /* which has group_id field as input  */

   s_init( &form_id,          "3121"          );

}
else
{

   s_init( &form_id,          "3121"          );

}
#else

s_init( &form_id,          "3121"          );

#endif


s_init( &GROUP_OWNER,      "GROUP_OWNER"        );
s_init( &GROUP_ID,         "GROUP_ID"           );
s_init( &GROUP_LABEL,      "GROUP_LABEL"        );
s_init( &GROUP_TYPE,       "GROUP_TYPE"         );
s_init( &ACCESS_TYPE,      "ACCESS_TYPE"       );
s_init( &STATUS_CODE,      "STATUS_CODE"       );
s_init( &TITLE_PUBLIC,     "TITLE_PUBLIC"      );
s_init( &DESC_PUBLIC,      "DESC_PUBLIC"       );
s_init( &MEMBER_PUBLIC,    "MEMBER_PUBLIC"     );
s_init( &CONF_LST_PUB,     "CONF_LST_PUB"      );

ocd_new( &group_ocd, "GROUP.ID" );

userocb = TM_master->ucb->directory;
if( ! OCB_POINTER_OK( userocb ) )
{


}
else
{

   s_init( &(group_ocd->name), "NAME.NICKNAME" );
   if( obj_get( userocb, group_ocd, &nickname ) == 0 )
   {

      var_set( GROUP_OWNER, nickname );

   }

#if 0    /* till we've priv functions working */
   if( prv_chk(SYSTEM_ADMIN) == FALSE )
   {

      s_init( &(group_ocd->name) "NAME.NUMBER" );
      obj_get( userocb, group_ocd, &group_id );
      s_init( &dot, "." );
      s_cat( &group_id, group_id, dot, 0, 0 );
      obj_get( userocb, "MEMBERSHIPS.LAST_GROUP_ID", &last_id );
      last_id++;
      s_sitos( &suffix, last_id );
      s_cat( &group_id, group_id, suffix, 0, 0 );

      var_set( GROUP_ID, group_id );

      s_drop( &group_id );
      s_drop( &dot     );
      s_drop( &suffix  );

   }
#else

      s_init( &(group_ocd->name), "NAME.NUMBER" );
      obj_get( userocb, group_ocd, &group_id );
      s_init( &dot, "." );
      s_cat( &group_id, group_id, dot, 0, 0 );
      s_init( &(group_ocd->name), "MEMBERSHIPS.LAST_GROUP_ID" );
      obj_get( userocb, group_ocd, &last_id );
      last_id++;
      s_sitos( &suffix, last_id );
      s_cat( &group_id, group_id, suffix, 0, 0 );
                                 /* we should chk to see if the group  */
                                  /* already exists, if so increment   */
                                  /* last_id till we get which does not*/
                                  /* exist..put it in a while loop     */
      var_set( GROUP_ID, group_id );

      s_drop( &group_id );
      s_drop( &dot     );
      s_drop( &suffix  );

#endif

}


while( show_form && (rc == NO_ERR) )
{

   pub_info = 0;

   rc = t_form( form_id, modifier, group_ocb );

   if ( (rc == USER_REQUESTED_ENTER )
       || (rc == USER_REQUESTED_PROCESS ) )
   {
      rc = 0;
      group_owner = var_get( GROUP_OWNER );
      if( s_len( group_owner ) == 0 )
      {

         except( GROUP_OWNER_INVALID, USER_ERROR,
                           group_owner,S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_OWNER, 0.0, 0.0 );
         continue;
      }
      else
      {

         if( s_comp(nickname, group_owner, 0, 0, 'M', &junk) == 0 )
         {

            os_memc(&(TM_master->ucb->user_id), &owner_oid, sizeof(OID));

         }
         else
         {

            cr_list( list_ocb, modifier, level, &list_ocb);
            if( (rc = ex_user( list_ocb, group_owner, STRING_TO_OID,
                                      (string *) &params )) == NO_ERR )
            {

               s_init( &(group_ocd->name), "LIST.LIST.OID" );
               group_ocd->bias = 1;
               group_ocd->key = NULL;

               obj_get( list_ocb, group_ocd, &owner_oid );
               group_ocd->bias = 0;

            }
            else
            {
               except(rc,USER_ERROR,
                               group_owner,S_NULL,S_NULL,S_NULL,S_NULL);
               f_curpos( form_id, NO_FLAGS, GROUP_OWNER, 0.0, 0.0 );
               continue;
            }

         }
         s_init( &(group_ocd->name), "%LINKS.OWNER" );
         obj_rep( group_ocb, group_ocd, &owner_oid, SEG_UPDATE );

      }


#if 0            /* When priv stuff works, check for sys adm,  */
                      /* if yes, then get new ID from the field     */
      if( prvchk(SYSTEM_ADMIN) )
      {

         group_id = var_get( GROUP_ID );

      }
#else

      group_id = var_get( GROUP_ID );
      /* should check for correct format...only digits and dot's allow*/
      /* call s_parse() */

#endif

      if( s_len( group_id ) == 0 )
      {

         except( GROUP_ID_NOT_PROVIDED, USER_ERROR,
                             S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_ID, 0.0, 0.0 );

         continue;
      }
      else
      {
         s_init( &(group_ocd->name), "GROUP.ID" );
         obj_rep( group_ocb, group_ocd, group_id, SEG_UPDATE );

      }

      group_label = var_get( GROUP_LABEL );
      if( s_len( group_label ) == 0 )
      {

         except( GROUP_LABEL_NOT_PROVIDED, USER_ERROR,
                             S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_LABEL, 0.0, 0.0 );

         continue;
      }
      else
      {
         s_init( &(group_ocd->name), "GROUP.LABEL" );
         obj_rep( group_ocb, group_ocd, group_label, SEG_UPDATE );

      }


      tmp_str = var_get( GROUP_TYPE );
      type = isv_match( tmp_str, ISV_MMSG, ISV_REGULAR);
                           /* not a good way, the range of ISV values */
                           /* is too wide, temporary fix, OK          */
      if( type <= 0 )
      {
         except( UNEXPECTED_RESPONSE, USER_ERROR, tmp_str,
                                      S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_TYPE, 0.0, 0.0 );

         continue;
      }
      else
      {
         s_init( &(group_ocd->name), "GROUP.STATUS.TYPE" );
         obj_rep( group_ocb, group_ocd, &type, SEG_UPDATE );

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

         s_init( &(group_ocd->name), "GROUP.STATUS.ACCESS" );
         obj_rep( group_ocb, group_ocd, &type, SEG_UPDATE );

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

         s_init( &(group_ocd->name), "GROUP.STATUS.STATUS" );
         obj_rep( group_ocb, group_ocd, &type, SEG_UPDATE );

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

         pub_info |= G_PUBLIC_TITLE;

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

         pub_info |= G_PUBLIC_DESC;

      }


      tmp_str = var_get( CONF_LST_PUB );
      var_yn = t_valyn( tmp_str );
      if( var_yn == UNEXPECTED_RESPONSE )
      {
         except( Yes_Or_No_Was_Expected, USER_ERROR, tmp_str,
                                      S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, CONF_LST_PUB, 0.0, 0.0 );
         continue;
      }
      else if( var_yn == TRUE )
      {

         pub_info |= G_PUBLIC_CONF;

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

         pub_info |= G_PUBLIC_MEMBER;

      }
      s_init( &(group_ocd->name), "GROUP.STATUS.PUBLIC_INFO" );
      obj_rep( group_ocb, group_ocd, &pub_info, SEG_UPDATE );


                                  /* Whew! Now that we are done making */
                                  /* sure all the input is correct,    */
                                  /* find out from MS that if ID and   */
                                  /* LABEL are not already taken       */
      search_type = CHECK_GROUP_ID | G_NEED_ONE_OID;
      rc  = ex_grup( NIL, group_id, search_type, (void **)&grp_oid1);
      search_type = CHECK_GROUP_LABEL | G_NEED_ONE_OID;
      rc2 = ex_grup( NIL, group_label, search_type, (void **)&grp_oid2 );

      if( (rc == GROUP_ID_NOT_IN_SGI) && (rc2 == GROUP_LABEL_NOT_IN_SGI) )
      {
         show_form = FALSE;       /* We're done...so get out           */
         rc = NO_ERR;           /* Say we can create our group now!   */
         rc2 = NO_ERR;
      }
      else
      if( (rc == GROUP_ID_ALREADY_IN_SGI) &&
                                   (rc2 == GROUP_LABEL_ALREADY_IN_SGI) )
      {

         except( GROUP_ID_AND_LABEL_ALREADY_IN_SGI, USER_ERROR,
                 group_id, S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_ID, 0.0, 0.0 );

         rc = NO_ERR;
         rc2 = NO_ERR;
         continue;

      }
      else
      if( rc == GROUP_ID_ALREADY_IN_SGI )
      {

         except( GROUP_ID_ALREADY_IN_SGI, USER_ERROR, group_id,
                                 S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_ID, 0.0, 0.0 );
         rc = NO_ERR;
         rc2 = NO_ERR;
         continue;
      }
      else
      if( rc2 == GROUP_LABEL_ALREADY_IN_SGI )
      {

         except( GROUP_LABEL_ALREADY_IN_SGI, USER_ERROR, group_label,
                                 S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_LABEL, 0.0, 0.0 );
         rc = NO_ERR;
         rc2 = NO_ERR;
         continue;
      }
      else
      {

         except( rc2, USER_ERROR, group_label,
                                 S_NULL, S_NULL, S_NULL, S_NULL );
         f_curpos( form_id, NO_FLAGS, GROUP_LABEL, 0.0, 0.0 );

         rc = NO_ERR;
         rc2 = NO_ERR;
         continue;
      }

   }  /* else what we received from t_form() */

}  /* while */


s_drop( &form_id        );

s_drop( &GROUP_OWNER     );
s_drop( &GROUP_LABEL     );
s_drop( &GROUP_TYPE      );
s_drop( &ACCESS_TYPE    );
s_drop( &STATUS_CODE    );
s_drop( &TITLE_PUBLIC   );
s_drop( &DESC_PUBLIC    );
s_drop( &MEMBER_PUBLIC  );
s_drop( &CONF_LST_PUB   );

ocd_drp( &group_ocd );

if( OCB_POINTER_OK(list_ocb) )
{

   obj_set_flag( list_ocb, OBJ_DELETE );
   obj_cls( list_ocb );

}


   return(rc);

}

