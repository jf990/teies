/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    lk_cc                                                  |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   cc_ocb      OCB *      INPUT  OCB of the comment to    |
  |                                           link.                   |
  |  <PARAM>   modifier    string     INPUT  * not used *             |
  |  <PARAM>   level       hword      INPUT  Special linking consid-  |
  |                                           erations                |
  |  <PARAM>   cr_ocb      OCB *      INPUT  Conference Root OCB to   |
  |                                           link the comment to.    |
  |                                                                   |
  |  <PURPOSE> Link the conference comment to the conference root.    |
  |                                                                   |
  |  <OUTPUT>  sint                  result code.                     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "isv.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"
#include "exc.h"


sint     lk_cc( cc_ocb, modifier, level, params )

OCB            *cc_ocb;
string          modifier;
hword           level;
byte           *params;


{


byte     **ptr    = &params;
sint       rc     = 0;



if( TM_master->usrtype == USER_SERVER )
{

   OCD   * ocd             =   NIL;
   OCB   * contents        =   NIL;
   OID     contents_oid           ;
   boolean ok_to_link      = FALSE;
   sint    state           =     0;
   string  result_str      = S_NIL;



   ocd_new (   & ocd, "COMMENT.CONTENTS" );
   rc = obj_get( cc_ocb, ocd,       & contents_oid );
   if( rc == 0  && ( ! OID_IS_NULL( & contents_oid ) )  )
   {

      contents = obj_opn( & contents_oid, OBJ_READ );
      if( OCB_POINTER_OK( contents ) )
      {

         ok_to_link = FALSE;
         state      =     0;
         rc = t_m_obj( TM_master->MS, contents, & result_str,
                                                     ok_to_link, state );
         if( rc == 0 )
         {

            obj_set_mode( contents, OBJ_REMOTE );
            obj_cls(      contents );

         }
      }
   }
   rc = t_spawn( cc_ocb, modifier, (sint) level, ptr );
   if( rc == NO_ERR )
   {

      rc =t_waitc(TM_master->MS, ptr);      /* Now wait for updated CR*/

      if( level & DOING_REPLY )             /* Wait for reply OCB if  */
      {                                     /* we're actually doing a */
                                            /* reply                  */
         rc =t_waitc(TM_master->MS, ptr);

      }
   }
   ocd_drp( & ocd );
   s_drop(  & result_str );

}
else
{

   OCB   * reply_ocb        = NIL,
         * author_ocb       = NIL,
         * memb_ocb         = NIL,
         * user_ocb         = NIL,
         * act_ocb          = NIL,
         * cr_ocb           = NIL;
   OCD   * memb_ocd         = NIL,
         * act_ocd          = NIL,
         * undone_act_ocd   = NIL,
         * new_cc_count_ocd = NIL,
         * tot_cc_count_ocd = NIL,
         * component        = NIL;
   OID     tmp_oid               ,
         * user_oid         = NIL,
           act_oid               ;
   sint    count            =   0,
           status           =   0,
           cc_seqnum        =   0,
           current_id       =   0;
   string  reply_id         = S_NULL,
           dot              = S_NULL,
           dt               = S_NULL,
           name_str         = S_NULL,
           work_str         = S_NULL,
           key_words        = S_NULL,
           line             = S_NULL,
           tmpstr           = S_NULL,
           cr_id            = S_NULL,
           cc_title         = S_NULL,
           id               = S_NULL,
           act_id           = S_NULL,
           cc_id_prefix     = S_NULL,
           cc_id_suffix     = S_NULL,
           cc_id            = S_NULL;
   datetime current_dt              ;
   boolean  done            =  FALSE,
            replying        =  FALSE,
            activity_attached= FALSE;
   byte     rep_lvl          =     0;


   ocd_new(   & component,   "%LINKS.ROOT"     );
   obj_get(     cc_ocb, component, & tmp_oid   );
   cr_ocb = obj_opn(  & tmp_oid, OBJ_UPDATE    );

   ocd_new(   & component,   "CONFERENCE.MEMBERS" );
   obj_get(     cr_ocb, component, & tmp_oid      );
   memb_ocb = obj_opn( & tmp_oid, OBJ_UPDATE      );

   ocd_new(   & component,   "%LINKS.AUTHOR"      );
   obj_get(     cc_ocb,     component, & tmp_oid  );
   author_ocb = obj_opn(  & tmp_oid, OBJ_UPDATE   );

   s_init( & (component->name), "COMMENT.REPLY_ID" );
   obj_get( cc_ocb, component,  & reply_id         );
   if( s_len( reply_id )   )
   {

      replying = TRUE;
      s_init(  & ( component->name ), "%LINKS.OWNER"  );
      obj_get(     cc_ocb, component, & tmp_oid       );
      reply_ocb =  obj_opn( & tmp_oid, OBJ_UPDATE     );

   }
   obj_find( cc_ocb, "COMMENT.KEYS", & key_words );
   rc     =      t_ckkeys( & work_str, key_words );
   if( rc == 0 )
   {

      obj_update( cc_ocb, "COMMENT.KEYS", work_str );

   }
   s_drop( & work_str  );
   s_drop( & key_words );

   s_init(   & ( component->name ), "COMMENT.ACTIVITY" );
   rc = obj_get( cc_ocb, component, & act_oid );
   if( ( rc == NO_ERROR ) && ( act_oid.one != 0 ) )
   {

      activity_attached = TRUE;

   }

              /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
              /* Okay, now we'll first take care of linking comment to */
              /* its owner(if we're replying to an item).              */
              /* Create new item id, etc...                            */
              /* Then take care of activity, if any.                   */
              /* Then link comment with conference                     */
              /* And finally, let all members know of the item         */
              /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if( replying )
   {

      s_init(  & (component->name),      "%LINKS.LINKS.OBJECT_TYPE"  );
      oid_key(    component,            & (cc_ocb->oid)              );
      obj_rep(    reply_ocb, component, & (cc_ocb->type), SEG_UPDATE );
      s_drop(  & (component->key)                                    );

      s_copy(  & cc_id, reply_id, 0, 0 );

      s_init(  & (component->name), "COMMENT.REPLY_COUNT" );
      obj_get(    reply_ocb, component, & count  );
      count ++ ;
      obj_rep(    reply_ocb, component, & count, SEG_UPDATE );

      s_init(  & dot,                         "." );
      s_cat(   & cc_id, cc_id, dot,          0, 0 );
      s_sitos( & cc_id_suffix, count              );
      s_cat(   & cc_id, cc_id, cc_id_suffix, 0, 0 );
      s_drop(  & cc_id_suffix                     );
      s_drop(  & dot                              );

      s_init(  & (component->name), "COMMENT.REPLY_LEVEL" );
      obj_get(    reply_ocb, component,        & rep_lvl  );
      rep_lvl ++ ;
      obj_rep( cc_ocb, component, & rep_lvl,   SEG_UPDATE );

   }
   else
   {

      count = 0;
      s_init( & (component->name), "CONFERENCE.STATUS.ROOT_COUNT" );
      obj_get(   cr_ocb, component, & count );
      count ++ ;
      obj_rep(   cr_ocb, component, & count, SEG_UPDATE );

      s_init( & (component->name), "CONFERENCE.STATUS.CURRENT_ROOT_ID" );
      obj_get(   cr_ocb, component, & current_id );
      current_id ++ ;
      obj_rep(   cr_ocb, component, & current_id, SEG_UPDATE );

      s_sitos( & cc_id, current_id );

   }
   s_init(  & (component->name), "CONFERENCE.STATUS.ITEM_COUNT" );
   obj_get(    cr_ocb, component, & count );
   count ++ ;
   obj_rep(    cr_ocb, component, & count, SEG_UPDATE );

   s_init(  & (component->name),        "COMMENT.ID" );
   obj_rep(    cc_ocb, component, cc_id, SEG_UPDATE  );

   os_dttm( &  current_dt );

   s_init(  & (component->name),         "COMMENT.ENTERED" );
   obj_rep(    cc_ocb, component, & current_dt, SEG_UPDATE );

   s_init(  & (component->name), "CONFERENCE.STATUS.LAST_ITEM_SEQNUM" );
   obj_get(    cr_ocb, component, & cc_seqnum );
   cc_seqnum ++ ;
   obj_rep( cr_ocb, component,    & cc_seqnum, SEG_UPDATE );

   s_init(  & (component->name),         "COMMENT.SEQNUM" );
   obj_rep(    cc_ocb, component, & cc_seqnum, SEG_UPDATE );

   s_init(  & (component->name), "CONFERENCE.STATUS.LAST_ITEM_ID"  );
   obj_rep(    cr_ocb, component,   cc_id, SEG_UPDATE              );

   s_init(  & (component->name), "CONFERENCE.STATUS.LAST_ITEM_OID" );
   obj_rep(    cr_ocb, component, & (cc_ocb->oid), SEG_UPDATE      );

   s_init(  & (component->name), "CONFERENCE.STATUS.LAST_ITEM_DT"  );
   obj_rep(    cr_ocb, component, & current_dt, SEG_UPDATE         );

   s_init(  & (component->name), "CONFERENCE.ITEM_ID.SEQNUM" );
   component->key = cc_id;
   obj_rep(   cr_ocb, component,  & cc_seqnum,    SEG_UPDATE );
   component->key = S_NULL;

   oid_key(    component,               (OID *) & current_dt );
   s_init(  & (component->name), "CONFERENCE.ITEM_DT.SEQNUM" );
   obj_rep(    cr_ocb, component,    & cc_seqnum, SEG_UPDATE );

   os_memc( & cc_seqnum, s_buffer( component->key ), sizeof( sint ) );
   s_smp_ul(component->key) =                        sizeof( sint )  ;

   s_init(  & (component->name),        "CONFERENCE.ITEM_SN.OID" );
   obj_rep(    cr_ocb, component, & (cc_ocb->oid),    SEG_UPDATE );

   s_init(  & (component->name), "CONFERENCE.ITEM_SN.TIME_STAMP" );
   obj_rep(    cr_ocb, component, & current_dt,       SEG_UPDATE );

   s_init(  & (component->name), "CONFERENCE.ITEM_SN.ID"         );
   obj_rep(    cr_ocb, component, cc_id,              SEG_UPDATE );

   s_init(  & (component->name), "COMMENT.TITLE"            );
   obj_get(   cc_ocb, component, & cc_title                 );
   s_init(  & (component->name), "CONFERENCE.ITEM_SN.TITLE" );
   obj_rep(   cr_ocb, component, cc_title,       SEG_UPDATE );
   s_drop(  & cc_title );

   s_init(  & (component->name), "CONFERENCE.ID" );
   obj_get( cr_ocb, component, & cr_id );

   ocd_new( & tot_cc_count_ocd, "INDEXES.CC_COUNT"                     );
   ocd_new( & new_cc_count_ocd, "MEMBERSHIPS.CONFERENCES.NEW_CC_COUNT" );
   ocd_new( & memb_ocd,         "MEMBERSHIP.MEMBERS.STATUS"            );
   new_cc_count_ocd->key = cr_id;
   memb_ocd->bias        =     1;

   if( activity_attached )
   {

      count = 0;
      ocd_new( & act_ocd, "CONFERENCE.STATUS.ACT_COUNT" );
      obj_get( cr_ocb, act_ocd, & count );
      count++;
      obj_rep( cr_ocb, act_ocd, &count, SEG_UPDATE );
                                       /* Activity needs CCs id        */
      isv_get( & tmpstr, ISV_CONF,     1 );
      s_cat(   & id, tmpstr, cr_id, 0, 0 );
      isv_get( & tmpstr, ISV_CC,       2 );
      s_cat(   & id, id, tmpstr,    0, 0 );
      s_cat(   & id, id, cc_id,     0, 0 );

      rc = lk_act( cc_ocb, id, ACT_LINK_CC, NIL );

      s_drop(  & id );

      s_init(  &(act_ocd->name), "INDEXES.UNDONE_COUNT"   );
      ocd_new( & undone_act_ocd, "INDEXES.UNDONE_ACT.OID" );
      oid_key(   undone_act_ocd, & (act_oid)              );

   }
   done = FALSE;
   while( NOT done )
   {

      if( ( rc = obj_get( memb_ocb, memb_ocd, & status ) ) == NO_ERROR )
      {

         user_oid = (OID *) s_buffer( memb_ocd->key );
         if( ! oid_cmp( (*user_oid), author_ocb->oid ) )
         {

            user_ocb = obj_opn( user_oid, OBJ_UPDATE );
            if( OCB_POINTER_OK( user_ocb ) )
            {

               count = 0;
               obj_get( user_ocb, tot_cc_count_ocd, & count );
               count++;
               obj_rep( user_ocb, tot_cc_count_ocd, &count, SEG_UPDATE );

               count = 0;
               obj_get( user_ocb, new_cc_count_ocd, & count );
               count++;
               obj_rep( user_ocb, new_cc_count_ocd, &count, SEG_UPDATE );

               if( activity_attached )
               {

                  count = 0;
                  obj_get( user_ocb, act_ocd, & count );
                  count++;
                  obj_rep( user_ocb, act_ocd, & count, SEG_UPDATE );
                  obj_rep( user_ocb, undone_act_ocd,&act_oid,SEG_UPDATE);

               }
               obj_cls( user_ocb );

            }
            else
            {

               done = TRUE;

            }
         }
      }
      else
      {

         if( rc == NO_DATA_FOUND )
         {

            rc   =    0;

         }
         done = TRUE;

      }

   }
   if( rc == NO_ERROR )
   {

      t_increment_items_created( NIL );
      rc = t_m_obj( t_invoker(), cc_ocb, S_NULL, FALSE, 0 );
      rc = t_m_obj( t_invoker(), cr_ocb, S_NULL, FALSE, 0 );

      if( replying && OCB_POINTER_OK(reply_ocb) )
      {

         rc = t_m_obj( t_invoker(), reply_ocb, S_NULL, FALSE, 0 );

      }
      rc = DO_NOT_CONFIRM_TRANSACTION;

   }
   new_cc_count_ocd->key = S_NULL;
   ocd_drp( & new_cc_count_ocd );
   ocd_drp( & tot_cc_count_ocd );
   ocd_drp( & memb_ocd );
   ocd_drp( & act_ocd  );
   ocd_drp( & component  );

   obj_cls( cr_ocb       );
   obj_cls( memb_ocb     );
   obj_cls( author_ocb   );

   if(  OCB_POINTER_OK( act_ocb ) )
   {

      obj_cls(  act_ocb  );

   }
   if( OCB_POINTER_OK( reply_ocb ) )
   {

      obj_cls( reply_ocb );

   }
   s_drop( & cc_id );
   s_drop( & cr_id );

}


   return(rc);

}
