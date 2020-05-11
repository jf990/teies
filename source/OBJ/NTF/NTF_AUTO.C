/************************************************************************
     <DOC>
     <NAME>    ntf_auto
     <AUTHOR>  Tanmay S. Kumar
     <DATE>    November 16, 1988
     <LEVEL>   OBJ
     <PARAM>   notif_ocb   OCB *      INPUT  Notif obj   control block
     <PARAM>   packet      string     INPUT  Packet of OCD's that are
                                              modified for reiterative
                                              calls from cr_notif().
     <PARAM>   level       hword      INPUT  State level modifier

     <PURPOSE>

     <OUTPUT>  sint
     </DOC>
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "isv.h"
#include "obj_prim.h"
#include "bse.h"


sint ntf_auto(ocb, list_ocb, author_oid, n_ocb, num_of_recip, modifier)

OCB              *  ocb         ,
                 *  list_ocb    ,
                 ** n_ocb       ;
OID              *  author_oid  ;
sint             *  num_of_recip;
string              modifier    ;

{

 sint   rc                =             0,
        bse_value         =             0,
       *line_num          =           NIL,
        type              =             0,
        amount            =             1;

 string author            = S_NULL,
        label             = S_NULL,
        key               = S_NULL,
        recipient         = S_NULL,
        received          = S_NULL,
        asso_id           = S_NULL;

 OCB    *notif_ocb        =    NIL,
        *recipient_ocb    =    NIL,
        *author_ocb       =    NIL;

 OCD    *notif_ocd        =    NIL;

 OID                 recipient_oid;

 datetime                       dt;





/* NEED TO PUT IN THE LINKS AND REVERSE LINKS FOR THE OCB LATER */



 rc = obj_find(ocb, "%LINKS.AUTHOR", &recipient_oid);
                                                  /*********************/
                                                  /* the author of the */
                                                  /* object is now the */
                                                  /* recipient         */
                                                  /*********************/
 s_alloc( & key,    sizeof( OID ) );
 s_smp_ul(  key ) = sizeof( OID )  ;
 os_memc( & recipient_oid, s_buffer( key ), sizeof( OID ) );
 rc = md_list( list_ocb, key, OBJ_LIST_ADD, & recipient_oid );

 recipient_ocb = obj_opn( & recipient_oid, OBJ_UPDATE );
 rc = gt_name( recipient_ocb, S_NULL, NICKNAME_ONLY, & recipient);
 obj_cls(      recipient_ocb );


 notif_ocb = obj_all( NOTIF_OBJ, LOCAL_OBJECT, NIL );

 /* make the owner of the Notification object as the atch object itself*/

 ocd_new( & notif_ocd, "%LINKS.OWNER" );
 obj_rep(   notif_ocb, notif_ocd, & (ocb->oid), SEG_UPDATE );

 s_init( & notif_ocd->name, "NOTIFICATION.TO" );
 obj_rep(  notif_ocb, notif_ocd, recipient, SEG_UPDATE );


 if (ocb->type == MAIL_OBJ)
 {
    type = AUTO_MAIL_NTF;
    isv_get(& label, ISV_MMSG, 1 );
    rc = obj_find(ocb, "MESSAGE.ID" , & asso_id );
    s_trim( & asso_id, 'B' );
    s_cat(  & asso_id, label, asso_id, 0, 0 );

 }
 else if ( ocb->type == ACT_OBJ )
 {
    type = AUTO_ACT_NTF;
    rc = obj_find( ocb, "%LINKS.OWNER", & recipient_oid );
    recipient_ocb = obj_opn( & recipient_oid, OBJ_UPDATE );
    if( OCB_POINTER_OK( recipient_ocb ) )
    {

       if ( recipient_ocb->type == MAIL_OBJ )
       {
          isv_get( & label, ISV_MMSG, 1 );
          rc = obj_find( recipient_ocb, "MESSAGE.ID" , & asso_id );
          s_trim( & asso_id, 'B' );
          s_cat(  & asso_id, label, asso_id, 0, 0 );
       }
       else if ( recipient_ocb->type == COMMENT_OBJ )
       {
          isv_get( & label, ISV_CC, 1 );
          rc = obj_find( recipient_ocb, "COMMENT.ID" , & asso_id );
          s_trim(& asso_id, 'B' );
          s_cat( & asso_id, label, asso_id, 0, 0 );
       }
       obj_cls( recipient_ocb );

    }
 }
 /* at some point we will have to store only the id and reslove the */
 /* 'M' by storing the associated type                              */

 s_init(  & notif_ocd->name, "NOTIFICATION.TYPE"       );
 obj_rep(   notif_ocb, notif_ocd, & type, SEG_HOLD     );

 s_init(  & notif_ocd->name, "NOTIFICATION.ASSOCIATED" );
 obj_rep(   notif_ocb, notif_ocd, asso_id, SEG_UPDATE  );

 author_ocb = t_author();

 if (  OCB_POINTER_OK( author_ocb )  )
 {

    gt_name(     author_ocb, S_NULL, FULL_NAME , & author    );
    os_memc( & ( author_ocb->oid ),  author_oid, sizeof(OID) );
    obj_cls(     author_ocb            );

 }
 else
 {

    s_init( & author, "(TEIES Phantom User)" );
    author_ocb = NIL;

 }
 s_init( & notif_ocd->name, "NOTIFICATION.AUTHOR"   );
 obj_rep(  notif_ocb, notif_ocd, author, SEG_UPDATE );
 s_init( & notif_ocd->name, "%SYSTEM.CREATE_DATE"   );
 os_dttm(  & dt  );
 obj_rep( notif_ocb, notif_ocd, & dt, SEG_UPDATE );

 if (ocb->type == MAIL_OBJ)
 {

    s_init(& received, " Received " );
    s_cat( & received,   received, asso_id, 0, 0 );

    s_cat( & received, author, received,    0, 0 );

 }
 else if (ocb->type == ACT_OBJ)
 {

    s_cat( & received, author, modifier, 0, 0 );

 }
 s_init(  & notif_ocd->name, "NOTIFICATION.CONTENTS.TEXT" );
 s_alloc( &(notif_ocd->key),  BSE_MAX_KEY_SIZE );
 os_memn(   s_buffer(notif_ocd->key), BSE_MAX_KEY_SIZE );
 s_smp_ul(  notif_ocd->key) = BSE_MAX_KEY_SIZE;
 line_num  = (sint *) s_buffer(notif_ocd->key);
 bse_value = B_RFIRST | B_PRINTABLE;
 *(line_num) = 1;
 obj_rep( notif_ocb, notif_ocd, received, SEG_UPDATE );

 s_init( &(notif_ocd->name), "NOTIFICATION.CONTENTS.BSE" );
 obj_rep(  notif_ocb, notif_ocd, & bse_value, SEG_UPDATE );

 if (n_ocb != NIL)
 {

    * n_ocb = notif_ocb;

 }
 else
 {

    obj_cls( notif_ocb );

 }
 if (num_of_recip != NIL )
 {

    * num_of_recip = amount;

 }

 s_drop( & author    );
 s_drop( & key       );
 s_drop( & recipient );
 s_drop( & received  );
 s_drop( & asso_id   );
 ocd_drp(& notif_ocd );

   return(rc);

}
