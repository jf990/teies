/************************************************************************
     <DOC>
     <NAME>    vw_ntf(OCB *some_ocb, string modifier, hword level,
                                                       byte *params);
     <AUTHOR>  Tanmay S. Kumar
     <DATE>    October 26, 1988
     <LEVEL>   OBJ
     <PARAM>   some_ocb    OCB *      INPUT  OCB of user or Notif to
                                             view
     <PARAM>   modifier    string     INPUT  may have the id of the
                                             notification to view
     <PARAM>   level       hword      INPUT  type of notification to
                                             view i.e NEW or OLD
     <PARAM>   params      byte *     INPUT  not used

     <PURPOSE> View Notifications OLD and NEW.

               If a bad OCB is passed the current users OCB is taken.

               If Level is 160(ISV_NEW) then NEW notif's are Viewed
               If Level is 161(ISV_OLD) then OLD notif's are Viewed

               If Modifier is not S_NULL then it is a string from the
               user that must be parsed containing a list of notif
               item identifiers to be verified into a list for pro-
               cessing.

               The input object can be 3 possible types:

               Notification - then show the single piece of notif
                   List - the list contains Notif items.  Items other
                          than notif will not be processed.
                   User - Process Notf associated to this user by the
                          level assignments above.

               NOTE: View calls Scan, Abstract or Get based on the
               user's set preferance.  The default is Get to show the
               whole notf item one at a time.  Scan and abstract show
               a list format where the user can choose which items to
               perform actions on.

     <OUTPUT>  0 if sucessful
               or error code depending upon error. Error codes are
               defined in obj_errs.h and tm_errs.h
     <CALLS>   gt_ntf()
     <LIMITS>  Right now only one item# can be passed in modifier
     </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "exc.h"
#include "isv.h"
#include "obj_prim.h"




sint     vw_ntf( some_ocb, modifier, level, params  )

OCB              *some_ocb;
string                      modifier;
hword                                 level;
byte                                        *params;


{

tint    notif_key_len   =      4;

sint    i               =      0,
        rc              =      0,
        num_bad         =      0,
        item_num        =      0,
        counter         =      0;

OCB     *notif_ocb      =    NIL,
        *list_ocb       =    NIL;

OCD     *user_ocd       =    NIL;

OID     oid,
        list_oid,
        * oid_ptr       =    NIL;

string  TOT_NOTIF       = S_NULL,
        CURRENT_NOTIF   = S_NULL,
        state           = S_NULL,
        key             = S_NULL,
        key1            = S_NULL,
        holder          = S_NULL,
        bad_items       = S_NULL,
        temp            = S_NULL;




s_init(&TOT_NOTIF, "TOT_NOTIF");
s_init(&CURRENT_NOTIF, "CURRENT_NOTIF");

if( ((some_ocb == NIL) && (level == 1) && (s_len( modifier ) > 0))  ||
    ((some_ocb == NIL) && (level == 2) && (s_len( modifier ) > 0))  ||
    ((some_ocb == NIL) && (level == 0) && (s_len( modifier ) > 0))    )
{


    OCB     *list_ocb       =                NIL;
    OCD     *list_ocd       =                NIL;
    hword    what_to_get    =  OBJ_NEED_MANY_OID;
    sint     num_of_items   =                  0;

    ocd_new(&list_ocd, "LIST.SIZE");
    list_ocb = obj_all(LIST_OBJ, LOCAL_OBJECT, NIL);

    oid_ptr = & oid;
    rc = obj_item(modifier, what_to_get, &oid_ptr, &list_ocb, &bad_items,
                                                    &num_bad, NOTIF_OBJ);
    rc = obj_get(list_ocb, list_ocd, &num_of_items);

    if ( (rc == 0) && (num_of_items > 0) )
    {
       s_sitos( & holder,   num_of_items );
       var_set( TOT_NOTIF, holder );

       for (i=1; i<= num_of_items; i++)
       {
          s_sitos(&holder, i);
          var_set( CURRENT_NOTIF,  holder );
          s_init(&list_ocd->name, "LIST.LIST.OID");
          list_ocd->bias = 1;
          obj_get(list_ocb, list_ocd, &oid);

          s_init(&list_ocd->name, "LIST.LIST.NUMBER");
          list_ocd->bias = 0;
          rc = obj_get(list_ocb, list_ocd, &item_num);

          s_sitos(&key, item_num);
          s_pad( & key1, key, notif_key_len, ' ', 'L');
          some_ocb = TM_master->ucb->directory;
          if (level == VIEW_NTF_NEW)
          {
             ocd_new(&user_ocd, "INDEXES.NOTIFICATIONS.OID");
          }
          else if (level == VIEW_NTF_OLD)
          {
             ocd_new(&user_ocd, "INDEXES.OLD_NOTIF.OID");
          }
          if ( (level == VIEW_NTF_NEW) || (level == VIEW_NTF_OLD) )
          {
             s_copy(&(user_ocd->key), key1, 0, 0);
             rc = obj_get(some_ocb, user_ocd, &oid);
          }
          if (rc == 0)
          {
             notif_ocb = obj_opn( &oid, OBJ_UPDATE | OBJ_REMOTE);

             rc = gt_ntf(notif_ocb, key1, level, params);

             obj_cls(notif_ocb);

             if( ( rc == USER_REQUESTED_ABANDON ) ||
                                 ( rc == USER_REQUESTED_QUIT ) )
             {
                break;
             }
          }
          else
          {
             rc =  NTF_DOES_NOT_EXIST;
             except(rc, USER_INFO, key, S_NULL, S_NULL, S_NULL, S_NULL);
          }
       }
       s_drop(&holder);
    }
    else if (num_of_items <= 0)
    {
       rc =  NTF_DOES_NOT_EXIST;
       except(rc, USER_INFO, bad_items, S_NULL, S_NULL, S_NULL, S_NULL);
    }
}
else
{
   if ( ! OCB_POINTER_OK( some_ocb ) )
   {
      some_ocb = TM_master->ucb->directory;
   }

   if ( OCB_POINTER_OK( some_ocb ) )
   {
      switch(some_ocb->type)
      {

      case USER_OBJ :


          ocd_new( & user_ocd, NIL );

          if ( (  level == ISV_NEW)
           ||  ( (level == VIEW_NTF_NEW) && (s_len(modifier) <= 0) )
           ||  ( (level == 0) && (s_len(modifier) <= 0) ) )
          {
             s_init( & state,  "1170" );
             s_init( & (user_ocd->name), "INDEXES.NOTIFICATIONS");
          }
          else
          {
             s_init( & state,  "1173" );
             s_init( & (user_ocd->name), "INDEXES.OLD_NOTIF");
          }

          rc = obj_get( some_ocb, user_ocd, & list_oid );
          if( rc == 0 && ! OID_IS_NULL( (& list_oid) ) )
          {

             list_ocb = obj_opn( & list_oid, OBJ_UPDATE | OBJ_REMOTE );
             if( OCB_POINTER_OK( list_ocb ) )
             {

                rc = gt_list( list_ocb, modifier, level, params );

             }
             else
             {

                rc = INDEX_IS_EMPTY;

             }
          }
          else
          {

             rc = INDEX_IS_EMPTY;

          }
          s_drop(  & state    );
          ocd_drp( & user_ocd );
          obj_cls(   list_ocb );
          break;


         case  NOTIF_OBJ :

          s_copy(&key, modifier, 0, 0);
          rc = gt_ntf(some_ocb, modifier, level, params);
          break;



         default :

          rc = IMPROPER_OBJECT_TYPE;
          break;

      }
   }
   else
   {
      rc = TM_USER_CONTROL_BLOCK_NOT_FOUND;   /* 28007 */
   }
}
s_drop(&TOT_NOTIF);
s_drop(&CURRENT_NOTIF);
s_drop(&key);

   return(rc);
}

