/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    gt_cr                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 24, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf        OCB *      INPUT  description              |
  |  <PARAM>   modifier    string     INPUT  description              |
  |  <PARAM>   flags       hword      INPUT  description              |
  |  <PARAM>   cc_count    sint       INPUT  description              |
  |  <PURPOSE> Get Conference Root                                    |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "obj_cr.h"
#include "obj_cc.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "isv.h"
#include "obj_memb.h"


sint     gt_cr( conf_ocb, modifier, flags, cc_count )

OCB            *conf_ocb;
string          modifier;
hword           flags;
sint            cc_count;

{

string       tmp_str     =  S_NULL,
             NAMES       =  S_NULL,
             CC_COUNT    =  S_NULL,
             TOT_MEMB    =  S_NULL,
             TOT_WRITER  =  S_NULL,
             cc_count_str=  S_NULL,
             tot_memb    =  S_NULL,
             tot_writer  =  S_NULL,
             new_role    =  S_NULL,
             cc_id       =  S_NULL,
             form_id     =  S_NULL;
OCB        * memb_ocb    =    NIL,
           * cc_ocb      =    NIL,
           * list_ocb    =    NIL,
           * user_ocb    =    NIL;
OCD        * conf_ocd    =    NIL,
           * user_ocd    =    NIL,
           * memb_ocd    =    NIL;
hword        access_type =      0,
             rc_form     =      0,
             type_isv    =      0;
OID          memb_oid            ,
             user_oid            ,
             cc_oid              ;
datetime     last_active         ;
PRIVS        roles               ;
sint         rc          =      0,
             count       =      0,
             seqnum      =      0;
boolean      done        =  FALSE,
             GETTING_CCID=   TRUE,
             called_cc_gtid = FALSE,
             formed_cr   =  FALSE;





if( flags & VIEWING_COMMENT )
{


}
else         /* if coming from VW_CC, modifier is an cc_ocb */
{


}

ocd_new( & conf_ocd, "CONFERENCE.MEMBERS"       );
ocd_new( & memb_ocd, "MEMBERSHIP.MEMBERS.ROLES" );

if( ! OCB_POINTER_OK( conf_ocb ) )
{

   rc = vw_cr( NULL, modifier, flags,  NULL  );

}
else
{                                 /* Chk if user is allowed to access */
                                  /* this conference                  */

   if( (rc = obj_get( conf_ocb, conf_ocd, & memb_oid )) == NO_ERROR )
   {

      memb_ocb = obj_opn( & memb_oid, OBJ_UPDATE | OBJ_REMOTE );

   }

   if( ! OCB_POINTER_OK(  memb_ocb  ) )
   {

      rc = CONF_DOES_NOT_HAVE_MEMBERS;
      except( rc, USER_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );

   }
   else
   {

      user_ocb = t_author();

      oid_key(      memb_ocd, & (user_ocb->oid) );
      rc = obj_get( memb_ocb, memb_ocd, & roles );
      if( rc == NO_DATA_FOUND )   /* user not in conf membership list  */
      {                           /* check if conf public              */

         s_init(      &( conf_ocd->name), "CONFERENCE.STATUS.ACCESS" );
         obj_set_flag(   conf_ocb, OBJ_RAW );
         obj_get(        conf_ocb, conf_ocd, & access_type  );
         obj_reset_flag( conf_ocb, OBJ_RAW );
         if( access_type == ISV_PUBLIC )
         {

            rc     = NO_ERROR;
            flags |= JOINING_PUBLIC_CONF;

         }
         else
         {

            rc = CONF_ACCESS_NOT_ALLOWED;

         }
      }
   }  /* if there are any members */

   if( rc == NO_ERROR )
   {
                                  /*================+|+================*/
                                  /* If user is allowed to access conf */
                                  /* If we already know user is not in */
                                  /* conf and is joining the public    */
                                  /* conf, then we don't really need   */
                                  /* to chk when he was last_active    */
                                  /*================+|+================*/

      os_memn( & last_active, sizeof( datetime ) );
      if( (flags & JOINING_PUBLIC_CONF) == 0 )
      {

         s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.LAST_ACTIVE" );
         rc = obj_get( memb_ocb, memb_ocd, & last_active );

      }
      if( (rc == NO_DATA_FOUND) || (last_active.century == 0) )
      {                                 /* TWEAK! To see if user was   */
                                        /* ever active in the conf?    */
         if( (flags & (CREATING_COMMENT | VIEWING_COMMENT)) == 0 )
         {

            rc = jn_cr( conf_ocb, modifier, flags, memb_ocb );

            ocd_new( & user_ocd, "MEMBERSHIPS.CONFERENCES.NEW_CC_COUNT");
            s_init( & (conf_ocd->name), "CONFERENCE.ID" );
            obj_get(   conf_ocb, conf_ocd, &(user_ocd->key) );
            obj_get(   user_ocb, user_ocd, &cc_count );
            ocd_drp( & user_ocd );

         }
      }
      if( rc == NO_ERROR )
      {

         if( flags & VIEWING_COMMENT )
         {

            rc = gt_cc( (OCB *) modifier, conf_ocb, flags, memb_ocb );

         }
         else if( flags & CREATING_COMMENT )
         {

            rc = cr_cc( conf_ocb, (string) memb_ocb, flags, NIL );

         }
         else
         {

            s_init (  & CC_COUNT,    "CC_COUNT"             );
            s_sitos ( & cc_count_str, cc_count              );
            var_set(    CC_COUNT,     cc_count_str          );
            s_init (  & TOT_MEMB,    "TOT_MEMB"             );
            s_init (  &(memb_ocd->name), "MEMBERSHIP.COUNT" );
            obj_get(    memb_ocb,     memb_ocd,   & count   );
            s_sitos ( & tot_memb,     count                 );
            var_set(    TOT_MEMB,     tot_memb              );

            s_init (  & TOT_WRITER,  "TOT_WRITER"            );
            s_init (  &(memb_ocd->name), "MEMBERSHIP.WRITER" );
            obj_get(    memb_ocb,     memb_ocd,   & count    );
            s_sitos ( & tot_writer,   count                  );
            var_set(    TOT_WRITER,   tot_writer             );

            s_init ( & form_id, "1141"         );
            done     = FALSE;          /* We want to go thru WHILE loop*/
            formed_cr=  TRUE;
            rc       =     0;

         }
      }

      while( ( ! done )  &&  (rc == NO_ERROR) )
      {

         if( flags & SCAN_ALL_CONFS )
         {

            rc_form = USER_REQUESTED_SCAN;
            done    =                TRUE;

         }
         else
         {

            if(( (sint) rc_form != FORM_NOT_FORMED )  &&
               (        rc_form != 0               )     )
            {

               f_close( form_id, STATE_FORM );

            }
            rc_form = t_form( form_id, S_NULL, conf_ocb );

         }
         switch(  rc_form  )
         {

            case FORM_NOT_FORMED        :
            case USER_REQUESTED_ABANDON :
            case USER_REQUESTED_QUIT    :

               rc   = rc_form;
               done =    TRUE;
               break         ;

            case USER_REQUESTED_CONFIRM:

               done = FALSE;          /* Continue to present conference*/
               rc = vw_memb( memb_ocb, modifier, flags, conf_ocb );

               if( rc == USER_REQUESTED_QUIT )
               {

                  rc = NO_ERROR;

               }
               break;


            case USER_REQUESTED_SCAN    :
            case USER_REQUESTED_ENTER   :
            case USER_REQUESTED_PROCESS :

               done   = FALSE;
               flags |= SCAN_ALL_COMMENT;
               rc     = vw_cc( conf_ocb, S_NULL, flags, memb_ocb );
               flags &= ~SCAN_ALL_COMMENT;             /* Turn bit off */

               if(  (rc == USER_REQUESTED_QUIT) || User_Level_Error( rc )  )
               {

                  rc = NO_ERROR;

               }
               break;

            case USER_REQUESTED_REPLY :

               done   =  FALSE;
               rc     = cr_cc( conf_ocb, (string) memb_ocb, flags, NIL );
               if( rc == USER_REQUESTED_QUIT )
               {

                  rc = NO_ERROR;

               }
               break;

            case USER_REQUESTED_PRINT:

               done         = FALSE;
               GETTING_CCID =  TRUE;
               while( GETTING_CCID )
               {

                  GETTING_CCID   = FALSE;
                  called_cc_gtid =  TRUE;
                  rc = cc_gtid( rc, conf_ocb, memb_ocb, & cc_id, & flags,
                                                            & list_ocb );
                  if( rc == USER_REQUESTED_QUIT )
                  {

                     rc = NO_ERROR;

                  }
                  else if( rc == NO_ERROR )
                  {

                     if(flags & SCAN_ALL_COMMENT)/* user said to scan  */
                     {                           /* in gt_ccid         */

                        rc = vw_cc( conf_ocb, (string) list_ocb, flags,
                                    memb_ocb );
                        flags &= ~SCAN_ALL_COMMENT;/*Turn bit off so we*/
                                                  /* don't scan again  */
                        if( rc == USER_REQUESTED_QUIT )
                        {

                           rc = NO_ERROR;

                        }
                     }
                     else
                     {

                     ocd_new( &conf_ocd, "CONFERENCE.ITEM_ID.SEQNUM" );
                     s_copy( &(conf_ocd->key), cc_id, 0, 0 );
                     if( obj_get( conf_ocb, conf_ocd, &seqnum ) == NO_ERROR )
                     {

                        s_alloc( &conf_ocd->key,   sizeof(sint) );
                        s_smp_ul( conf_ocd->key) = sizeof(sint)  ;
                        os_memc( &seqnum, s_buffer(conf_ocd->key),
                                                   sizeof(sint) );
                        s_init(&(conf_ocd->name),"CONFERENCE.ITEM_SN.OID");
                        rc = obj_get( conf_ocb, conf_ocd, &cc_oid );

                        cc_ocb = obj_opn( &cc_oid, OBJ_REMOTE|OBJ_UPDATE);
                        if( OCB_POINTER_OK(cc_ocb) )
                        {

                            rc = gt_cc( cc_ocb,conf_ocb,flags,memb_ocb);
                            obj_set_flag( cc_ocb, OBJ_DELETE );
                            obj_cls(      cc_ocb );
                            cc_ocb = NIL;

                            if( rc == USER_REQUESTED_QUIT )
                            {

                               rc = NO_ERROR;

                            }
                        }
                        else
                        {

                           rc = Item_Does_Not_Exist;
                           GETTING_CCID = TRUE;/*Couldn't find the item*/
                                               /*ask user to correct or*/
                        }                      /*for something else    */
                     }
                     else
                     {

                        rc = Item_Does_Not_Exist;
                        GETTING_CCID = TRUE;

                     }
                     ocd_drp( &conf_ocd );

                   } /* if scanning comments */
                  }  /* If no error from cc_gtid() */
               }  /* while we are getting comment id */
               break;

            default :

               done = FALSE;
               break;

         }
         if(
              ( rc == COMMENT_OBJECT_RECEIVED    )
          ||  ( rc == NO_COMMENTS_TO_LIST        )
          ||  ( rc == CONF_DOES_NOT_HAVE_MEMBERS )
           )
         {
                                         /*============================*
                                          Certain errors should not
                                          cause a return from this state.
                                          Note the trouble and represent
                                          the form.
                                          *============================*/

            except( rc, USER_ERROR, S_NIL, S_NIL, S_NIL, S_NIL, S_NIL );
            rc = 0;

         }

      }  /* while */

      if( s_len( form_id ) > 0 )
      {

         f_close(    form_id,   STATE_FORM    );

      }
      if( formed_cr )
      {

         var_clr(    CC_COUNT     );
         var_clr(    TOT_MEMB     );
         var_clr(    TOT_WRITER   );
         s_drop (  & CC_COUNT     );
         s_drop (  & TOT_MEMB     );
         s_drop (  & TOT_WRITER   );

      }
      if( called_cc_gtid )
      {

         s_init (  & NAMES, "NAMES"    );     /* Used in CC_GTID      */
         var_clr(    NAMES             );
         s_init (  & form_id, "1150"   );     /* Opened in CC_GTid()  */
         f_close(    form_id, NO_FLAGS );

      }
      s_drop (  & form_id      );
      s_drop (  & NAMES        );
      s_drop (  & cc_count_str );
      s_drop (  & tot_memb     );
      s_drop (  & tot_writer   );
      s_drop (  & form_id      );
      s_drop (  & cc_id        );

   }                              /* If no errors while accessing conf */

   if( OCB_POINTER_OK( user_ocb ) )
   {

      obj_cls( user_ocb );

   }
   if(      OCB_POINTER_OK( memb_ocb )
    && (    (rc == NO_ERROR               )
      ||    (rc == USER_REQUESTED_QUIT    )
      ||    (rc == USER_REQUESTED_ABANDON ) )  )
   {
                                /* Let MS know all the updates */
      rc = t_m_obj( TM_master->MS, memb_ocb, S_NULL, FALSE, 0);
      obj_set_flag( memb_ocb, OBJ_DELETE );
      obj_cls(      memb_ocb );

   }
   ocd_drp( & memb_ocd );
   ocd_drp( & conf_ocd );

}  /* if there was any conf to get */


   return(rc);

}
