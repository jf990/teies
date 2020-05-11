/************************************************************************
     <DOC>
     <NAME>    ntf_valto
     <AUTHOR>  Tanmay S. Kumar
     <DATE>    November 16, 1988
     <LEVEL>   OBJ
     <PARAM>   notif_ocb   OCB *      INPUT  Notif obj   control block
     <PARAM>   list_ocb    OCB *      INPUT  Packet of OCD's that are
                                              modified for reiterative
                                              calls from cr_notif().
     <PARAM>   author_oid  OID *      OUTPUT Place to store Author OID.
     <PARAM>   num_of_recip sint *    OUTPUT How many names on TO list.
     <PARAM>   result_str  string *   INOUT  Pointer to a string where
                                              error messages and other
                                              communicated data will
                                              go.

     <PURPOSE> Format the notifcation attribute NOTIFICATION.TO and store
               it in the notif_ocb.  The names are taken out of a list
               object that ex_user() fills in.

               Set the notification create date to the current date and
               time (%SYSTEM.CREATE_DATE).

               Return the oid of the author or the real user considered
               to be the author of this notification.

     <OUTPUT>  sint
     <CALLS>   ex_user(), gt_name()
     </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "obj_prim.h"
#include "bse.h"



sint ntf_valto(notif_ocb, list_ocb, author_oid, num_of_recip, result_str)

OCB              *  notif_ocb;
OCB              *  list_ocb;
OID              *  author_oid;
sint             *  num_of_recip;
string           *  result_str;

{

 sint    rc                 =        0,
         bse_value          =        0,
        *line_num           =      NIL;

 string bad_names           =   S_NULL,
        comma               =   S_NULL,
        target              =   S_NULL,
        receiver            =   S_NULL,
        recipients          =   S_NULL;

 OCD   *notif_ocd           =      NIL,
       *list_ocd            =      NIL;

 OCB   *author_ocb          =      NIL,
       *user_ocb            =      NIL;

 hword  s_to_oid            = STRING_TO_OID | NO_NEED_TO_XMIT ;

 OID                          name_oid;

 datetime                           dt;


 ocd_new( & notif_ocd, "NOTIFICATION.TO"   );
 obj_get(   notif_ocb,  notif_ocd, &target );


 rc = ex_user( list_ocb, target, s_to_oid , & bad_names );

 if (rc == 0)
 {
    ocd_new( & list_ocd, "LIST.LIST.OID" );

    list_ocd->bias = 1;
    s_init( & comma, ", " );
    rc = obj_get( list_ocb, list_ocd, & name_oid );

    while ( rc != NO_DATA_FOUND )
    {

       user_ocb = obj_opn( & name_oid, OBJ_UPDATE );
       gt_name( user_ocb, S_NULL, NICKNAME_ONLY, & receiver );
       s_cat( & receiver,   receiver,   comma,    0, 0 );
       s_cat( & recipients, recipients, receiver, 0, 0 );
       *num_of_recip += 1;
       rc = obj_get( list_ocb, list_ocd, & name_oid );
       obj_cls( user_ocb );

    }                     /* end while getting user name(s) loop */
    ocd_drp( & list_ocd );
    if (rc == NO_DATA_FOUND)
    {
       rc = 0;
    }
 }                        /* end if rc == 0 */

 if ( rc != 0 )
 {

    if ( result_str != NIL )
    {
       s_copy( result_str, bad_names, 0, 0 );
    }
   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    |
    |  There were errors the user server must resolve.  Send back
    |  what we know and cancel this task.  We will keep the object
    |  on this server unless the user server signs off without
    |  resolving the problem.  Then we will have the temporary
    |  object handy if the user server decides to send us a correct
    |  version.  Note they will only have to send any corrected OCDs
    |  and we can start the whole process over but avoid the
    |  resending of the whole object.
    |
    + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 }
 else
 {

    s_smp_ul(recipients) = s_len(recipients) - s_len(comma);
    obj_rep( notif_ocb, notif_ocd, recipients, SEG_UPDATE );

    author_ocb = t_author();

    if (  OCB_POINTER_OK( author_ocb )  )
    {
       os_memc( &( author_ocb->oid ),  author_oid, sizeof(OID));
       rc = obj_cls( author_ocb );
    }
    else
    {
       author_ocb = NIL;
    }


    s_init(   & notif_ocd->name, "%SYSTEM.CREATE_DATE");
    os_dttm(  & dt  );
    obj_rep(    notif_ocb, notif_ocd, & dt, SEG_UPDATE );

 }

 ocd_drp( & notif_ocd  );
 s_drop(  & target     );
 s_drop(  & comma      );
 s_drop(  & recipients );
 s_drop(  & receiver   );
 s_drop(  & bad_names  );

   return(rc);
}
