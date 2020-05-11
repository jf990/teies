/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    rc_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 10, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   mail        OCB *      INPUT  Mail object to receive   |
  |  <PARAM>   modifier    string     INPUT                           |
  |  <PARAM>   level       hword      INPUT                           |
  |  <PARAM>   user        OCB *      INPUT  User receiving.          |
  |                                                                   |
  |  <PURPOSE> Receive Mail.  If the User OCB is NIL assume the       |
  |            current user.                                          |
  |                                                                   |
  |            If OCB is bad, then validate the Modifier as a Mail ID |
  |            to be received.                                        |
  |                                                                   |
  |            Receive moves a piece of mail indexed in the users     |
  |            New Mail Box to the users Old Mail Box.                |
  |                                                                   |
  |            If the confirmation should be generated cr_notif()     |
  |            is called for a mail confirmation type notification.   |
  |                                                                   |
  |            The request is done locally and sent to the MS.        |
  |                                                                   |
  |            The exception handler is used to report the mail has   |
  |            been received to the user.                             |
  |                                                                   |
  |  <OUTPUT>  sint                          result code              |
  |  <CALLS>   cr_ntf(), except()                                     |
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


sint     rc_mail(  mail_ocb, modifier, level, receiver_ocb  )

OCB               *mail_ocb;
string             modifier;
hword              level;
OCB               *receiver_ocb;



{

   sint     rc             =         0,
            old_mail_count =         0,
            waiting        =         0,
            type_of_mail   =         0;
   hword    mail_type      =         0;
   OID     *mail_oid       =       NIL;
   string   mail_key       =    S_NULL,
            mail_id        =    S_NULL,
            author_name    =    S_NULL,
            date_received  =    S_NULL,
            invoker        =    S_NULL,
            result_string  =    S_NULL;
   datetime              mail_creation;
   boolean  opened_root_locally =     FALSE,
            opened_user_locally =     FALSE;




if( ! OCB_POINTER_OK( mail_ocb ) || level == LINK_MAIL_VALIDATE_MODIFIER)
{

   rc = obj_item( modifier, OBJ_LEVEL_ZERO, & mail_oid, NIL,
                                           NIL, NIL, ANY_OBJ );
   if( ! OID_POINTER_OK( mail_oid ) )
   {
      rc = NO_MAIL_TO_RECEIVE;
   }
   else
   {
      mail_ocb = obj_opn( mail_oid, OBJ_UPDATE );
      opened_root_locally = TRUE;
   }
}
if( rc == 0 )
{

   if( TM_master->usrtype == MASTER_SERVER )
   {

      if( level == LINK_MAIL_KEY_PROVIDED )
      {

         s_copy(  & mail_key, modifier,  0, 0 );

      }
      else
      {

         obj_find(  mail_ocb, "MESSAGE.TYPE",        & type_of_mail );
         obj_find(  mail_ocb, "%SYSTEM.CREATE_DATE", & mail_creation);
         s_alloc( & mail_key,     sizeof( datetime ) + 2 );
         s_smp_ul(  mail_key  ) = sizeof( datetime ) + 2  ;
         mail_type = (hword) type_of_mail ;
         os_memc( & mail_type,      s_buffer( mail_key ),  2 );
         os_memc( &(mail_creation), s_buffer( mail_key )+2, sizeof( datetime));

      }

      invoker = t_invoker();

      if( (NOT OCB_POINTER_OK( receiver_ocb )) ||
          (receiver_ocb->type != USER_OBJ    )    )
      {

         receiver_ocb = t_author();
         opened_user_locally = TRUE;

      }

      rc = remove_from_user_list( receiver_ocb, "INDEXES.MAILBOX",
                                  mail_key, NIL, 0, S_NULL           );

      if( rc == 0 )
      {

         rc = add_to_user_list( receiver_ocb, "INDEXES.OLD_MAIL",
                                mail_key, & (mail_ocb->oid), 0, S_NULL );

         level = AUTOMATIC_CREATION ;
         rc    = cr_ntf( mail_ocb, modifier, level,
                                              (OCB **) & result_string );
         rc    = t_increment_items_received( t_conn( invoker ) );

      }
      t_csend( invoker, rc, result_string );
      s_drop(  & mail_key     );
      os_memd(   mail_oid     );
      if( opened_user_locally )
      {

         obj_cls( receiver_ocb );

      }

   }
   else
   {


      receiver_ocb = t_author();

      rc = remove_from_user_list( receiver_ocb, "INDEXES.MAILBOX",
                                  modifier, NIL, 0, S_NULL         );

      rc = t_remote( TM_master->MS, MAIL_OBJ, RECEIVE_ACT, level,
                                    mail_ocb,  modifier, S_NULL        );
      if (rc == 0)
      {

         obj_find( mail_ocb, "MESSAGE.ID", & mail_id );
         t_gtdate( & date_received, NIL );
         except( MAIL_OBJECT_RECEIVED, USER_INFORMATION, mail_id,
                           date_received, author_name, S_NULL, S_NULL );
      }
      else
      {
         except( rc , USER_WARNING, S_NULL, S_NULL, S_NULL, S_NULL,
                                                                S_NULL );
      }
      s_drop( & mail_id       );
      s_drop( & date_received );

   }
}
else
{

   if( TM_master->usrtype == MASTER_SERVER )
   {

      t_csend( t_invoker(), rc, result_string );

   }
}
if( opened_root_locally )
{

   obj_cls( mail_ocb );

}


   return(rc);

}
