/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_id                                                 |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 18, 1989                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object_oid  OID *      INPUT  description              |
  |  <PARAM>   item_id     string *   INPUT  description              |
  |                                                                   |
  |  <PURPOSE> Given an object ID, return the user interface item ID  |
  |            of that object.                                        |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isv.h"
#include "obj.h"
#include "tm.h"




sint   obj_id(  oid, item_id  )

OID           * oid;
string        * item_id;

{

   sint        rc         =      0;
   hword       isvkey     =      0;
   byte      * component  =    NIL;
   string      item_name  = S_NULL,
               blank      = S_NULL,
               item_isv   = S_NULL,
               id         = S_NULL;
   OCD       * ocd        =    NIL;
   OCB       * object     =    NIL;



if( OID_POINTER_OK( oid ) )
{

   if( item_id != S_NULL )
   {


      object = obj_opn( oid, OBJ_READ );
      if( OCB_POINTER_OK( object ) )
      {


         switch( object->type )
         {

            case         USER_OBJ   :

               component = "NAME.NUMBER";
               isvkey    = ISV_USER     ;
               break;

            case        GROUP_OBJ   :

               component = "GROUP.ID";
               isvkey    = ISV_GROUP ;
               break;

            case      NETWORK_OBJ   :

               component = NIL;
               isvkey    = 0  ;
               break;

            case         CONF_OBJ   :

               component = "CONFERENCE.ID";
               isvkey    = ISV_CONF       ;
               break;

            case      COMMENT_OBJ   :

               component = "COMMENT.ID";
               isvkey    = ISV_CC      ;
               break;

            case         MAIL_OBJ   :

               component = "MESSAGE.ID";
               isvkey    = ISV_MMSG    ;
               break;

            case NOTIFICATION_OBJ   :

               component = NIL      ;
               isvkey    = ISV_NOTIF;
               break;

            case     ACTIVITY_OBJ   :

               component = "ACTIVITY.ID";
               isvkey    = ISV_ACTIVITY ;
               break;

            case   MEMBERSHIP_OBJ   :

               component = NIL     ;
               isvkey    = ISV_MEMB;
               break;

            case         LIST_OBJ   :

               component = NIL     ;
               isvkey    = ISV_LIST;
               break;

            case       TICKET_OBJ   :

               component = NIL       ;
               isvkey    = ISV_TICKET;
               break;

            case     CONTENTS_OBJ   :

               component = NIL         ;
               isvkey    = ISV_CONTENTS;
               break;

            case          SYS_OBJ   :
            case          SGI_OBJ   :
            case          PSI_OBJ   :
            case        SOIDI_OBJ   :
            case        SNTFI_OBJ   :
            case          SKI_OBJ   :
            case          SAI_OBJ   :
            case         SDTI_OBJ   :
            case          CRI_OBJ   :
            case        SODTI_OBJ   :
            case          ARI_OBJ   :
            case          SHI_OBJ   :
            case          SNI_OBJ   :
            case        SERRI_OBJ   :
            case          SFI_OBJ   :
            case        SDELI_OBJ   :
            case          SMI_OBJ   :

               component = NIL;
               isvkey    = ISV_PSI;
               break;

            default                 :

               break;

         }

         if( isvkey > 0 )
         {

            rc = isv_get( & item_isv, isvkey, 0 );

         }
         if( component != NIL )
         {

            ocd_new( & ocd, component );
            rc = obj_get( object, ocd, & id );

         }
         if( s_len( id ) > 0   &&   s_len( item_isv ) > 0 )
         {

            s_init( & blank, " " );
            s_copy( & item_name, item_isv,         1, 1 );
            s_cat(  & item_name, item_name, blank, 0, 0 );
            s_cat(  & id,        item_name, id,    0, 0 );
            s_drop( & blank );

         }
         if( s_len( id ) > 0 )
         {

            s_copy( item_id, id, 0, 0 );

         }
         s_drop(  & id       );
         s_drop(  & item_name);
         ocd_drp( & ocd      );
         obj_close( object   );

      }
      else
      {

         if( TM_master->usrtype != MASTER_SERVER )
         {

            string oid_str = S_NULL;


            s_alloc( & oid_str, sizeof( OID ) );
            os_memc(   oid, s_buffer( oid_str ), sizeof( OID ) );
            s_smp_ul( oid_str ) = sizeof( OID );
            rc = t_send( TM_master->MS, RECEIVE_OBJECT_ID, 0, 0, oid_str );
            rc = t_waitc( TM_master->MS, item_id );
            s_drop( & oid_str );

         }
         else
         {

            rc = OID_NOT_IN_SOIDI;

         }
      }
   }
   else
   {

      rc = INVALID_DATA_POINTER;

   }
}
else
{

   rc = INVALID_OID_POINTER;

}




   return(rc);

}
