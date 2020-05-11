/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_author()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 14, 1988                                      |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   * none *                                               |
  |                                                                   |
  |  <PURPOSE> Get the object control block of the current user,      |
  |            considered as the author of the object we are          |
  |            currently working on.                                  |
  |                                                                   |
  |            This routine is considered an OPEN of the user object  |
  |            and the open counter will be incremented.  Therefore   |
  |            a subsequent CLOSE of the object is required.          |
  |                                                                   |
  |  <OUTPUT>  OCB *                                                  |
  |  <CALLS>   t_invoker(), t_conn(), s_len()                         |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |            4-14-89    JF      Added OPEN_COUNT increment and      |
  |                               object pointer check for local serv.|
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_prim.h"
#include "exc.h"
#include "tm.h"




OCB    * t_author()


{

   string     invoker          = S_NULL;
   OCB       *author_ocb       =    NIL;
   ISCF_TAB  *conn_table_entry =    NIL;






   if( TM_master->usrtype == MASTER_SERVER )
   {

      invoker = t_invoker();
      if( s_len( invoker ) > 0 )
      {

         conn_table_entry = t_conn(  invoker  );
         if( conn_table_entry != NIL )
         {

            author_ocb = obj_opn(
                            & ( conn_table_entry->objectID ) , OBJ_UPDATE
                             );
         }
         else
         {

            author_ocb = NIL;

         }
      }
   }
   else
   {

      author_ocb = TM_master->ucb->directory;
      if( OCB_POINTER_OK( author_ocb ) )
      {

         author_ocb->open_count ++ ;

      }
   }


   return(author_ocb);

}
