/************************************************************************
     <DOC>
     <NAME>    ntf_memb
     <AUTHOR>  Tanmay S. Kumar
     <DATE>    January 10, 1988
     <LEVEL>   OBJ
     <PARAM>   memb_ocb     OCB *     INPUT  Membership OCB
     <PARAM>   notification string    INPUT  What to tell each member
     <PARAM>   author_oid   OID *     OUTPUT author OID is sent back
     <PARAM>   ocb          OCB **    OUTPUT Notif OCB is sent back
     <PURPOSE> Associate the new notification with a membership object.
               the membership is the first parameter and the message to
               send each member of the membership is in modifier.
     <OUTPUT>  sint
     </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "bse.h"
#include "obj_prim.h"



sint ntf_memb(memb_ocb, notification, author_oid, ocb)

OCB              *memb_ocb;
string           notification;
OID              *author_oid;
OCB              **ocb;

{

 sint          bse_value    =             0,
               type         = AUTO_MEMB_NTF,  /* 52 */
               *line_num    =           NIL;

 byte          *current_ptr = NIL;

 OCB           *notif_ocb   = NIL,
               *author_ocb  = NIL;

 OCD           *notif_ocd   = NIL;

 string        message      = S_NULL,
               author       = S_NULL,
               by           = S_NULL,
               curpointer   = S_NULL,
               recipients   = S_NULL;

 datetime                         dt;




 notif_ocb = obj_all(NOTIF_OBJ, LOCAL_OBJECT, NIL);

 /* make the owner of the Notification object as the memb object itself*/

 ocd_new( & notif_ocd, "%LINKS.OWNER");
 obj_rep(   notif_ocb, notif_ocd, &(memb_ocb->oid), SEG_UPDATE);

 s_init( & notif_ocd->name, "NOTIFICATION.TO");
 current_ptr = s_buffer( notification    );
 curpointer  = (string) current_ptr   ;
 s_copy( & message, curpointer, 0, 0 );

 current_ptr += s_smp_ul( curpointer ) + 4;
 curpointer   = (string) current_ptr;

 s_copy( & recipients, curpointer, 0, 0 );

 obj_rep( notif_ocb, notif_ocd, recipients, SEG_UPDATE );

 s_init( & notif_ocd->name,     "NOTIFICATION.TYPE" );
 obj_rep(  notif_ocb, notif_ocd, & type, SEG_UPDATE );
 author_ocb = t_author( );


 if ( OCB_POINTER_OK( author_ocb ) )
 {
    gt_name(     author_ocb, S_NULL, FULL_NAME , & author );
    os_memc( & ( author_ocb->oid ),  author_oid, sizeof( OID ) );
    obj_cls(     author_ocb            );
 }
 else
 {
    s_init( & author, "(TEIES Phantom User)" );
    author_ocb = NIL;
 }
 s_init(&notif_ocd->name, "NOTIFICATION.AUTHOR");
 obj_rep(notif_ocb, notif_ocd, author, SEG_UPDATE);

 s_init( &notif_ocd->name, "%SYSTEM.CREATE_DATE");
 os_dttm(  & dt  );
 obj_rep(notif_ocb, notif_ocd, &dt, SEG_UPDATE);

 s_init(&notif_ocd->name, "NOTIFICATION.CONTENTS.TEXT");
 s_alloc(&(notif_ocd->key), BSE_MAX_KEY_SIZE);
 os_memn(s_buffer(notif_ocd->key), BSE_MAX_KEY_SIZE);
 s_smp_ul(notif_ocd->key) = BSE_MAX_KEY_SIZE;
 line_num = (sint *) s_buffer(notif_ocd->key);
 bse_value = B_RFIRST | B_PRINTABLE;
 *(line_num) = 1;
 obj_rep(notif_ocb, notif_ocd, message, SEG_UPDATE);

 s_init( &(notif_ocd->name), "NOTIFICATION.CONTENTS.BSE");
 obj_rep(notif_ocb, notif_ocd, &bse_value, SEG_UPDATE);

if (ocb != NIL)
{
   *ocb = notif_ocb;
}
else
{
   obj_cls( notif_ocb );
}

s_drop( & message   );
s_drop( & author    );
s_drop( & recipients);
ocd_drp(& notif_ocd );

   return(0);

}
