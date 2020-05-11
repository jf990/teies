/************************************************************************
    <DOC>
    <NAME>         vw_act(OCB *some_ocb, string  modifier,hword level,
                                                          OID *act_oid)
    <LEVEL>        Activity
    <DATE>         APril 27, 1988    March 1, 1989
    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar
    <PARAM>        some_ocb   OCB *   INPUT  The ocb of the user or the
                                             activity.
                   modifier   string  INPUT
                   level      hword   INPUT  Tells if old or new
                   act_oid    OID *   IN/OUT The oid of the activity
    <OUTPUT>       Sint. See error codes returned.
    <PURPOSE>
    <CALLS>        a_crrd(), a_crqs(), a_crsl(), obj_opn(), Obj_upd(),
                   stosi(), s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), os_memn(), t_form(), isv_match(),
                   var_clr().
    <ERRORS>
                   INVALID_OID                     20030
                   DATA_BASE_INDEX_ANOMALY         20052
                   FUNCTION_NOT_SUPPORTED          20222
                   TM_USER_CONTROL_BLOCK_NOT_FOUND 28007
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
#include "activity.h"

sint vw_act( some_ocb, modifier, level, act_oid )

OCB     *some_ocb;
string  modifier;
hword   level;
OID     *act_oid;

{

 OCB     *act_ocb       =         NIL,
         *atch_ocb      =         NIL;

 OCD     *act_ocd       =         NIL,
         *user_ocd      =         NIL;

 OID     a_oid                      ,
         atch_oid                   ;

 sint    rc             =           0,
         undone_count   =           0;



 if ( ! OCB_POINTER_OK( some_ocb ) )
 {
    some_ocb = TM_master->ucb->directory;
 }
 if ( OCB_POINTER_OK( some_ocb ) )
 {
    switch(some_ocb->type)
    {
      case USER_OBJ :


          if (level == ISV_NEW)
          {
             ocd_new(&user_ocd, "INDEXES.UNDONE_COUNT");
             ocd_new(&act_ocd,  "%LINKS.OWNER");

             rc = obj_get(some_ocb, user_ocd, &undone_count);

             if ((rc == 0) && (undone_count > 0))
             {
                s_init(&user_ocd->name, "INDEXES.UNDONE_ACT.OID");
                user_ocd->bias = 1;

                while (undone_count > 0)
                {
                   rc = obj_get( some_ocb, user_ocd, &a_oid );
                   if( rc != 0 )
                   {
                      rc = DATA_BASE_INDEX_ANOMALY;
                      except(rc, INFO, S_NULL,
                                         S_NULL, S_NULL, S_NULL, S_NULL);
                      undone_count -= 1;
                      continue;
                   }

                   act_ocb = obj_opn(&a_oid, OBJ_UPDATE | OBJ_REMOTE);
                   if( OCB_POINTER_OK( act_ocb ) )
                   {


                      rc = obj_get(act_ocb, act_ocd, &atch_oid);
                      atch_ocb =obj_opn(&atch_oid,OBJ_UPDATE|OBJ_REMOTE);

                      if( OCB_POINTER_OK( atch_ocb ) )
                      {
                         rc = do_act(atch_ocb, modifier, level, NIL);
                         obj_cls(atch_ocb);
                      }

                      obj_cls(act_ocb);

                      if( ( rc == USER_REQUESTED_ABANDON )
                        || ( rc == USER_REQUESTED_QUIT   ) )
                      {
                         break;
                      }

                      undone_count -= 1;
                   }
                   else
                   {

                      rc = INVALID_OID;
                      except(rc, INFO, S_NULL,
                                         S_NULL, S_NULL, S_NULL, S_NULL);

                   }
                }
             }
             ocd_drp( &user_ocd );
             ocd_drp( &act_ocd  );
          }
          break;

      case  ACT_OBJ :

          ocd_new(&act_ocd,  "%LINKS.OWNER");
          rc = obj_get(act_ocb, act_ocd, &atch_oid);
          atch_ocb = obj_opn(&atch_oid,OBJ_UPDATE | OBJ_REMOTE);

          if( OCB_POINTER_OK( atch_ocb ) )
          {
             rc = do_act(atch_ocb, modifier, level, NIL);
             obj_cls(atch_ocb);
          }
          ocd_drp( &act_ocd  );

          break;

      default :

          rc = FUNCTION_NOT_SUPPORTED;
          break;



    }  /* end switch */
 }
 else
 {
    rc = TM_USER_CONTROL_BLOCK_NOT_FOUND;   /* 28007 */
 }

   return(rc);

}
