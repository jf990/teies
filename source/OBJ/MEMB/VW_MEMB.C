/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    vw_memb                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PURPOSE> View Membership List                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isv.h"
#include "tm.h"
#include "vars.h"
#include "obj.h"
#include "obj_memb.h"




sint    vw_memb( memb_ocb, modifier, level, owner_ocb )

OCB             *memb_ocb,  *owner_ocb;
string           modifier;
hword            level;

{

OID        oid                             ,
         * oid_ptr               = &    oid;
sint       rc                    = NO_ERROR,
           bad_itm_num           =        0;
string     bad_itm_str           =   S_NULL;
byte     * ocd_name              =      NIL;
boolean    OWNER_OCB_OPENED_HERE =    FALSE,
           MEMB_OCB_OPENED_HERE  =    FALSE;



#D_begin( vw_memb, OBJ_MEMB );

if( OCB_POINTER_OK( memb_ocb ) == FALSE )
{

   if( s_len( modifier ) > 0 )
   {

      if( (rc == obj_item( modifier, OBJ_NEED_ONE_OID, &oid_ptr, NIL,
                      &bad_itm_str, &bad_itm_num, ANY_OBJ)) == NO_ERROR )
      {

         owner_ocb = obj_opn( oid_ptr, OBJ_UPDATE );

         if( OCB_POINTER_OK( owner_ocb ) )
         {

            OWNER_OCB_OPENED_HERE = TRUE;

            switch( owner_ocb->type )
            {

            case CONF_OBJ :

               ocd_name = "CONFERENCE.MEMBERS";
               break;

            case GROUP_OBJ :

               ocd_name = "GROUP.MEMBERS";
               break;

            case ACTIVITY_OBJ :

               ocd_name = "ACTIVITY.MEMBERS";
               break;

            default :

               #D_e_say( 'Cannot handle object type for membership.' );
               break;

            }  /* Switch */

            if( (rc == obj_find( owner_ocb, ocd_name, &oid )) == NO_ERR )
            {

               memb_ocb = obj_opn( &oid, OBJ_UPDATE );

               if( OCB_POINTER_OK( memb_ocb ) )
               {

                  MEMB_OCB_OPENED_HERE = TRUE;

               }
               else
               {

                  rc = INVALID_OCB;

               }
            }
         }
         else
         {

            rc = INVALID_OCB;

         }
      }
   }
   else
   {

      rc = INVALID_OCB;

   }
}
else
{

   if( OCB_POINTER_OK( owner_ocb ) == FALSE )
   {

      if( (rc = obj_find( memb_ocb, "%LINKS.OWNER", &oid )) == NO_ERR )
      {

         owner_ocb = obj_opn( &oid, OBJ_UPDATE );
         if( OCB_POINTER_OK( owner_ocb ) )
         {

            OWNER_OCB_OPENED_HERE = TRUE;

         }
         else
         {

            rc = INVALID_OWNER_OCB;

         }
      }
   }
}
if( rc == NO_ERROR )
{

string   form_id        = S_NULL,
         blank          = S_NULL,
         tmpstr         = S_NULL,
         tmpstr2        = S_NULL,
         OBJ_TYP        = S_NULL,
         OBJ_ID         = S_NULL,
         OBJ_LABEL      = S_NULL,
         OBJ_TITLE      = S_NULL,
         owner_typ      = S_NULL,
         owner_lab      = S_NULL,
         owner_tit      = S_NULL,
         owner_id       = S_NULL;
sint     end_function   = 0;
boolean  done           = FALSE;



   s_init( & form_id,      "1190"         );
   s_init( & OBJ_TYP,      "OBJ_TYP"      );
   s_init( & OBJ_ID,       "OBJ_ID"       );
   s_init( & OBJ_LABEL,    "OBJ_LABEL"    );
   s_init( & OBJ_TITLE,    "OBJ_TITLE"    );


                                 /* Find type of object are we working*/
                                 /* with by looking up who the owner  */

   #D_show('Owner OCB type = %d','owner_ocb->type')
   switch( owner_ocb->type )
   {

   case CONFER_OBJ :

      #D_say('Owner is a Conference')
      isv_get( & owner_typ, ISV_CONF, 0 );

      rc = obj_find( owner_ocb, "CONFERENCE.ID"   , & owner_id  );
      rc = obj_find( owner_ocb, "CONFERENCE.LABEL", & owner_lab );
      rc = obj_find( owner_ocb, "CONFERENCE.TITLE", & owner_tit );
      break;

   case GROUP_OBJ :

      #D_say(   'Owner is a Group' );
      isv_get( & owner_typ, ISV_GROUP, 0 );

      rc = obj_find( owner_ocb, "GROUP.ID"   , & owner_id  );
      rc = obj_find( owner_ocb, "GROUP.LABEL", & owner_lab );
      rc = obj_find( owner_ocb, "GROUP.TITLE", & owner_tit );
      break;

   case ACTIVITY_OBJ :

      #D_say(   'Owner is a Activity'       );
      isv_get( & owner_typ, ISV_ACTIVITY, 0 );

      rc = obj_find( owner_ocb, "ACTIVITY.ID", & owner_id );

                                   /* Get ACTIVITY word in correct  */
                                   /* language. They copy the word  */
                                   /* since we are going to say     */
                                   /* ACTIVITY.TITLE because we dont*/
                                   /* titles in activities. We might*/
                                   /* consider getting owners title?*/

      isv_get( & tmpstr2, ISV_TITLE,              0 );
      s_copy(  & owner_tit, tmpstr2,           0, 0 );
      s_cat (  & tmpstr, blank, owner_typ,     0, 0 );
      s_cat (  & owner_tit, tmpstr, owner_tit, 0, 0 );
      break;

   default :

      #D_say(   'Owner is a UNKNOWN')
      isv_get( & owner_typ,  ISV_UNKNOWN,         0 );
      s_cat(   & tmpstr,     owner_typ, blank, 0, 0 );

      isv_get( & tmpstr2 ,  ISV_ID,               0 );
      s_cat(   & owner_id,  tmpstr, tmpstr2,   0, 0 );

      isv_get( & tmpstr2,   ISV_LABEL,            0 );
      s_cat(   & owner_lab, tmpstr, tmpstr2,   0, 0 );

      isv_get( & tmpstr2,   ISV_TITLE,            0 );
      s_cat(   & owner_tit, tmpstr, tmpstr2,   0, 0 );

   }

   #D_str( 'owner_typ', 'Type of owner object'  );
   #D_str( 'owner_lab', 'Label of owner object' );
   #D_str( 'owner_tit', 'Title of owner object' );
   #D_str( 'owner_id' , 'ID of owner object'    );

   var_set( OBJ_TYP,   owner_typ );
   var_set( OBJ_LABEL, owner_lab );
   var_set( OBJ_TITLE, owner_tit );
   var_set( OBJ_ID,    owner_id  );


   while( ( NOT done )  &&  ( rc == NO_ERROR ) )
   {

      end_function = t_form( form_id, S_NULL, memb_ocb );
      #D_show( 'form returned %d',      'end_function' );

      switch( end_function )
      {

         case USER_REQUESTED_PROCESS    :
         case USER_REQUESTED_ENTER      :

            done = TRUE;
            rc   =    0;
            break;

         case USER_REQUESTED_QUIT       :
         case USER_REQUESTED_ABANDON    :
         case FORM_NOT_FORMED           :

            done =         TRUE;
            rc   = end_function;
            break;

         default                        :

            #D_e_show( 'Unhandled request %d.', 'end_function' );
            done = TRUE;
            rc   =    0;
            break;

      }
   }

   var_clr( OBJ_TYP      );
   var_clr( OBJ_LABEL    );
   var_clr( OBJ_TITLE    );
   var_clr( OBJ_ID       );

   s_drop( & blank       );
   s_drop( & tmpstr      );
   s_drop( & form_id     );
   s_drop( & OBJ_TYP     );
   s_drop( & OBJ_ID      );
   s_drop( & OBJ_LABEL   );
   s_drop( & OBJ_TITLE   );

}
if( OWNER_OCB_OPENED_HERE )
{

   obj_cls( owner_ocb );

}
if( MEMB_OCB_OPENED_HERE )
{

   obj_cls( memb_ocb );

}


#D_leave( rc, 'View Membership List ' );

}
