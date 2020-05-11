/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    rm_ntf(OCB *notif_ocb, string modifier, hword level,   |
  |                                                      byte *params)|
  |  <AUTHOR>  Tanmay S. Kumar                                        |
  |  <DATE>    January 25, 1989                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   notif_ocb  OCB *      INPUT   Not yet used             |
  |            modifier   string     INPUT   number of the notif to   |
  |                                          delete                   |
  |            level      hword      INPUT   Not yet used             |
  |            params     byte *     INPUT   Not yet used             |
  |                                                                   |
  |  <PURPOSE> Delete Notifications from the system.                  |
  |                                                                   |
  |  <OUTPUT>  Always return 0 if delete is sucessful.                |
  |            return error code depending on type of error.          |
  |                                                                   |
  |  <LIMITS>  A notification can be deleted from the system only by  |
  |            the author of the notification.                        |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_prim.h"
#include "exc.h"


#define     NO_AUTHORIZATION                           20059
#define     NUMBER_NOT_PROVIDED                        20060


sint     rm_ntf(notif_ocb, modifier, level, params)

OCB     *notif_ocb;
string    modifier;
hword        level;
byte       *params;


{


sint      rc                 =         0,
          notif_key_length   =         0,
          position           =         0,
          new_count          =         0,
          old_count          =         0;

hword     s_to_oid           = STRING_TO_OID | NO_NEED_TO_XMIT,
          link_type          = RECIPIENT_LINK,
          object_type        =         0;

OCB       *user_ocb          =       NIL;

OCD       *user_ocd          =       NIL,
          *notif_ocd         =       NIL;

string    target             =    S_NULL,
          bad_names          =    S_NULL,
          access             =    S_NULL,
          component          =    S_NULL,
          result_str         =    S_NULL;

OID       *oid               =       NIL,
          oid1                          ,
          oid2                          ;

boolean   flag               =     FALSE;


oid = (OID *) os_mema(sizeof(OID));
os_memn( oid, sizeof(OID));

if ( s_len(modifier) <= 0 )
  {
   except(NUMBER_NOT_PROVIDED, USER_ERROR, S_NULL,S_NULL, S_NULL, S_NULL,
                                                                 S_NULL);
  }
else
  {

   if (TM_master->usrtype == MASTER_SERVER)
     {
      if ( OCB_POINTER_OK(notif_ocb) )
        {
         s_init(&component, "ACCESS");
         rc = obj_glk(notif_ocb, component, &oid, position, link_type,
                                                          &object_type );
         position = 1;
         ocd_new(&user_ocd, "INDEXES.NOTIFICATIONS.OID");

         while (rc == 0)
           {
            user_ocb = obj_opn(oid, OBJ_UPDATE);
            if ( OCB_POINTER_OK(user_ocb) )
            {
               s_init(&access, "ACCESS");
               obj_rlk(notif_ocb, access, &(user_ocb->oid));
               s_init(&user_ocd->name, "INDEXES.NOTIFICATIONS.OID");
               s_alloc(&(user_ocd->key), notif_key_length);
               s_smp_ul(user_ocd->key) = notif_key_length;
               os_memn(s_buffer(user_ocd->key), notif_key_length);
               user_ocd->bias = 1;

               while (rc != NO_DATA_FOUND)
                 {
                  os_memn(&oid1, sizeof(OID));
                  rc = obj_get(user_ocb, user_ocd, &oid1);
                  if ( oid_cmp(notif_ocb->oid, oid1) )
                    {
                     s_init(&user_ocd->name, "INDEXES.NOTIFICATIONS.ID");
                     user_ocd->bias = 0;
                     obj_rmv(user_ocb, user_ocd);
                     flag = TRUE;
                     break;
                    }
                 }
               if (!flag)
                 {
                  flag = FALSE;
                  s_init(&user_ocd->name, "INDEXES.OLD_NOTIF.OID");
                  os_memn(s_buffer(user_ocd->key), notif_key_length);
                  user_ocd->bias = 1;
                  rc = 0;

                  while (rc != NO_DATA_FOUND)
                    {
                     rc = obj_get(user_ocb, user_ocd, &oid1);
                     if ( oid_cmp(notif_ocb->oid, oid1) )
                      {
                       os_memn(&oid1, sizeof(OID));
                       s_init(&user_ocd->name, "INDEXES.OLD_NOTIF.ID");
                       user_ocd->bias = 0;
                       obj_rmv(user_ocb, user_ocd);
                       break;
                      }
                    }
                 }
               obj_cls(user_ocb);
              }
            rc = obj_glk(notif_ocb, component, &oid, position, link_type,
                                                          &object_type );
           }

         user_ocb = t_author();
         if (OCB_POINTER_OK (user_ocb))
           {
            s_init(&user_ocd->name, "INDEXES.OLD_NOTIF.ID");
            s_copy(&(user_ocd->key), modifier, 0, 0);
            rc = obj_rmv(user_ocb, user_ocd);
            obj_cls(user_ocb);
           }

         obj_set_flag(notif_ocb, OBJ_DELETE);
         obj_cls(notif_ocb);

         ocd_drp(&user_ocd);
         ocd_drp(&notif_ocd);
         s_drop(&target);
         s_drop(&bad_names);
        }
      else
        {
        rc = INVALID_OCB;
        }
      t_csend(t_invoker(), rc, result_str);

     } /* end if for master server */
   else
     {
      s_pad(&modifier, modifier, notif_key_length, ' ', 'L');
      ocd_new(&user_ocd, "INDEXES.OLD_NOTIF.OID");

      user_ocb = t_author();
      s_copy(&user_ocd->key, modifier, 0, 0);

      rc = obj_get( user_ocb, user_ocd, &oid );

      if( rc == 0 )
        {
         notif_ocb = obj_opn( oid, OBJ_UPDATE | OBJ_REMOTE);
         ocd_new(&notif_ocd, "%LINKS.AUTHOR");
         rc = obj_get( notif_ocb, notif_ocd, &oid1 );
         s_init(&(notif_ocd->name), "%LINKS.OWNER");
         rc = obj_get( notif_ocb, notif_ocd, &oid2 );
         ocd_drp(&notif_ocd);
         /* right now only if the author and owner are the same - del */

         if ( (oid_cmp( (user_ocb->oid), oid1))    &&
                            (oid_cmp( (user_ocb->oid), oid2)) )
           {

            s_init( &(user_ocd->name), "INDEXES.OLD_NOTIF.ID");
            rc = obj_rmv( user_ocb, user_ocd );

            if (rc == 0)
              {

               rc = t_remote(TM_master->MS, NOTIF_OBJ, REMOVE_ACT, level,
                                notif_ocb,  modifier , &(result_str));
               if (rc == 0)
                 {
                  obj_set_flag(notif_ocb, OBJ_DELETE);
                  obj_cls(notif_ocb);
                  s_trim(&modifier, 'L');
                  except( OBJECT_RECEIVED, USER_INFORMATION, modifier,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
                 }
              }
            if (rc != 0)
              {
               s_trim(&modifier, 'L');
               except(rc , USER_WARNING, modifier,S_NULL, S_NULL, S_NULL,
                                                                S_NULL );
              }

           }
         else
           {
            s_trim(&modifier, 'L');
            except(NO_AUTHORIZATION, USER_WARNING, modifier, S_NULL,
                                                  S_NULL, S_NULL,S_NULL);
           }
        }
      else
        {
         s_trim(&modifier, 'L');
         except( NTF_DOES_NOT_EXIST, USER_ERROR, modifier, S_NULL,
                                               S_NULL, S_NULL, S_NULL );

        }
      ocd_drp( & user_ocd );
     }

  }
os_memd(oid);

   return(rc);

}
