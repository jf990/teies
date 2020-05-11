/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    md_memb                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    December 22, 1988                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   memb_ocb    OCB *      INPUT   Membership or Parent obj|
  |                                            to modify membership of|
  |  <PARAM>   modifier    string     INPUT   Either user names, OIDs |
  |                                            or Owner ID depending  |
  |                                            on what level is set to|
  |  <PARAM>   level       hword      INPUT   Determines how to handle|
  |                                            input arguments and    |
  |                                            what actions to take.  |
  |  <PARAM>   result      string *   OUTPUT                          |
  |                                                                   |
  |  <PURPOSE> Modify Membership Object entries.                      |
  |                                                                   |
  |            If memb_ocb is CR, GRP or ACT then locate the member-  |
  |            ship object related to it and modify that.             |
  |                                                                   |
  |            if memb_ocb is a membership and modifer is given then  |
  |            check level for the following:                         |
  |                                                                   |
  |              MEMB_MODIFIER_IS_OID : The modifier string is an OID |
  |                                     to be added to the membership.|
  |              MEMB_MODIFIER_IS_NAME: The modifier string is a user |
  |                                     name to derive an OID to add  |
  |                                     to the membership.            |
  |                                                                   |
  |            if memb_ocb is NIL or cannot determine a membership and|
  |            modifier string is empty then ask the user for a memb- |
  |            ership owning object to derive a membership from.      |
  |                                                                   |
  |              MEMB_MODIFIER_IS_ID  : The modifier string is the ID |
  |                                     of the owning object (CR, etc)|
  |                                                                   |
  |            If level has the MEMB_OCB_ALREADY_CHECKED bit set then |
  |            owner OCB is pointed to by the params argument and does|
  |            need to be validated.                                  |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "form.h"
#include "exc.h"
#include "isv.h"
#include "obj_memb.h"




sint    md_memb( memb_ocb, modifier, level, params )

OCB            * memb_ocb;
string           modifier;
hword            level;
OCB           ** params;

{

sint       rc                = NO_ERROR,
           bad_itm_num       =        0,
           oid_len           =        0;

hword      key_hit           =        0;

boolean    memb_ocb_opened   =    FALSE,
           done_getting_memb =    FALSE,
           ok_to_search      =    FALSE,
           owner_ocb_opened  =     TRUE;

OID      * memb_oid          =      NIL,
         * owner_oid         =      NIL;

string     bad_item_list     =   S_NULL,
           items             =   S_NULL,
           ITEMS             =   S_NULL,
           state             =   S_NULL;

byte     * ocd_name          =      NIL;

OCB      * owner_ocb         =      NIL;







oid_len = sizeof( OID );

                                  /* If we already went through checks */
                                  /* in calling program, then we don't */
                                  /* have to do it again. Also, caller */
                                  /* will pass owner's OCB in params   */

if( level & MEMB_OCB_ALREADY_CHECKED )
{

   owner_ocb        = * params;
   owner_ocb_opened =    FALSE;

}
else
{

   owner_oid = (OID *) os_mema( oid_len );

   if( OCB_POINTER_OK( memb_ocb ) )
   {

      if( obj_find( memb_ocb, "%LINKS.OWNER", owner_oid ) == NO_ERROR )
      {

         owner_ocb = obj_opn( owner_oid, (OBJ_UPDATE | OBJ_REMOTE) );

      }
      else
      {

         rc = INVALID_OWNER_OCB;  /* We could not find the owner from  */
                                  /* the membership OCB                */
      }
   }
   else
   {
                                  /* - - - - - - - - - - - - - - - - - */
                                  /* If no good then check to see if we*/
                                  /* can find a ocb from modifier str. */
                                  /* Modifier SHOULD be the owner of   */
                                  /* the membership list               */
                                  /* - - - - - - - - - - - - - - - - - */

      if( s_len( modifier ) < 1 )
      {

         s_init( & state, "4090" );
         s_init( & ITEMS, "ITEMS");

         while( ! done_getting_memb )
         {

            level  |= MEMB_DOING_VALIDATION ;
            key_hit = t_form( state, S_NULL, NIL );

            if( t_ifquit( key_hit ) )
            {

               done_getting_memb = TRUE;
               ok_to_search      = FALSE;
               rc                = key_hit;
               break;

            }
            else
            {

               s_copy( & items, var_get( ITEMS ), 0, 0 );
               if( s_len( items ) < 1 )
               {

                  except( rc, USER_ERROR, S_NIL, S_NIL, S_NIL, S_NIL, S_NIL );

               }
               else
               {

                  ok_to_search      = TRUE;
                  done_getting_memb = TRUE;

               }
            }
         }
         f_close(  state, STATE_FORM );
         s_drop( & state             );
         s_drop( & ITEMS             );

      }
      else
      {

         s_copy( & items, modifier, 0, 0 );
         ok_to_search = TRUE;
         level       |= MEMB_DOING_VALIDATION ;

      }
      if( s_len( items ) > 0 )
      {                                /* Get OID first for the item   */

         if( (rc = obj_item( items, OBJ_NEED_ONE_OID, & owner_oid, NIL,
                   & bad_item_list, & bad_itm_num, ANY_OBJ)) == NO_ERROR)
         {

            owner_ocb = obj_opn( owner_oid, (OBJ_UPDATE | OBJ_REMOTE) );
                                 /* Chk to see if that OCB is any good */
            if( OCB_POINTER_OK( owner_ocb ) == FALSE )
            {

               except( MEMBERSHIP_DOES_NOT_EXIST, USER_ERROR,
                         bad_item_list, S_NULL, S_NULL, S_NULL, S_NULL );
               rc = INVALID_OCB;     /* If no GOOD again, just set RC  */

            }
            else
            {
                                       /* - - - - -- - - - - - - - - - */
                                       /* We are assuming membership   */
                                       /* object has MEMBERS element of*/
                                       /* type OID. If in future, we   */
                                       /* decide that MEMBERS should be*/
                                       /* an INDEX type of MEMBERSHIPS */
                                       /* OID's then we have to change */
                                       /* our logic here.              */
                                       /* - - - - -- - - - - - - - - - */

               memb_oid = (OID *) os_mema( oid_len );

               switch( owner_ocb->type )
               {

               case CONFER_OBJ :     ocd_name = "CONFERENCE.MEMBERS";
                                     break;

               case GROUP_OBJ :      ocd_name = "GROUP.MEMBERS";
                                     break;

               case ACTIVITY_OBJ :   ocd_name = "ACTIVITY.MEMBERS";
                                     break;

               default :
                                     rc       = INVALID_OCB;
                                     break;
               }


               if((rc = obj_find(owner_ocb, ocd_name,memb_oid))== NO_ERROR)
               {

                  memb_ocb = obj_opn(memb_oid,(OBJ_UPDATE | OBJ_REMOTE));
                  memb_ocb_opened = TRUE;

                  if( NOT OCB_POINTER_OK(memb_ocb) )
                  {

                     rc = INVALID_OCB;

                  }
               }
               os_memd( memb_oid );     /* Do not have any need for OID*/

            }                           /* if else modifer OCB is good */
            s_drop( & items );

         }
         else
         {

           rc = INVALID_OCB;    /* Passed membership OCB was no GOOD. */

         }
      }  /* if there was something in modifer to process */
      else
      {

         rc = NOTHING_TO_PROCESS;

      }
      if( state != S_NULL )
      {

         f_close(  state, STATE_FORM );
         s_drop( & state );

      }
   }  /* if else passed argument membership object was no GOOD */

   if( (rc == NO_ERROR) && (memb_ocb->type != MEMBERSHIP_OBJ) )
   {

      rc = IMPROPER_OBJECT_TYPE;

   }
   os_memd( owner_oid );       /* Do not have any need for OID      */

}   /* If OCB pointer was already checked by calling routine */

                                  /* At this point we have a valid OCB */
                                  /* Or RC would be some error, which  */
                                  /* case we can't do too much but retu*/
if( rc == NO_ERROR )
{
string   form_id        = S_NULL,

         OBJ_TYP        = S_NULL,
         OBJ_PRE        = S_NULL,
         OBJ_ID         = S_NULL,
         OBJ_LABEL      = S_NULL,
         OBJ_TITLE      = S_NULL,
         ADD_MEMB       = S_NULL,
         MOD_MEMB       = S_NULL,
         REM_MEMB       = S_NULL,
         FUL_NAME       = S_NULL,
         CHANGE_ROLE    = S_NULL,
         ROOT_RANGE     = S_NULL,
         ROLE_ALT       = S_NULL,

         prefix            = S_NULL,
         owner_typ         = S_NULL,
         owner_lab         = S_NULL,
         owner_tit         = S_NULL,
         owner_id          = S_NULL,
         memb_roles        = S_NULL,
         members_to_add    = S_NULL,
         members_to_modify = S_NULL,
         members_to_remove = S_NULL,
         nam_errs          = S_NULL,

         tmpstr         = S_NULL,
         tmpstr2        = S_NULL,
         blank          = S_NULL;

PRIVS    memb_privs;

sint     memb_root      = 0,
         rc_form        = 0;

boolean  not_done             = TRUE,
         something_to_work_on = FALSE;

    s_init( & blank,        " "            );
    s_init( & form_id,      "3190"         );
    s_init( & OBJ_TYP,      "OBJ_TYP"      );
    s_init( & OBJ_PRE,      "OBJ_PRE"      );
    s_init( & OBJ_ID,       "OBJ_ID"       );
    s_init( & OBJ_LABEL,    "OBJ_LABEL"    );
    s_init( & OBJ_TITLE,    "OBJ_TITLE"    );
    s_init( & ADD_MEMB,     "ADD_MEMB"     );
    s_init( & MOD_MEMB,     "MOD_MEMB"     );
    s_init( & REM_MEMB,     "REM_MEMB"     );
    s_init( & FUL_NAME,     "FULL_NAME"    );
    s_init( & CHANGE_ROLE,  "CHANGE_ROLE"  );
    s_init( & ROOT_RANGE,   "ROOT_RANGE"   );
    s_init( & ROLE_ALT,     "ROLE_ALT"     );

                                  /* Find type of object are we working*/
                                  /* with by looking up who the owner  */
      switch( owner_ocb->type )
      {

      case CONFER_OBJ :

         isv_get( & owner_typ, ISV_CONF, 0 );

         obj_find( owner_ocb, "CONFERENCE.ID"   , & owner_id  );
         obj_find( owner_ocb, "CONFERENCE.LABEL", & owner_lab );
         obj_find( owner_ocb, "CONFERENCE.TITLE", & owner_tit );
         break;

      case GROUP_OBJ :

         isv_get( & owner_typ, ISV_GROUP, 0 );

         obj_find( owner_ocb, "GROUP.ID"   , & owner_id  );
         obj_find( owner_ocb, "GROUP.LABEL", & owner_lab );
         obj_find( owner_ocb, "GROUP.TITLE", & owner_tit );
         break;

      case ACTIVITY_OBJ :

         isv_get( & owner_typ, ISV_ACTIVITY, 0 );

                                      /* * * * * * * * * * * * * * * * */
                                      /* Get ACTIVITY word in correct  */
                                      /* language. They copy the word  */
                                      /* since we are going to say     */
                                      /* ACTIVITY.TITLE because we dont*/
                                      /* titles in activities. We might*/
                                      /* consider getting owners title?*/
                                      /* * * * * * * * * * * * * * * * */
         rc = obj_find( owner_ocb, "ACTIVITY.ID"    , & owner_id );
         isv_get( & tmpstr2,   ISV_TITLE,            0 );
         s_copy(  & owner_tit, tmpstr2,           0, 0 );
         s_cat (  & tmpstr,    blank,  owner_typ, 0, 0 );
         s_cat (  & owner_tit, tmpstr, owner_tit, 0, 0 );
         break;

      default :

         isv_get( & owner_typ, ISV_UNKNOWN,        0 );
         s_cat(   & tmpstr, owner_typ, blank,   0, 0 );

         isv_get( & tmpstr2 , ISV_ID,              0 );
         s_cat(   & owner_id, tmpstr, tmpstr2,  0, 0 );

         isv_get( & tmpstr2, ISV_LABEL,            0 );
         s_cat(   & owner_lab, tmpstr, tmpstr2, 0, 0 );

         isv_get( & tmpstr2, ISV_TITLE,            0 );
         s_cat(   & owner_tit, tmpstr, tmpstr2, 0, 0 );
         break;

      }  /* switch(type) */

      s_copy( & prefix, owner_typ, 1, 1 );
      var_set( OBJ_TYP,   owner_typ );
      var_set( OBJ_PRE,   prefix    );
      var_set( OBJ_LABEL, owner_lab );
      var_set( OBJ_TITLE, owner_tit );
      var_set( OBJ_ID,    owner_id  );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 |   HERE GOES THE CODE TO DISPLAY MEMBERS ALREADY IN THE OBJECT,      |
 |   WHEN LISTS START WORKING. Or we can let form level fetch          |
 |   it from the object itself. We could put all members in CONTENT    |
 |   object and then "<attach>" it to the form???                      |
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   while( not_done && (rc == NO_ERROR) )
   {

      rc_form = t_form( form_id, S_NULL, memb_ocb );

      switch( rc_form )
      {
                                  /* Stuff user names in OCB so we can */
                                  /* send it to the master server for  */
                                  /* linking to the user lists         */

      case           USER_REQUESTED_REPLY    :

         level |= MEMB_DOING_LINKING ;

      case           USER_REQUESTED_PROCESS  :
      case           USER_REQUESTED_ENTER    :

         members_to_add = var_get( ADD_MEMB );
         if( s_len(members_to_add) )
         {

            obj_update(memb_ocb, "TMP.ADDMEMB", members_to_add);
            something_to_work_on = TRUE;

         }

         members_to_modify = var_get( MOD_MEMB );
         if( s_len( members_to_modify ) )
         {

            obj_update(memb_ocb, "TMP.MODMEMB", members_to_modify);
            something_to_work_on = TRUE;

         }

         members_to_remove = var_get( REM_MEMB );
         if( s_len( members_to_remove ) )
         {

            obj_update(memb_ocb, "TMP.REMMEMB", members_to_remove);
            something_to_work_on = TRUE;

         }

         if( ( something_to_work_on    == TRUE                 )
         ||  ( rc_form                 == USER_REQUESTED_REPLY ) )
         {

            something_to_work_on = FALSE;
            s_stosi(  & memb_root, var_get( ROOT_RANGE ), 0, 0);
            obj_update( memb_ocb, "TMP.ROOT",    & memb_root          );

            memb_roles = var_get( ROLE_ALT );  /* Get member roles     */
            os_memc( s_buffer(memb_roles), &memb_privs, sizeof(PRIVS) );
            obj_update( memb_ocb, "TMP.ROLES",   &  memb_privs        );
            obj_update( memb_ocb, "TMP.OWNID" ,     owner_id          );
            obj_update( memb_ocb, "TMP.OWNOID",  & (owner_ocb->oid)   );
            obj_update( memb_ocb, "TMP.OWNTYPE", & (owner_ocb->type)  );

            rc = lk_memb( memb_ocb, S_NULL, level, & nam_errs );

            if((rc == NO_ERROR) && (rc_form == USER_REQUESTED_REPLY))
            {

                not_done = FALSE;

            }
            if( rc == NAME_NOT_FOUND )
            {

               except( NAME_NOT_FOUND, USER_ERROR, nam_errs, S_NULL,
                                            S_NULL,  S_NULL, S_NULL );
               rc = NO_ERROR;

            }
            else if ( rc != NO_ERROR )
            {

               except( rc, CONTROL_ERROR, S_NULL, S_NULL, S_NULL,
                                                  S_NULL, S_NULL );
               not_done = FALSE;

            }
            else
            {

               level |= MEMB_USE_EXISTING_LISTS;

                                          /* we use existing lists so  */
                                          /* users already added don't */
                                          /* dis-appear                */
            }
         }                       /* if( something_to_work_on == TRUE ) */
         var_clr( ADD_MEMB      );
         var_clr( MOD_MEMB      );
         var_clr( REM_MEMB      );
         var_clr( ROOT_RANGE    );
         var_clr( ROLE_ALT      );

         f_close( form_id, STATE_FORM );
         break;

      case   USER_REQUESTED_QUIT    :
      case   USER_REQUESTED_ABANDON :

         rc       = rc_form;
         not_done =   FALSE;
         break;

      default                       :

         break;

      }  /* Switch to handle each key pressed */
   }  /*  while there is something to display */

   var_clr( ADD_MEMB      );
   var_clr( MOD_MEMB      );
   var_clr( REM_MEMB      );
   var_clr( ROOT_RANGE    );
   var_clr( ROLE_ALT      );

   f_close( form_id, STATE_FORM );

   s_drop( & blank        );
   s_drop( & tmpstr       );
   s_drop( & form_id      );
   s_drop( & OBJ_TYP      );
   s_drop( & OBJ_ID       );
   s_drop( & OBJ_LABEL    );
   s_drop( & OBJ_TITLE    );
   s_drop( & ADD_MEMB     );
   s_drop( & MOD_MEMB     );
   s_drop( & REM_MEMB     );
   s_drop( & FUL_NAME     );
   s_drop( & CHANGE_ROLE  );
   s_drop( & ROOT_RANGE   );

   s_drop( & owner_lab    );
   s_drop( & owner_tit    );
   s_drop( & owner_id     );

   if( (rc == NO_ERROR) && ((level & MEMB_USE_EXISTING_LISTS) == 0) )
   {

      rc = OBJECT_NOT_LINKED;

   }

   if( (level & MEMB_OCB_ALREADY_CHECKED) == 0 )
   {                             /* Basicall if caller has passed OCB */
                                 /* in arguement then we should not   */
                                 /* touch them!                       */
      if( owner_ocb_opened )
      {

         obj_cls( owner_ocb );

      }

      if( memb_ocb_opened )
      {

         obj_cls( memb_ocb  );

      }
   }
}


   return(rc);

}
