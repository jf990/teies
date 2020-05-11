/************************************************************************
    <DOC>
    <NAME>         lk_act(OCB *attach_ocb, string  packet,hword level,
                                                         string *result)
    <LEVEL>        Activity
    <DATE>         March 1, 1989
    <AUTHOR>       Tanmay S. Kumar
    <PARAM>        attach_ocb OCB *   INPUT  The ocb of the object to
                                             which the activity will be
                                             attached for MASTER SERVER.
                                             The activity ocb for the
                                             LOCAL SERVER.
                   packet     string  INPUT  Not used.
                   level      hword   INPUT  Shows whether the activity
                                             is attached to a mial item
                                             or a conference comment.
                   result     string* IN/OUT Not used.
    <OUTPUT>       Sint.
    <PURPOSE>      At the local server this routine does the sending of
                   the activity object and the sections of the activity.
                   At the master server it does the necessary linking of
                   objects and data.
    </DOC>
************************************************************************/
#include  "defs.h"
#include  "str.h"
#include  "osk.h"
#include  "debug.h"
#include  "obj.h"
#include  "tm.h"
#include  "exc.h"
#include  "isv.h"
#include  "obj_name.h"
#include  "activity.h"


sint lk_act( attach_ocb, packet, level, result )

OCB     *attach_ocb;
string  packet;
hword   level;
string  *result;
{
     OCB       *act_ocb      =       NIL,
               *tbl_ocb      =       NIL,
               *memb_ocb     =       NIL,
               *user_ocb     =       NIL,
               *author_ocb   =       NIL;

     OCD       *act_ocd      =       NIL,
               *tbl_ocd      =       NIL,
               *memb_ocd     =       NIL;

     OID       act_oid                  ,
               tbl_oid                  ,
               memb_oid                 ;

     string    author        =    S_NULL,
               title         =    S_NULL,
               key           =    S_NULL,
               who           =    S_NULL,
               para          =    S_NULL,
               line          =    S_NULL,
               result_str    =    S_NULL,
               response_id   =    S_NULL,
               attach_id     =    S_NULL;

     sint      n_resp        =         0,
               added_res     =         0,
               count         =      9999,
               itm_num       =         0,
               *line_num    =        NIL,
               rc            =         0;

     datetime  dt                       ;

     hword     act_type      =         0,
               level1        =         0;

     boolean   ok_to_link    =     FALSE;



  if ( TM_master->usrtype == MASTER_SERVER )
  {
     if (level == ACT_LINK_MAIL)
     {
        obj_find( attach_ocb, "MESSAGE.ACTIVITY", &act_oid );
        obj_find( attach_ocb, "MESSAGE.ID", &attach_id );
     }
     else if (level == ACT_LINK_CC)
     {
        obj_find( attach_ocb, "COMMENT.ACTIVITY", &act_oid );
#if 0
        obj_find( attach_ocb, "COMMENT.ID", &attach_id );
#endif
        s_copy(&attach_id, packet, 0, 0);
     }
     else if (level == ACT_LINK_RESPONSE)
     {
        obj_find( attach_ocb, "COMMENT.ACTIVITY", &act_oid );
     }
     else if (level == ACT_LINK_SELECTION)
     {
        os_memc(&(attach_ocb->oid), &act_oid, sizeof(OID) );
     }
     act_ocb = obj_opn( &act_oid, OBJ_UPDATE );

     if ((OCB_POINTER_OK(act_ocb))  && (level != ACT_LINK_RESPONSE) &&
                                       (level != ACT_LINK_SELECTION)  )
     {
        /* for conference comment i'm am putting in Cxxx.xCCxxx.x */

        obj_upd( act_ocb, "ACTIVITY.ID", attach_id  );
        memb_ocb = obj_all( MEMB_OBJ, LOCAL_OBJECT, NIL );
        obj_upd( memb_ocb, "%LINKS.OWNER", &(act_ocb->oid) );
        obj_upd( act_ocb, "ACTIVITY.MEMBERS", &(memb_ocb->oid) );
        obj_cls(act_ocb);
        obj_cls(memb_ocb);
     }
     else if (level == ACT_LINK_RESPONSE)
     {
        /* the attach ocb in this case is the response ocb */

        obj_find( act_ocb, "ACTIVITY.PART_COUNT", &n_resp );
        if (n_resp < 0 )
        {
           n_resp = 0;
        }
        n_resp++;
        obj_upd( act_ocb, "ACTIVITY.PART_COUNT", &n_resp );
        s_sitos(&response_id, n_resp);
        obj_upd( attach_ocb, "COMMENT.ID", response_id);
        obj_find( attach_ocb, "COMMENT.TITLE", &title );

        ocd_new( &act_ocd, "ACTIVITY.PART_LIST.TITLE" );
        s_init( &(act_ocd->key), "    " );
        s_alloc(&(act_ocd->key), 4);
        s_smp_ul(act_ocd->key) = 4;
        os_memc( &n_resp, s_buffer( act_ocd->key ), 4 );
        obj_rep( act_ocb, act_ocd, title, SEG_UPDATE );

        s_init( &(act_ocd->name), "ACTIVITY.PART_LIST.LOCATION" );
        obj_rep( act_ocb, act_ocd, &(attach_ocb->oid), SEG_UPDATE );
     }
     else if (level == ACT_LINK_SELECTION)
     {
        /* need to make the selection if possible */
        ocd_new(&act_ocd, "ACTIVITY.SEL_WINDOW" );
        s_stosi(&itm_num, packet, 0, 0);
        rc = a_addsel(act_ocb, packet, level, itm_num, act_ocd);
     }
     else
     {
        rc = INVALID_OCB;
     }
     if ( (rc == 0) && (
          (level == ACT_LINK_RESPONSE) || (level == ACT_LINK_SELECTION)))
     {
        s_alloc(&key, sizeof(OID) );
        s_smp_ul(key) = sizeof(OID);
        os_memc(&(act_ocb->oid), s_buffer(key), sizeof(OID));
        /* receiveing the activity */
        rc_act(act_ocb, key, level, (byte *) (*result));
        obj_set_flag(act_ocb, OBJ_RAW);
        obj_find( act_ocb, "ACTIVITY.ISV_TYPE", &act_type );
        obj_reset_flag(act_ocb, OBJ_RAW);
        rc =  a_crntf(act_ocb, itm_num, act_type, attach_ocb->type);
        obj_find(act_ocb, "ACTIVITY.MEMBERS", &memb_oid);
        memb_ocb = obj_opn(&memb_oid, OBJ_UPDATE);

        if (OCB_POINTER_OK(memb_ocb) )
        {
           user_ocb = t_author();
           ocd_new(&memb_ocd,"MEMBERSHIP.MEMBERS.NOTIF_STATUS");
           oid_key( memb_ocd, &(user_ocb)->oid );
           rc = obj_get(memb_ocb, memb_ocd, &added_res );

           if (rc != 0)  /*should check for 20023*/
           {
              added_res = 1;
              obj_rep(memb_ocb, memb_ocd, &added_res, SEG_UPDATE );
              level1 = ACT_LINK_READ;
              rc = rc_act(act_ocb, packet, level1, (byte *)*result);
           }
           else
           {
              added_res += 1;
              obj_rep(memb_ocb, memb_ocd, &added_res, SEG_UPDATE );
              os_dttm(&dt);
              s_init(&(memb_ocd->name),"MEMBERSHIP.MEMBERS.LAST_ACTIVE");
              obj_rep(memb_ocb, memb_ocd, &dt, SEG_UPDATE );
           }
           if (level == ACT_LINK_SELECTION)
           {

              s_init( &(act_ocd->name), "ACTIVITY.PART_LIST.LOCATION" );
              s_alloc(&(act_ocd->key), 4);
              s_smp_ul(act_ocd->key) = 4;
              os_memc( &count, s_buffer( act_ocd->key ), 4 );
              obj_get( act_ocb, act_ocd, &tbl_oid );
              tbl_ocb = obj_opn(&tbl_oid, OBJ_UPDATE);
              if ( !OCB_POINTER_OK (tbl_ocb) )
              {
                 rc = INVALID_OCB;
              }
              else
              {
                 ocd_new( &tbl_ocd, "COMMENT.CONTENTS.TEXT" );
                 s_alloc(&(tbl_ocd->key),4);
                 s_smp_ul(tbl_ocd->key) = 4;
                 line_num = (sint *)s_buffer(tbl_ocd->key);
                 s_stosi(&count, packet, 0, 0);
                 *(line_num) = count;
                 obj_get(tbl_ocb, tbl_ocd, &line);
                 s_copy(&line, line, 1, 32);
                 gt_name(user_ocb,S_NULL,NICKNAME_ONLY,&who);
                 s_cat(&line, line, who, 0, 0);
                 s_init(&para,"<p>");
                 s_cat(&line, line, para, 0, 0);
                 obj_rep(tbl_ocb, tbl_ocd,line,SEG_UPDATE);
                 obj_cls(tbl_ocb);
                 ocd_drp(&tbl_ocd);
                 /* update membership window to show which selection */
                 /* user has made                                    */
                 s_init(&(memb_ocd->name), "MEMBERSHIP.MEMBERS.WINDOW");
                 rc = a_addsel(memb_ocb,packet,level,itm_num,memb_ocd);
              }
           }

           obj_cls(memb_ocb);
           obj_cls(user_ocb);
           ocd_drp(&memb_ocd);
        }
        rc = t_m_obj(t_invoker(), act_ocb,
                  (void **)(&result_str), ok_to_link, (sint)level);
        if ( (rc == 0)  && (level == ACT_LINK_RESPONSE) )
        {
            rc = t_m_obj(t_invoker(), attach_ocb,
                  (void **)(&result_str), ok_to_link, (sint)level);
            if (rc == 0)
            {
                t_csend( t_invoker(), rc, (string)result );
            }
        }
        ocd_drp(&act_ocd);
        obj_cls(act_ocb);
     }
     if ( (rc != 0) && (level == ACT_LINK_SELECTION) )
     {
        t_csend( t_invoker(), rc, (string)result );
     }
     s_drop( &attach_id);
  }                          /* end of if-else for Master_Server */
  else
  {
     sint       part_count =      0,
                i          =      0;
     OCB        *part_ocb  =    NIL;
     OID        part_oid;
                                       /* send the activity object */

     /*            Link Parts of the Activity                   */

     if (level == ACT_LINK_SELECTION)
     {
        rc = t_remote(TM_master->MS, ACT_OBJ, LINK_ACT, level,
                            attach_ocb, packet , (void **)(&result_str));

     }
     else if (level == ACT_LINK_RESPONSE)
     {
        rc = t_m_obj(TM_master->MS, attach_ocb,
                  (void **)(&result_str), ok_to_link, (sint)level);
        rc = t_remote(TM_master->MS, ACT_OBJ, LINK_ACT, level,
                            attach_ocb, packet , (void **)(&result_str));
        *result = (string)attach_ocb;
        rc = t_waitc( TM_master->MS, NIL );
        rc = t_waitc( TM_master->MS, NIL );
     }
     else
     {
        ocd_new( &act_ocd, "ACTIVITY.PART_COUNT" );
        obj_get( attach_ocb, act_ocd, &part_count );/*get no. of parts*/

        s_init( &act_ocd->name, "ACTIVITY.PART_LIST.LOCATION" );
        act_ocd->bias = 1;
        while ( i <= part_count )
        {
           rc = obj_get( attach_ocb, act_ocd, &part_oid );
           if (rc == 0)
           {
              /* need to put in check for if OCB_POINTER_OK  */
              part_ocb = obj_opn( &part_oid, OBJ_UPDATE );
              if (OCB_POINTER_OK(part_ocb) )
              {
                 rc = t_m_obj(TM_master->MS, part_ocb,
                      (void **)(&result_str), ok_to_link, (sint)level );
                 obj_cls(part_ocb);
              }
           }
           else
           {
           }
           ++i;
           rc = 0;
        }
        ocd_drp( &act_ocd );
     }
     if ( (rc == 0)  &&  (level != ACT_LINK_RESPONSE) &&
                          (level != ACT_LINK_SELECTION) )
     {
         /* now send the activity object */
         rc = t_m_obj( TM_master->MS, attach_ocb, (void **)(&result_str),
                                               ok_to_link, (sint)level );
     }
  }                      /* end of if-else for Local_User_Server */
   return(rc);
}
