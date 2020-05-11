/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    fld_drp.c                                              |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <LEVEL>   FORMS.FFUN                                             |*/
/*|  <DATE>    September 26, 1989                                     |*/
/*|  <PARAM>   id       string     INPUT   String containing the field|*/
/*|                                        id of a new field to add to|*/
/*|                                        the list of fields.        |*/
/*|                                                                   |*/
/*|  <PURPOSE> Remove an individual field.                            |*/
/*|  <OUTPUT>  sint                                                   |*/
/*|  <CALLS>              os_memd(), s_copy(), s_drop()               |*/
/*|  <MODS>                                                           |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "vars.h"
#include "ti.h"
#include "form_i.h"

sint fld_drp(      id, stem    )

string             id;
sint               stem;

{

   struct f_fmat_s * f_fmat = NIL;
   INF             * current= NIL;




   f_fmat = (struct f_fmat_s *)
           ((struct form_process *) TM_master->current->forms)->f_fmat;

   if( (current = fld_fnd( id, stem )) == NIL  )
   {


   }
   else
   {


      fld_clr( current );

      if( current->next == NIL && current->prev == NIL )
      {

         f_fmat->fst_inf = NIL;

      }
      else
      {

         if( current->prev == NIL )
         {

            current->next->prev = NIL;
            f_fmat->fst_inf     = current;

         }
         else if ( current->next == NIL )
         {

            current->prev->next = NIL;

         }
         else
         {

            current->prev->next = current->next;
            current->next->prev = current->prev;

         }
      }
      os_memd( current )    ;

   }


   return(0);
}
