/************************************************************************

    <DOC>

    <NAME>        a_crpt( string sp_loc, OCB *act_ocb, tint part_num,
                                          OCB **part_ocb, hword act_type)

    <LEVEL>       Activity

    <DATE>        July 15, 1988     March 1, 1989

    <AUTHOR>      Heidi E. Harting  Tanmay S. Kumar

    <GLOBAL>      None.

    <PARAM>       sp_loc     string  INPUT  Location of section sp
                  act_ocb    OCB *   INPUT  The ocb of the activity
                  part_num   tint    INPUT  section or response number

    <OUTPUT>      Sint  0

    <PURPOSE>     The purpose of this routine is to input and store the
                  information regarding a part for an activity.  A Read
                  Activity uses this routine to create the Read Sections,
                  a Question Activity to add a Response, and a Selection
                  Activity to create a Selection Explanation.

    <CALLS>       s_init(), t_spad(), obj_opn(), obj_all(), ocd_new(),
                  ocd_bse(), obj_cls(), obj_rep(), os_memc(), s_copy(),
                  s_drop(), ocd_drp(), s_buffer(), var_get().

    <ERRORS>      None.

    <LIMITS>      None.

    <MODS>        March 1, Rewritten routine to specifications.

    <END>

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
#include "isv.h"

sint a_crpt( sp_loc, act_ocb, part_num, part_ocb, act_type)

string  sp_loc;
OCB     *act_ocb;
tint    part_num;
OCB     **part_ocb;
hword   act_type;

{
   OCB      *sp_ocb     =     NIL,
            *cc_ocb     =     NIL;

   OCD      *cc_ocd     =     NIL,
            *act_ocd    =     NIL;
   string   title       =  S_NULL,
            keys        =  S_NULL,
            response_id =  S_NULL,
            end_function=  S_NULL;



   s_init( &title, "TITLE" );
   s_init( &keys, "KEYS" );
   sp_ocb = obj_opn( (OID *) sp_loc, OBJ_READ | OS_FILE );

   cc_ocb = obj_all( CONFCOM_OBJ, LOCAL_OBJECT, NIL );/* conf comment */

   ocd_new( &cc_ocd, "COMMENT.CONTENTS" );
   ocd_bse( cc_ocb, cc_ocd, sp_ocb );
   obj_cls( sp_ocb );

   s_init( &(cc_ocd->name), "COMMENT.ACT_TYPE" );
   obj_rep( cc_ocb,  cc_ocd, &act_type, SEG_UPDATE );
   s_init( &(cc_ocd->name), "COMMENT.TITLE" );
   obj_rep( cc_ocb,  cc_ocd, var_get(title), SEG_UPDATE );
   s_init( &(cc_ocd->name), "COMMENT.KEYS" );
   obj_rep( cc_ocb,  cc_ocd, var_get(keys), SEG_UPDATE );

   if  ( (act_type == ISV_READ) || (act_type == ISV_SELECTIO) )
   {
       ocd_new( &act_ocd, "ACTIVITY.PART_LIST.TITLE" );
       s_init( &(act_ocd->key), "    " );
       os_memc( &part_num, s_buffer( act_ocd->key ), 4 );
       obj_rep( act_ocb, act_ocd, var_get(title), SEG_UPDATE );

       s_init( &(act_ocd->name), "ACTIVITY.PART_LIST.LOCATION" );
       obj_rep( act_ocb, act_ocd, &(cc_ocb->oid), SEG_UPDATE );

       s_init( &(act_ocd->name), "ACTIVITY.PART_COUNT");
       obj_rep( act_ocb, act_ocd, &part_num, SEG_UPDATE);

       ocd_drp(&act_ocd);
   }

   s_init( &(cc_ocd->name), "COMMENT.ACTIVITY");
   obj_rep( cc_ocb,  cc_ocd, &(act_ocb->oid), SEG_UPDATE );
   s_init( &(cc_ocd->name), "COMMENT.AUTHOR" );
   obj_rep(cc_ocb, cc_ocd, TM_master->ucb->name, SEG_UPDATE);
   s_init( &(cc_ocd->name), "%LINKS.OWNER" );
   obj_rep(cc_ocb, cc_ocd, &(act_ocb->oid), SEG_UPDATE);


   if (part_ocb != NIL)
   {
      *part_ocb = cc_ocb;
    }
   var_clr(title);
   s_drop( &title);
   var_clr(keys);
   s_drop( &keys);

   s_drop( &end_function);
   s_drop( &response_id);

   ocd_drp( &cc_ocd );

   return(0);
}
