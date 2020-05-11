/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     RM_LIST C                                              |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <PARAM>    list_ocb   OCB*       IN   Should be used if a remote  |*/
/*|                                       list is to be delted.       |*/
/*|                                                                   |*/
/*| <PARAM>    modifer    string     IN   Remote server (if a remote  |*/
/*|                                       list is to be deleted )     |*/
/*|                                                                   |*/
/*| <PARAM>    level      hword      IN   OBJ_DELETE_LOCAL_LIST  or 0 |*/
/*|                                       OBJ_DELETE_REMOTE_LIST or 1 |*/
/*|                                                                   |*/
/*| <PARAM>    Parms      OCB**      I/O  List to be removed.         |*/
/*|                                                                   |*/
/*| <PURPOSE>  Delete the indicated list.                             |*/
/*|                                                                   |*/
/*| <OUTPUT>   Returns sint rcode; 0 for success or error RC.         |*/
/*|                                                                   |*/
/*| <CALLS>    ocd_new(), obj_rep(), obj_all(), obj_get()             |*/
/*| <MODS>     07_Dec_88   K.J.Walsh   Implemented.                   |*/
/*|            10_Oct_89   Rana Ajaz   Added logic to handle deletion |*/
/*|                                    of remote lists as well.       |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_prim.h"


sint    rm_list( list_ocb, modifier, level, params )

OCB             *list_ocb;   /* Used if list object is remote          */
string           modifier;   /* Server for t_remote()                  */
hword            level;      /* OBJ_DELETE_LOCAL_LIST                  */
                             /*      OR                                */
                             /* OBJ_DELETE_REMOTE_LIST                 */
OCB            **params;     /* Used if * OCB to be removed is local   */

{

   sint      rc       = 0  ;         /* Presumed RC                  */

   OCB      *ocb      = NIL;

   string    dummy_str =  S_NULL;




   if( ( level == OBJ_DELETE_LOCAL_LIST  ) ||
       ( level == OBJ_DELETE_REMOTE_CALL )    )
   {

      if( OCB_POINTER_OK ( list_ocb ) )
      {

         ocb = list_ocb;

      }
      else if( params != NIL )
      {

         if( OCB_POINTER_OK ( *params ) )
         {

            ocb = *params;

         }
         else
         {

            rc = INVALID_OCB;

         }

      }
      else
      {

         rc = INVALID_OCB;

      }

      if( OCB_POINTER_OK ( ocb ) )
      {

         obj_set_flag( ocb, OBJ_DELETE   );
         rc = obj_cls( ocb );

         if( ( rc == 0 ) && ( params != NIL && params != (OCB **) -1 ) )
         {

               *params = NIL;

         }

      }  /* if ocb pointer ok */

      if( level == OBJ_DELETE_REMOTE_CALL )
      {

         t_csend( t_invoker(), rc, dummy_str );

      }

   }
   else if( level == OBJ_DELETE_REMOTE_LIST )
   {

      if( s_len( modifier ) < 1 )
      {

         modifier = TM_master->MS;

      }

      rc = t_remote( modifier, LIST_OBJ, REMOVE_ACT,
                     OBJ_DELETE_REMOTE_CALL, list_ocb, S_NULL, NIL );
      if( rc != 0 )
      {

         except( rc, USER_INFO, S_NULL, S_NULL, S_NULL,
                                        S_NULL, S_NULL );

      }

   }
   else if ( level == OBJ_LIST_DEL_ENTRY )
   {

      rc = md_list( list_ocb, modifier, level, NIL );

   }
   else
   {

      rc = FUNCTION_NOT_SUPPORTED;

   }

   return(rc);
}
