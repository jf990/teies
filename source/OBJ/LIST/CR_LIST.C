/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     CR_LIST C                                              |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <Author>   Kevin J. Walsh                                         |*/
/*| <Date>     4 Nov. 1988                                            |*/
/*| <PARAM>    list_ocb   OCB*       IN   Optional existing OCB to be |*/
/*|                                          used.                    |*/
/*| <PARAM>    modifer    string     IN   Not used.                   |*/
/*| <PARAM>    level      hword      IN   Not used.                   |*/
/*| <PARAM>    Parms      *          I/O  Pointer to data, used to    |*/
/*|                                          return OCB;              |*/
/*| <PURPOSE>  Starts or restarts (flush then start) a LIST.          |*/
/*| <OUTPUT>   sint         rcode; 0 for success or error RC.         |*/
/*| <CALLS>    ocd_new(), obj_rep(), obj_all(), obj_get()             |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "obj_prim.h"

sint    cr_list( list_ocb, modifier, level, params )

OCB             *list_ocb;             /* Target list (if defined)     */
string           modifier;             /* Not used.                    */
hword            level;                /* Not used.                    */
OCB            **params;               /* Returns new OCB (if INITIng) */
                                       /* and/or contains incoming OCB */
{

   sint      rc       = 0    ;         /* Presumed RC                  */
   OCD       *list_ocd= NIL  ;         /* OCD of list object (for add) */




   if ( params == NIL || params == (OCB**) -1)
   {

      rc = INVALID_DATA_POINTER;
   }
   else
   {
      if ( OCB_POINTER_OK(list_ocb) )  /* Pointer to existing OCB?     */
      {

         ocd_new(&list_ocd,"LIST.LIST");

         rc = obj_rmv(list_ocb,list_ocd);    /* Clear existing OCB     */

         ocd_drp(&list_ocd);

      }
      else                             /* Need a new list OCB          */
      {
         /*+----------------------------------------------------------+*/
         /*| Not an existing OCB--allocate one.                       |*/
         /*+----------------------------------------------------------+*/

         list_ocb = obj_all( LIST_OBJ, LOCAL_OBJECT, NIL );


         if( ! OCB_POINTER_OK( list_ocb ) )
         {

            rc = CANNOT_CREATE_OBJECT;

         }
      }
      *params = list_ocb;

   }

   return(rc);
}
