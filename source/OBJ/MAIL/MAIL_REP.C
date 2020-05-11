/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    mail_reply()                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 26, 1990                                         |
  |  <LEVEL>   OBJ_MAIL                                               |
  |  <PARAM>   mail_ocb    OCB *      INPUT    Mail object to check for
  |                                             and format reply info.|
  |  <PARAM>   reply_to    OCB **     INPUT    Object to reply to.    |
  |  <PARAM>   reply_to_id string     INPUT    Item ID to reply to.   |
  |  <PURPOSE> Check the mail object to see if a reply is allowed.  If|
  |            so, format the mail object with the necessary reply    |
  |            information and prepare the create mail form with the  |
  |            necessary reply data.                                  |
  |                                                                   |
  |            If both reply_to_object and reply_to_id are provided,  |
  |            the object has precedence and the string is ignored.   |
  |  <OUTPUT>  sint                                                   |
  |  <ERRORS>  CANNOT_REPLY_TO_ACTIVITY, MAIL_CANNOT_ASSOCIATE_REPLY, |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isv.h"
#include "tm.h"
#include "obj_mail.h"




sint   mail_reply(  OCB       * mail_object,
                    OCB      ** where_to_store_reply_to_object,
                    string      reply_to_id         )


{

   sint        rc            =      0;
   OID         activity_oid          ,
             * reply_oid     =    NIL;
   OCB       * reply_object  =    NIL,
             * reply_to_object =  NIL;
   OCD       * component     =    NIL;
   string      mail_root_id  = S_NULL;
   boolean     reply         =  FALSE;





if( where_to_store_reply_to_object != NIL )
{

   reply_to_object = * where_to_store_reply_to_object;

}
if( OCB_POINTER_OK( reply_to_object ) )
{

   /*================================================================+
   |  If message.isv_type is not 0 then already has activity attached|
   |  and we cannot allow a reply to the reply to object.            |
   +================================================================*/

   os_memn( & activity_oid, sizeof( OID ) );
   rc = obj_find( reply_to_object, "MESSAGE.ACTIVITY", & activity_oid );
   if( rc == 0  &&  ! OID_IS_NULL( ( & activity_oid ) ) )
   {

      rc    = CANNOT_REPLY_TO_ACTIVITY;
      reply = FALSE;
      obj_find( reply_to_object, "MESSAGE.ID", & mail_root_id );
      except( rc, USER_ERROR, mail_root_id,
                                     S_NULL, S_NULL, S_NULL, S_NULL );
      s_drop( & mail_root_id );

   }
   else
   {

      reply        =            TRUE;
      reply_object = reply_to_object;

   }
}
else if ( s_len( reply_to_id ) > 0 )
{

   if(
       obj_item( reply_to_id, OBJ_LEVEL_ZERO, & reply_oid, NIL, S_NULL,
                                                           NIL, ANY_OBJ )
                == 0 )
   {

      reply_object = obj_opn( reply_oid, OBJ_READ );
      if( OCB_POINTER_OK( reply_object ) )
      {

         reply = TRUE;
         if( where_to_store_reply_to_object != NIL )
         {

            * where_to_store_reply_to_object = reply_to_object;

         }
      }
      else
      {

         reply = FALSE;
         rc    = MAIL_CANNOT_ASSOCIATE_REPLY;
         except( rc, USER_ERROR, reply_to_id,
                                     S_NULL, S_NULL, S_NULL, S_NULL );

      }
      os_memd( reply_oid );

   }
}
ocd_new( & component,   "MESSAGE.REPLY"                );
obj_rep(   mail_object, component, & reply, SEG_UPDATE );

if( reply == TRUE )
{

   string separator        = S_NULL,
          holder           = S_NULL,
          reply_to         = S_NULL,
          reply_to_spill   = S_NULL,
          nick_name        = S_NULL,
          author_nick_name = S_NULL,
          REPLY_ID         = S_NULL,
          TITLE            = S_NULL,
          SENDTO1          = S_NULL,
          SENDTO2          = S_NULL,
          item_prefix      = S_NULL,
          blank            = S_NULL,
          attach_id        = S_NULL,
          comma            = S_NULL;
   OCD  * author_ocd       =    NIL;
   sint   position         =      0;
   OID    author_oid               ;


   s_init(   & separator, ":"        );
   s_init(   & blank,     " "        );
   s_init(   & comma,     ","        );
   s_init(   & REPLY_ID,  "REPLY_ID" );
   s_init(   & TITLE,     "TITLE"    );
   s_init(   & SENDTO1,   "SENDTO1"  );
   s_init(   & SENDTO2,   "SENDTO2"  );

   obj_update( mail_object, "%LINKS.OWNER", & (reply_object->oid) );

   obj_find(   reply_object, "MESSAGE.ID", & holder );
   s_trim(   & holder, 'B'                       );
   isv_get(  & item_prefix, ISV_MMSG,       1    );
   s_cat(    & holder, blank,       holder, 0, 0 );
   s_cat(    & holder, item_prefix, holder, 0, 0 );
   var_set(    REPLY_ID,  holder                 );
   s_copy(   & attach_id,           holder, 0, 0 );
   s_nulout( & holder                            );

   s_init(   & (component->name), "MESSAGE.TITLE" );
   rc = obj_get( reply_object, component, & holder   );


   rc = obj_rep(   mail_object, component, holder, SEG_UPDATE );
   s_drop( & holder );

   s_init( & (component->name), "MESSAGE.TO" );
   rc = obj_get( reply_object, component, & reply_to  );


      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |  Now remove the name of the replying author from the original to|
      |  list so we do not send this mail to ourselves.  We can get the |
      |  author nick name locally since Signon updated that object here.|
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   obj_find( TM_master->ucb->directory, "NAME.NICKNAME", & nick_name);
   position = s_first( nick_name, reply_to, 0, 0 );

   if( position > 0 )
   {

      string   front_portion = S_NULL,
               back_portion  = S_NULL;


      if ( position != 1 )
      {

         s_copy( & front_portion, reply_to, 0, position - 1 );

      }
      s_copy( & back_portion,  reply_to,
                          position + s_len( nick_name ) + 2,   0    );


      s_cat(   & reply_to, front_portion, back_portion, 0, 0  );
      s_trim(  & reply_to, 'T'      );
      s_ctrim( & reply_to, 'T', ',' );

   }

   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
   |  Next, add the author of the original mail as the first addresse|
   |  on the reply.  We must go to the MS to get the author's Nicknam|
   |  since it may have changed since this mail was sent. Also note  |
   |  if the original author is already in the To list we do not do  |
   |  the add as there will be duplicate names otherwise.            |
   \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   os_memn( & author_oid, sizeof( OID ) );
   rc = obj_find( reply_object, "%LINKS.AUTHOR", & author_oid );
   if( ! oid_cmp( TM_master->ucb->directory->oid,  author_oid ) )
   {


      ocd_new( & author_ocd, "SNI.OBJECT.NICKNAME" );
      oid_key(   author_ocd,  & author_oid );
      rc = t_fndsys(  SNI_OBJ,  author_ocd, & author_nick_name );


      if( rc == 0 )
      {

         position = 0;
         s_upper( reply_to, 0, 0 );
         position = s_first( author_nick_name, reply_to, 0, 0 );
         if (position == 0)
         {

            s_cat( & author_nick_name, author_nick_name, comma, 0, 0 );
            s_cat( & reply_to, author_nick_name,  reply_to,     0, 0 );

         }
         else
         {


         }
      }
      else
      {


      }
   }
   else
   {


   }

   rc = obj_update( mail_object, "MESSAGE.TO", reply_to );

   /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
   |  Handle field wrap if the Send To names cannot fit on one line. |
   |  SENDTO1 and SENDTO2 map to form fields since forms cannot take |
   |  care of the spillover itself.  TWEEK since we actually cannot  |
   |  know the amount of field space for the names so eventually this|
   |  code should come out of here and be integrated into FORMS.     |
   \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   if ( s_len( reply_to )  >  62 )
   {

      position = s_last( comma, reply_to, 0, 62 );
      s_copy( & reply_to_spill, reply_to, position + 1,
                                       s_len( reply_to ) - position );
      s_copy( & reply_to,  reply_to, 0, position);

   }

   var_set( SENDTO1, reply_to       );
   var_set( SENDTO2, reply_to_spill );

   s_drop (  & reply_to_spill   );
   s_drop (  & author_nick_name );
   s_drop (  & separator        );
   s_drop (  & reply_to         );
   s_drop (  & nick_name        );
   s_drop(   & REPLY_ID         );
   s_drop(   & TITLE            );
   s_drop(   & SENDTO1          );
   s_drop(   & SENDTO2          );
   ocd_drp(  & author_ocd       );

}
ocd_drp( & component );

   return(rc);

}
