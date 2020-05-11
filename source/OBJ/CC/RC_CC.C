/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    rc_cc                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 28, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   comment_object    OCB * INPUT description              |
  |  <PARAM>   conference_object OCB * INPUT description              |
  |  <PARAM>   flags             hword INPUT description              |
  |  <PARAM>   membership_object OCB * INPUT description              |
  |  <PURPOSE> Receive Conference Comment                             |
  |                                                                   |
  |            User server sends to the Master a packet containing    |
  |            information to update the users marker in the conference
  |            Packet is formated as:                                 |
  |                                                                   |
  |          +-------+------------+----------+---------------+        |
  |          | Users | Membership | Comment  | CONFERENCE.ID |        |
  |          |  OID  |    OID     | Sequence |               |        |
  |          |       |            |  Number  |               |        |
  |          +---8---+-----8------+----4-----+------STR------+        |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "obj.h"
#include "obj_cc.h"
#include "obj_cr.h"
#include "obj_memb.h"


sint rc_cc( OCB      *  comment_object   ,
            OCB      *  conference_object,
            hword       flags            ,
            OCB      *  membership_object    )

{

sint      rc           = NO_ERROR,
          cc_count     =        0,
          new_cc_count =        0,
          seq_num      =        0,
        * seqnum_ptr   =      NIL;
byte    * packet       =      NIL;
string    result_str   =   S_NULL,
          pkt_str      =   S_NULL;
OCB     * user_ocb     =     NIL ;
OCD     * component    =     NIL ;
datetime  time_stamp             ;




if( TM_master->usrtype == USER_SERVER )
{

if( NOT OCB_POINTER_OK( comment_object ) )
{                           /* In future we might want to prompt for   */
                            /* comment id If coming from command line  */
   rc = INVALID_OCB;

}
else
{

   ocd_new( & component, "COMMENT.SEQNUM"          );
   obj_get(   comment_object, component, & seq_num );

   s_init(  &(component->name), "CONFERENCE.ID"    );
   obj_get(   conference_object, component, & (component->key) );

   rc = memb_rc(        membership_object,
                       NIL, flags, seq_num,
                 & ( TM_master->ucb->user_id ) );

   if( rc == ITEM_ALREADY_RECEIVED )
   {

      rc = NO_ERROR;

   }
   else if( rc == NO_ERROR )
   {
                                       /*==============================*/
                                       /* Update info & counters on MS */
                                       /* And MS will send packet back */
                                       /* with all the updates         */
                                       /* Allocate space for pkt we're */
                                       /* sending to MS                */
                                       /*==============================*/
      s_alloc( & pkt_str, 20 );
      packet = s_buffer( pkt_str );

      os_memc( &(TM_master->ucb->user_id), packet, sizeof(OID) );
      packet += sizeof( OID );

      os_memc( &(membership_object->oid), packet, sizeof(OID) );
      packet += sizeof( OID );

      os_memc( &seq_num, packet, sizeof(sint) );
      packet += sizeof( sint );

      s_smp_ul( pkt_str ) = packet - s_buffer( pkt_str );
      s_catx( & pkt_str, component->key, 0, 0 );


      rc = t_remote( TM_master->MS, COMMENT_OBJ, RECEIVE_ACT, flags,
                                            NIL, pkt_str, & result_str );

      if( rc == ITEM_ALREADY_RECEIVED )
      {

         rc = NO_ERROR;

      }
      else if( rc == NO_ERROR )
      {

         packet = s_buffer( result_str );
                                            /*=========================*/
                                            /* Point to data from MS   */
                                            /* which has total new cc  */
                                            /* waiting for user and new*/
                                            /* CCs in this particular  */
                                            /* conference              */
                                            /*=========================*/

         user_ocb = t_author();
         s_init( &(component->name), "INDEXES.CC_COUNT" );
         obj_rep( user_ocb, component, packet, SEG_UPDATE );
         packet += sizeof( sint );
         obj_rep( user_ocb, component, packet, SEG_UPDATE );
         obj_cls( user_ocb );

      }
   }
   ocd_drp( & component );
   s_drop(  & pkt_str   );

}  /* If CC_OCB passed was good */

}  /* If NOT on MS */
else
{

   pkt_str = (string) conference_object;
   packet = s_buffer( pkt_str );

   user_ocb = obj_opn( (OID *) packet, OBJ_UPDATE );
   packet += sizeof( OID );

   membership_object = obj_opn( (OID *) packet, OBJ_UPDATE );
   packet += sizeof( OID );

   seq_num = *((sint *) packet);
   packet += sizeof( sint );

   rc = memb_rc( membership_object, NIL, flags, seq_num, &(user_ocb->oid) );

   if( rc == NO_ERROR )
   {

      ocd_new( & component, "INDEXES.CC_COUNT" );
      s_copy(  &(component->key), (string) packet, 0, 0 );
      obj_get(   user_ocb, component, & new_cc_count );
      new_cc_count--;
      obj_rep(   user_ocb, component, & new_cc_count, SEG_UPDATE );

      pkt_str = S_NULL;
      s_alloc( & pkt_str,    sizeof(sint) * 2 );
      s_smp_ul(  pkt_str ) = sizeof(sint) * 2  ;
      packet = s_buffer( pkt_str );

      os_memc( & new_cc_count, packet, sizeof(sint) );
      packet += sizeof(sint);
      os_memc( & new_cc_count, packet, sizeof(sint) );

   }
   t_csend( t_invoker(), rc, pkt_str );

   s_drop( & pkt_str           );
   obj_cls(  user_ocb          );
   obj_cls(  membership_object );

   rc = DO_NOT_CONFIRM_TRANSACTION;

}  /* If on MS */

   return(rc);

}
