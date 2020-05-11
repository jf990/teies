/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_updcnt()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 4, 1990                                        |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   user_object   OCB *    INPUT  User to update counters. |
  |                                                                   |
  |  <PURPOSE> Update the NEW, OLD, MARKED and UNDONE counters for a  |
  |            given user.  If the user object is NIL the do it for   |
  |            the user we consider is the current user.  If every-   |
  |            thing works request a confirmation be send back to the |
  |            requestor.                                             |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   t_author()                                             |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint   t_updcnt(   user_object   )

OCB             *  user_object;


{

   sint        rc             =      0,
               count          =      0;
   OID         list_oid               ;
   OCB       * list_object    =    NIL;
   OCD       * count_ocd      =    NIL;
   boolean     opened_locally =  FALSE;





if( ! OCB_POINTER_OK( user_object ) )
{

   user_object    = t_author();
   opened_locally = TRUE;

}
if( OCB_POINTER_OK( user_object ) )
{

   ocd_new( & count_ocd, "INDEXES.MAILBOX" );

   rc = obj_get( user_object, count_ocd, & list_oid );
   if( rc == 0  &&  ( ! OID_IS_NULL( & list_oid ) ) )
   {

      list_object = obj_opn( & list_oid, OBJ_READ );
      if( OCB_POINTER_OK( list_object ) )
      {

         count = obj_qns( list_object, "LIST.LIST" );
         s_init( &(count_ocd->name), "INDEXES.MAIL_COUNT" );
         rc = obj_rep( user_object, count_ocd, & count, SEG_HOLD );
         obj_cls( list_object );

      }
   }

   s_init( &(count_ocd->name), "INDEXES.NOTIFICATIONS" );

   rc = obj_get( user_object, count_ocd, & list_oid );
   if( rc == 0  &&  ( ! OID_IS_NULL( & list_oid ) ) )
   {

      list_object = obj_opn( & list_oid, OBJ_UPDATE );
      if( OCB_POINTER_OK( list_object ) )
      {

         count = obj_qns( list_object, "LIST.LIST" );
         s_init( &(count_ocd->name), "INDEXES.NEW_NOTIF_COUNT" );
         rc = obj_rep( user_object, count_ocd, & count, SEG_HOLD );

      }
   }

   s_init( &(count_ocd->name), "INDEXES.ACTIVITIES" );

   rc = obj_get( user_object, count_ocd, & list_oid );
   if( rc == 0   &&   ( ! OID_IS_NULL( & list_oid ) ) )
   {

      list_object = obj_opn( & list_oid, OBJ_UPDATE );
      if( OCB_POINTER_OK( list_object ) )
      {

         count = obj_qns( list_object, "LIST.LIST" );
         s_init( &(count_ocd->name), "INDEXES.ACT_COUNT" );
         rc = obj_rep( user_object, count_ocd, & count, SEG_HOLD );

      }
   }

   s_init( &(count_ocd->name), "INDEXES.TICKETS_GRANTED" );

   rc = obj_get( user_object, count_ocd, & list_oid );
   if( rc == 0   &&   ( ! OID_IS_NULL( & list_oid ) ) )
   {

      list_object = obj_opn( & list_oid, OBJ_UPDATE );
      if( OCB_POINTER_OK( list_object ) )
      {

         count = obj_qns( list_object, "LIST.LIST" );
         s_init( &(count_ocd->name), "INDEXES.TICK_GRANT_CNT" );
         rc = obj_rep( user_object, count_ocd, & count, SEG_HOLD );

      }
   }

   s_init( &(count_ocd->name), "INDEXES.UNDONE_ACT" );

   rc = obj_get( user_object, count_ocd, & list_oid );
   if( rc == 0  &&  ( ! OID_IS_NULL( & list_oid ) ) )
   {

      list_object = obj_opn( & list_oid, OBJ_UPDATE );
      if( OCB_POINTER_OK( list_object ) )
      {

         count = obj_qns( list_object, "LIST.LIST" );
         s_init( &(count_ocd->name), "INDEXES.UNDONE_COUNT" );
         rc = obj_rep( user_object, count_ocd, & count, SEG_HOLD );

      }
   }

   ocd_drp(    & count_ocd   );
   obj_rep(      user_object, NIL, NIL, SEG_UPDATE );
   obj_set_mode( user_object,           OBJ_FLUSH  );
   obj_cls(      user_object );

   if( opened_locally )
   {

      obj_cls( user_object );

   }
}
else
{

   rc = INVALID_OCB ;

}
TM_master->current->except = rc;
rc = TM_SEND_CONFIRMATION;

   return(rc);

}
