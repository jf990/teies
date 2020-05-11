/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    jn_cr                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   some_object    OCB *   INPUT  Object to locate a CR.   |
  |  <PARAM>   provided_names string  INPUT  CR Label or ID.          |
  |  <PARAM>   level          hword   INPUT                           |
  |  <PARAM>   membership     OCB *   INOUT                           |
  |  <PURPOSE> Join Conference Root.                                  |
  |            If no conference object is provided then check the     |
  |            modifier string, and if that too is empty prompt the   |
  |            user for the conference(s) to join.                    |
  |                                                                   |
  |            If an object is provided and it is not a conference    |
  |            root, attempt to derive a conference root from it.     |
  |                                                                   |
  |            If a membership object is not provided as the 4th arg  |
  |            then open it from the conference root reference.       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"
#include "obj_cr.h"
#include "obj_memb.h"
#include "obj_list.h"
#include "obj_user.h"



sint jn_cr( some_ocb, provided_conference_names, level, membership )

OCB       * some_ocb,                                 * membership;
string                provided_conference_names;
hword                                            level;



{

byte   * pkt_ptr             = NIL;
hword    command             = SEND_OBJECT,
         flags               = 0;
sint     rc                  = NO_ERROR,
         conf_c              = 0,
         count               = 0,
         seq_num             = 0,
       * seqnum              = & seq_num,
         cccount             = 0,
       * cc_count            = & cccount,
         junk                = 0,
         number_bad_conf_ids = 0,
         key_hit             = 0;
string   state               =  S_NULL,
         packet              =  S_NULL,
         conf_lab            =  S_NULL,
         conf_id             =  S_NULL,
         ITEM_NUM            =  S_NULL,
         TOT_MEMB            =  S_NULL,
         TOT_WRITER          =  S_NULL,
         NAMES               =  S_NULL,
         NAME2               =  S_NULL,
         tot_memb            =  S_NULL,
         tot_writer          =  S_NULL,
         item_num            =  S_NULL,
         tmp_key             =  S_NULL,
         new_role            =  S_NULL,
         pkt_str             =  S_NULL,
         conf_roots          =  S_NULL,
         senddata            =  S_NULL,
       * result_str          =  S_NULL,
         bad_conference_ids  =  S_NULL,
         invoker             =  S_NULL;

OCB    * user_ocb            =     NIL,
       * conf_ocb            =     NIL,
       * list_ocb            =     NIL;
OCD    * conf_ocd            =     NIL,
       * memb_ocd            =     NIL,
       * user_ocd            =     NIL;
OID      cc_oid                       ,
         conf_oid                     ;
boolean  NOT_DONE            =    TRUE,
         done_with_form      =   FALSE,
         presented_form      =   FALSE,
         membership_opened   =   FALSE,
         OK_TO_VIEW_CONFERENCE = FALSE,
         UPDATE_MEMB_INFO    =   FALSE;
datetime tmp_time                     ,
       * time_stamp          = & tmp_time,
         cur_dt                       ;







if( TM_master->usrtype == USER_SERVER )
{

   if( ! OCB_POINTER_OK( some_ocb ) )
   {

      s_init(         & state, "8040"      );
      s_init(         & NAMES, "NAMES"     );
      s_init(         & NAME2, "NAME2"     );

                                       /* = = = = = = = = = = = = = = */
                                       /* If we are not provided a    */
                                       /* valid conference object then*/
                                       /* attempt to get one by refer-*/
                                       /* ence OR ask the user which  */
                                       /* conference they want to join*/
                                       /* = = = = = = = = = = = = = = */

      while( NOT done_with_form )
      {

         if( s_len( provided_conference_names ) > 1 )
         {

            /* User provided name/id to Join, so convert that for validation */
            s_copy( & conf_roots, provided_conference_names, 0, 0 );
            presented_form = FALSE;
            done_with_form =  TRUE;

         }
         else
         {

            key_hit        = t_form( state, S_NULL, NIL );
            presented_form = TRUE;

            switch( key_hit )
            {

               case   USER_REQUESTED_QUIT    :
               case   USER_REQUESTED_ABANDON :
               case   FORM_NOT_FORMED        :

                  rc             = key_hit;
                  done_with_form =    TRUE;
                  var_clr(    NAMES      );
                  var_clr(    NAME2      );

                  continue;
                  break;

               case   USER_REQUESTED_ENTER   :
               case   USER_REQUESTED_PROCESS :

                  s_nulout( & conf_roots );
                  s_cat(    & conf_roots, conf_roots,
                                                var_get( NAMES ), 0, 0 );
                  s_cat(    & conf_roots, conf_roots,
                                                var_get( NAME2 ), 0, 0 );

                  s_nulout( & bad_conference_ids );
                  rc = obj_item( conf_roots, OBJ_NEED_MANY_OID, NIL,
                               & list_ocb, & bad_conference_ids,
                               & number_bad_conf_ids, CONF_OBJ );
                  break;

               case   USER_REQUESTED_PRINT   :

                  user_ocb = t_author();
                  rc = obj_find( user_ocb, "MEMBERSHIPS.CONFERENCES",
                                                        & conf_oid );
                  if(   rc == NO_ERROR  &&
                    ( ! OID_IS_NULL( (& conf_oid) ) ) )
                  {

                     list_ocb = obj_opn( & conf_oid,
                                                 OBJ_READ | OBJ_REMOTE );

                  }
                  else
                  {

                     rc = CONF_NOT_MEMBER_OF_ANY;

                  }
                  obj_cls( user_ocb );
                  break;

               case   USER_REQUESTED_SCAN    :

                  break;

               default                       :

                  break;

            }
            var_clr(    NAMES    );
            var_clr(    NAME2    );

         }

         if( number_bad_conf_ids > 0 )
         {

            rc = CONF_DOES_NOT_EXIST;
            except( rc, USER_ERROR, bad_conference_ids, S_NIL, S_NIL,
                                                        S_NIL, S_NIL  );
            f_curpos( state, STATE_FORM, NAMES, 0.0, 0.0 );

         }
         else
         {

            rc = vw_list( list_ocb, S_NULL, level, (byte *) membership );

            if( rc == USER_REQUESTED_ABANDON || System_Level_Error( rc ))
            {

               done_with_form = TRUE;

            }
         }
         if(  OCB_POINTER_OK( list_ocb )      &&
           (  key_hit != USER_REQUESTED_PRINT ) )
         {

            cr_list( list_ocb, S_NULL, 0, & list_ocb );

         }
         f_rstfld( state, STATE_FORM, NAMES );
         f_rstfld( state, STATE_FORM, NAME2 );

      }
      s_drop(  & bad_conference_ids );
      s_drop(  & conf_roots );
      s_drop(  & NAMES      );
      s_drop(  & NAME2      );
      if( presented_form )
      {

         f_close(   state, STATE_FORM  );

      }
      s_drop(  & state      );
      if( OCB_POINTER_OK( list_ocb ) )
      {

         obj_set_flag( list_ocb, OBJ_DELETE );
         obj_cls(      list_ocb             );

      }
   }
   else
   {

      switch( some_ocb->type )
      {

         case CONFERENCE_OBJ   :

            conf_ocb = some_ocb;
            break;

         case NOTIFICATION_OBJ :

            rc = jn_cr( conf_ocb, S_NULL, level, (byte *) membership );
            conf_ocb = NIL;
            break;

         case COMMENT_OBJ      :

            rc = jn_cr( conf_ocb, S_NULL, level, (byte *) membership );
            conf_ocb = NIL;
            break;

         case MEMBERSHIP_OBJ   :

            rc = jn_cr( conf_ocb, S_NULL, level, (byte *) membership );
            conf_ocb = NIL;
            break;

         case ACTIVITY_OBJ     :

            rc = jn_cr( conf_ocb, S_NULL, level, (byte *) membership );
            conf_ocb = NIL;
            break;

         default               :

            conf_ocb = NIL;
            break;


      }
   }


if ( OCB_POINTER_OK( conf_ocb )  &&  ( rc == NO_ERROR ) )
{
                                      /*+---------------------------+
                                        |Only do the Join screen if |
                                        |the user is NOT a member of|
                                        |the conference OR the user |
                                        |is altering their marker.  |
                                        +---------------------------+*/
   if( TRUE )
   {

   if( ! OCB_POINTER_OK( membership ) )
   {

      OID    membership_oid;


      rc = obj_find( conf_ocb, "CONFERENCE.MEMBERS", & membership_oid );
      if( rc == 0 )
      {

         membership = obj_opn( & membership_oid, OBJ_READ | OBJ_REMOTE );
         membership_opened = TRUE;

      }
   }
   s_init ( & state,      "8140"                 );
   s_init ( & ITEM_NUM,   "ITEM_NUM"             );
   s_init ( & TOT_MEMB,   "TOT_MEMB"             );
   ocd_new( & memb_ocd,   "MEMBERSHIP.COUNT"     );
   obj_get(   membership, memb_ocd, & count      );
   s_sitos( & tot_memb, count                    );
   var_set(   TOT_MEMB, tot_memb                 );

   count = 0;
   s_init ( &  TOT_WRITER,      "TOT_WRITER"        );
   s_init ( & (memb_ocd->name), "MEMBERSHIP.WRITER" );
   obj_get(    membership,      memb_ocd, & count   );
   s_sitos ( & tot_writer,      count               );
   var_set(    TOT_WRITER,      tot_writer          );

   os_memn( time_stamp, sizeof( datetime ) );      /* Clear out marker */

   while( NOT_DONE && (rc == NO_ERROR) )
   {

      key_hit = t_form( state, S_NULL, conf_ocb );

      switch( key_hit )
      {

         case FORM_NOT_FORMED        :
         case USER_REQUESTED_ABANDON :
         case USER_REQUESTED_QUIT    :

            NOT_DONE              =   FALSE;
            rc                    = key_hit;
            UPDATE_MEMB_INFO      =   FALSE;
            OK_TO_VIEW_CONFERENCE =   FALSE;
            break;

         case USER_REQUESTED_CONFIRM :

            NOT_DONE = TRUE;
            rc       = vw_memb( membership, S_NULL, level, conf_ocb );

            if( rc == USER_REQUESTED_QUIT )
            {

               rc = NO_ERROR;

            }
            UPDATE_MEMB_INFO      =   FALSE;
            OK_TO_VIEW_CONFERENCE =   FALSE;
            break;

         case USER_REQUESTED_ENTER:
         case USER_REQUESTED_PROCESS:

            /* get date&time or comment to start from, from input */


            ocd_new( & conf_ocd,     "CONFERENCE.ITEM_ID.SEQNUM" );
            s_copy(  & conf_ocd->key, var_get( ITEM_NUM ),  0, 0 );


            if( (rc=obj_get( conf_ocb, conf_ocd, seqnum))==NO_DATA_FOUND)
            {
                                       /*if we didn't find what we were*/
               conf_ocd->bias = 1;     /*looking for, lets look for the*/
                                       /*ccid after that one           */
               rc = obj_get( conf_ocb, conf_ocd, seqnum );
               conf_ocd->bias = 0;

            }
            if( rc == NO_ERROR )
            {

               s_alloc( & conf_ocd->key,    sizeof(sint) );
               s_smp_ul(  conf_ocd->key ) = sizeof(sint)  ;
               os_memc( seqnum, s_buffer(conf_ocd->key), sizeof(sint) );

               s_init(&(conf_ocd->name),"CONFERENCE.ITEM_SN.TIME_STAMP" );
               obj_get( conf_ocb, conf_ocd, time_stamp );
               if( time_stamp->fract == 0 )/* Just incase fract is 0   */
               {                           /* if so, then subtract secs*/
                                           /* i just hope secs never   */
                  time_stamp->second--;    /* get to be 0 also!!       */

               }
               else
               {

                  time_stamp->fract--;

               }
               UPDATE_MEMB_INFO = TRUE;

            }
            else if( rc == NO_DATA_FOUND )
            {

               time_stamp->century =        1;
               UPDATE_MEMB_INFO    =     TRUE;
               rc                  = NO_ERROR;

            }
            OK_TO_VIEW_CONFERENCE =  TRUE;
            NOT_DONE              = FALSE;
            break;

         case USER_REQUESTED_PRINT:

            ocd_new( & conf_ocd, "CONFERENCE.STATUS.LAST_ITEM_DT" );
            obj_get(   conf_ocb, conf_ocd, time_stamp );

            if( time_stamp->century == 0 )/*if there are no CCs in conf*/
            {

               time_stamp->century = 1; /* So we don't call JN_CR again*/

            }
            else                       /* And if there are no CCs in   */
            {                          /* this conf, then why bother   */
                                       /* looking for seqnum           */
               ocd_new(&conf_ocd,"CONFERENCE.STATUS.LAST_ITEM_SEQNUM" );
               obj_get( conf_ocb, conf_ocd, seqnum );

            }
            UPDATE_MEMB_INFO      =  TRUE;
            OK_TO_VIEW_CONFERENCE =  TRUE;
            NOT_DONE              = FALSE;
            break;

         case USER_REQUESTED_REPLY:

            time_stamp->century = 1;    /* So we don't call JN_CR again*/

                                        /* if marker is at begining    */
                                        /* hhen the user has not read  */
                                        /* any items yet...count will  */
                                        /* be # of items in conf       */
            ocd_new( &conf_ocd, "CONFERENCE.STATUS.ITEM_COUNT" );
            obj_get( conf_ocb, conf_ocd, cc_count );
            UPDATE_MEMB_INFO      =  TRUE;
            OK_TO_VIEW_CONFERENCE =  TRUE;
            NOT_DONE              = FALSE;
            break;

         default:

            break;

      }


      if( UPDATE_MEMB_INFO == TRUE )
      {
                                       /**- - - - - - - - - - - - - - **/
                                       /* Update info & counters on MS */
                                       /* And MS will send ocb back    */
                                       /* with all the updates         */
                                       /* Allocate space for pkt we're */
                                       /* sending to MS                */
                                       /**- - - - - - - - - - - - - - **/

         rc = cfr_update_member_marker( & ( TM_master->ucb->user_id ),
                                        & ( membership->oid         ),
                                        time_stamp, * seqnum, * cc_count,
                                        conf_ocb                    );

      }
      ocd_drp( & conf_ocd );
      ocd_drp( & memb_ocd );

   }                                                          /* while */

   f_close(  state, NO_FLAGS );

   s_drop ( & ITEM_NUM   );
   s_drop ( & TOT_MEMB   );
   s_drop ( & TOT_WRITER );
   s_drop ( & tot_memb   );
   s_drop ( & tot_writer );
   s_drop ( & state      );

   }
   else
   {

      OK_TO_VIEW_CONFERENCE = TRUE;

   }
   if( OK_TO_VIEW_CONFERENCE )
   {

      rc = vw_cr( conf_ocb, S_NULL, 0, NIL );
      OK_TO_VIEW_CONFERENCE = FALSE;

   }
}                                          /* if conf_ocb was OK */
}                                          /* If NOT on MS       */
else
{

   invoker = t_invoker();
   if( OCB_POINTER_OK( some_ocb ) )
   {

      conf_ocb = some_ocb;
      packet   = provided_conference_names;

      pkt_ptr = s_buffer( packet );
      user_ocb = obj_opn( (OID *) pkt_ptr, OBJ_UPDATE );

      pkt_ptr += sizeof( OID );
      membership = obj_opn( (OID *) pkt_ptr, OBJ_UPDATE );
      membership_opened = TRUE;

      pkt_ptr += sizeof( OID );
      time_stamp = (datetime *) pkt_ptr;

      pkt_ptr += sizeof( datetime );
      seqnum =  (sint *) pkt_ptr;

      pkt_ptr += 4;
      cc_count =  (sint *) pkt_ptr;


      ocd_new( & user_ocd, NIL                  );
      ocd_new( & conf_ocd, "CONFERENCE.ID"      );
      obj_get(   conf_ocb,  conf_ocd, & conf_id );

      ocd_new( & memb_ocd,  NIL                 );
      oid_key(   memb_ocd, & (user_ocb->oid)    );

      if( level & JOINING_PUBLIC_CONF )
      {

         s_init( &new_role, "MEMBER" );  /**** Change to prvkey **/
                                         /* once we intergrate privs*/
                                     /* Entry for user in memb obj  */
         s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.ROLES" );
         obj_rep( membership, memb_ocd, s_buffer(new_role), SEG_UPDATE);
         s_drop( &new_role );

         os_dttm( &cur_dt );
         s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.ADD_DATE" );
         obj_rep( membership, memb_ocd, &cur_dt, SEG_UPDATE);

         count = 0;
         s_init( &(memb_ocd->name), "MEMBERSHIP.COUNT" );
         obj_get( membership, memb_ocd, &count );
         count++;
         obj_rep( membership, memb_ocd, &count, SEG_UPDATE);

         count = 0;
         s_init( &(memb_ocd->name), "MEMBERSHIP.WRITER" );
         obj_get( membership, memb_ocd, &count );
         count++;
         obj_rep( membership, memb_ocd, &count, SEG_UPDATE);


         rc = add_to_user_list( user_ocb, "MEMBERSHIPS.CONFERENCES",
                                conf_id, & (conf_ocb->oid), 0, S_NULL );
         s_drop( & conf_id );

         s_init( &(user_ocd->name), "MEMBERSHIPS.CONF_COUNT" );
         obj_get(  user_ocb, user_ocd, &conf_c );
         conf_c++;
         obj_rep(  user_ocb, user_ocd, &conf_c, SEG_UPDATE);

      }  /* if conf is public */

      count = 0;
      s_init(&user_ocd->name,"INDEXES.CC_COUNT");
      obj_get( user_ocb, user_ocd, &count );
      count += *cc_count;
      obj_rep( user_ocb, user_ocd, &count, SEG_UPDATE );
                                    /* finaly set our marker        */
      if( *seqnum != 0 )     /* Is there any item we're using as our*/
      {                      /* marker? In other words, did we hit  */
                             /* either ENTER, PROCESS or LAST PF key*/

                             /* Lets change our begining window     */
                             /* marker to seqnum                    */
         rc = memb_rc( membership, S_NULL, (level | MEMB_SET_MARKER), *seqnum,
                                        &(TM_master->ucb->user_id) );

         s_init(&(memb_ocd->name),"MEMBERSHIP.MEMBERS.LAST_ITEM_READ");
         obj_rep( membership, memb_ocd, seqnum, SEG_UPDATE );

      }  /* if seqnum <> 0 */

      s_init( &(memb_ocd->name), "MEMBERSHIP.MEMBERS.LAST_ACTIVE" );
      obj_rep( membership, memb_ocd, time_stamp, SEG_UPDATE );

                                       /**- - - - - - - - - - - - - - **/
                                       /* Send back to the user the    */
                                       /* Membership object and their  */
                                       /* memberships information as we*/
                                       /* changed them here.           */
                                       /**- - - - - - - - - - - - - - **/

      t_m_obj( invoker, membership, S_NULL, FALSE, 0 );

      s_alloc( & senddata,    12 );
      s_smp_ul(  senddata ) = 12  ;

      ocd_new( & user_ocd, "MEMBERSHIPS" );
      rc = obj_pack( & senddata, user_ocb, user_ocd );

      rc = t_send( invoker, command, flags, 0, senddata );

      s_drop(  & senddata );
      ocd_drp( & user_ocd );
      ocd_drp( & conf_ocd );
      ocd_drp( & memb_ocd );
      obj_cls(   user_ocb );

      rc = DO_NOT_CONFIRM_TRANSACTION;

   }
   else
   {

      rc = INVALID_OCB;
      t_csend( invoker, rc, S_NULL );

   }
}
if( membership_opened )
{

  obj_cls( membership );

}

   return(rc);

}
