/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    fld_fnd.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :DATE.    December 05, 1987                                      |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   id      string            INPUT  String indicating     |*/
/*|                                             field id to locate the|*/
/*|                                             field element of.     |*/
/*|                                                                   |*/
/*|  :PURPOSE. Locate a field element in the list of active fields    |*/
/*|            by comparing field->id string elements.                |*/
/*|                                                                   |*/
/*|  :OUTPUT.  field  INF *                                           |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.                                                           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "form_i.h"


INF *fld_fnd( id, stem )

string        id;
sint         stem;

{

   INF     * field              =  NIL   ;
   sint      rc                 =      0 ;
   struct    f_fmat_s  * f_fmat =  NIL   ;
   string    tmp                =  S_NULL;




s_upper( id,  0,  0  );

f_fmat = (struct f_fmat_s *)
        ((struct form_process *)TM_master->current->forms)->f_fmat;

if( f_fmat != NIL )
{

   if(( field = f_fmat->fst_inf ) != NIL)
   {
      fld_stem (&tmp, id, stem );
      while ( field != NIL )
      {

         if ( 0 == s_comp(  field->id,  tmp,  0,  0,  'M', &rc)  )
         {

            break;

         }
         field = field->next;
      }
      s_drop ( &tmp );
   }
}
else
{


}


   return(field);
}
