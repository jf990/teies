/************************************************************************
     <DOC>
     <NAME>    gt_ntf(OCB *notif_ocb, string modifier, hword level,
                                                      byte *params);
     <AUTHOR>  TANMAY S. KUMAR
     <DATE>    NOVEMBER 10, 1988
     <LEVEL>   OBJ
     <PARAM>   notif_ocb  *OCB       INPUT   OCB of notif to get
               modifier  string      INPUT   has the key of the notif
               level     hword       INPUT   not used
               params    *byte       OUTPUT  not used
     <PURPOSE> To display the required notification and depending on
               the user input receive it and put in the old notif box
               or still keep it in the new box, or quit viewing it.
     <OUTPUT>  sint
     <ERRORS>  0 if received into old notif box
               28834 user requested quit
               28835 user requested abandon
               20001 Improper object type passed
               20022 Invalib OCB passed
               20224 Function not implemented
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
#include "obj_ntf.h"
#include "obj_mail.h"




sint     gt_ntf( notif_ocb, modifier, level, params )

OCB            * notif_ocb;
string           modifier;   /* has the key to the notification object */
hword            level;
byte           * params;

{



 string   form         = S_NULL,
          mod_copy     = S_NULL,
          label        = S_NULL,
          line         = S_NULL,
          blank        = S_NULL,
          line1        = S_NULL,
          LINE1        = S_NULL,
          associated   = S_NULL,
          prefix       = S_NULL,
          separator    = S_NULL;
 sint     rc           =      0,
          xc           =      0,
          position     =      0;
 OCD    * notif_ocd    =    NIL;
 OCB    * assoc_ocb    =    NIL;
 OID      assoc_oid            ;
 hword    key_pressed  =      0;
 boolean  done_with_get = FALSE,
          ok_to_receive = FALSE;




 if ( OCB_POINTER_OK( notif_ocb ) )
 {

    if ( notif_ocb->type == NOTIF_OBJ )
    {

       if (    ( level == ISV_NEW )  || ( level == VIEW_NTF_NEW )
            || ( level ==       0 ) )
       {

          s_init( & form,    "1170"    );

       }
       else
       {

          s_init( & form,    "1173"    );

       }
       s_init(  & LINE1,    "LINE1"               );
       s_init(  & blank,    " "                   );
       isv_get( & line,     ISV_NOTIF,          1 );
       s_copy(  & line1,    line,            0, 0 );
       s_cat(   & line1,    line1, blank,    0, 0 );
       s_copy(  & mod_copy, modifier,        0, 0 );
       s_trim(  & mod_copy, 'L'                   );
       s_cat(   & line1,    line1, mod_copy, 0, 0 );
       f_setfld( form,      LINE1,          line1 );
       var_set(  LINE1,     line1                 );

       while( ! done_with_get )
       {

          key_pressed = t_form( form, S_NULL, notif_ocb );

          switch( key_pressed )
          {

             case  USER_REQUESTED_PROCESS :
             case  USER_REQUESTED_ENTER   :

                if ( (level == ISV_NEW) || (level == VIEW_NTF_NEW) )
                {

                   ok_to_receive = TRUE;

                }
                done_with_get = TRUE;
                break;

             case  USER_REQUESTED_ABANDON :
             case  USER_REQUESTED_QUIT    :
             case  FORM_NOT_FORMED        :
             case  USER_REQUESTED_PRINT   :

                done_with_get =        TRUE;
                rc            = key_pressed;
                break;

             case  USER_REQUESTED_CONFIRM :

                rc = 0;
                except( FUNCTION_NOT_IMPLEMENTED, USER_WARNING, S_NULL,
                                        S_NULL, S_NULL, S_NULL, S_NULL );

                break;

             case  USER_REQUESTED_SCAN   :

                ocd_new(    & notif_ocd, "NOTIFICATION.ASSOCIATED" );
                rc = obj_get( notif_ocb, notif_ocd, & associated   );
                s_init(     &(notif_ocd->name), "%LINKS.OWNER"     );
                rc = obj_get( notif_ocb, notif_ocd, & assoc_oid    );


                if( (   s_len(        associated ) > 0 )     &&
                    ( ! OID_IS_NULL( & assoc_oid ) ) )
                {

                   rc        = obj_id(  & assoc_oid, & associated );
                   assoc_ocb = obj_opn( & assoc_oid,
                                                 OBJ_READ | OBJ_REMOTE );
                   if ( OCB_POINTER_OK( assoc_ocb ) )
                   {


                      rc = t_jmpst(  assoc_ocb->type, GET_ACT, (hword) 0,
                                            associated, assoc_ocb, NIL );

                      obj_cls( assoc_ocb );

                   }
                }
                else
                {

                   except(    NO_ASSOCIATED_ITEM, USER_ERROR, S_NULL,
                                       S_NULL, S_NULL, S_NULL, S_NULL );

                }
                s_drop ( & associated );
                if (rc == USER_REQUESTED_ABANDON)
                {

                   done_with_get = TRUE;

                }
                else
                {

                   rc = 0;

                }
                break;

             default   :

                break;

          }
       }                                          /* end while loop */
       f_close(  form, NO_FLAGS );

       if (ok_to_receive)
       {

          rc = rc_ntf( notif_ocb, modifier, level, params );

       }

       s_drop( & form           );
       s_drop( & LINE1          );
       s_drop( & line1          );
       s_drop( & label          );
       s_drop( & blank          );
       ocd_drp(& notif_ocd      );

    }
    else
    {

       rc = IMPROPER_OBJECT_TYPE;

    }
 }
 else
 {

    rc = INVALID_OCB;

 }

   return(rc);

}
