/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    rc_ntf()                                               |
  |  <AUTHOR>  Tanmay S. Kumar                                        |
  |  <DATE>    January 6, 1989                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   notif_ocb   OCB *      INPUT  notif object to receive  |
  |  <PARAM>   modifier    string     INPUT  has notif key to receive |
  |  <PARAM>   level       hword      INPUT  not really used          |
  |  <PARAM>   params      byte *     INPUT  not yet used  .          |
  |                                                                   |
  |  <PURPOSE> Receive the notification. If the user OCB is NIL assume|
  |            the current user.                                      |
  |                                                                   |
  |            Receive moves a notification  indexed in the users     |
  |            New Notifications to the users old notifications.      |
  |                                                                   |
  |            The request is done locally and sent to the MS.        |
  |                                                                   |
  |            The exception handler is used to report that the notif |
  |            has been received, to the user.                        |
  |                                                                   |
  |  <OUTPUT>  sint                          result code              |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "obj_prim.h"


sint     rc_ntf(notif_ocb, modifier, level, params)

OCB               *notif_ocb;
string             modifier;
hword              level;
byte              *params;



{

 sint     rc               =        0;
 OCD      *user_ocd        =       NIL;
 OCD      *notif_ocd       =       NIL;
 OCB      *user_ocb        =       NIL;

 string   result_string    =    S_NULL,
          invoker          =    S_NULL;

 OID      author_oid;





 if (OCB_POINTER_OK( notif_ocb ) )
 {

    if (TM_master->usrtype == MASTER_SERVER )
    {
       user_ocb = t_author();

       rc = remove_from_user_list( user_ocb, "INDEXES.NOTIFICATIONS",
                                   modifier, NIL, 0, S_NULL          );

       if (rc == 0)
       {

          rc = add_to_user_list( user_ocb, "INDEXES.OLD_NOTIF",
                                 modifier, & (notif_ocb->oid),
                                 0, S_NULL                     );

       }
       invoker = t_invoker();
       rc      = t_increment_items_received( t_conn( invoker ) );
       t_csend( invoker, rc, result_string );
       obj_cls(  user_ocb  );

   }
   else
   {

      user_ocb = t_author();

      rc = remove_from_user_list( user_ocb, "INDEXES.NOTIFICATIONS",
                                  modifier, NIL, 0, S_NULL          );
      if (rc == 0)
      {


         rc = t_remote( TM_master->MS, NOTIF_OBJ, RECEIVE_ACT, level,
                      notif_ocb,  modifier , (void **)(&result_string));
         if (rc == 0)
         {
            s_trim(&modifier, 'L');
            except( NTF_OBJECT_RECEIVED, USER_INFORMATION, modifier,
                                       S_NULL, S_NULL, S_NULL, S_NULL );
         }
      }
      if (rc != 0)
      {
         s_trim(&modifier, 'L');
         except( rc , USER_WARNING, modifier, S_NULL, S_NULL, S_NULL,
                                                                S_NULL );
      }
      ocd_drp( & user_ocd );

   }
}
else
{

   rc = INVALID_OCB;

}

   return(rc);

}
