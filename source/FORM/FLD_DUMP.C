/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    fld_dump.C                                             |*/
/*|  <LEVEL>   FORMS.FFUN                                             |*/
/*|  <DATE>    December 05, 1987                                      |*/
/*|  <PURPOSE> Dump out all the current fields.                       |*/
/*|  <OUTPUT>  0      sint                                            |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "form_i.h"


sint fld_dump(  )


{

   INF     *field    =  NIL   ;        /* Pointer for checking INF's   */
   sint     rc       =      0 ;
   struct  f_fmat_s  *f_fmat = NIL;


if( TM_master->current != NIL )
{

   f_fmat = (struct f_fmat_s *)
           ((struct form_process *)TM_master->current->forms)->f_fmat;

   if(( field = f_fmat->fst_inf ) != NIL)
   {

      while ( field != NIL )
      {

          field = field->next;

      }
   }
}
   return(0);
}
