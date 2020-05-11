/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    f_rstfld.c                                             |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <DATE>    February 29, 1988                                      |*/
/*|                                                                   |*/
/*|  <LEVEL>   FORMS.FFUN                                             |*/
/*|  <PARAM>   form_id    string         INPUT  Form to use.          |*/
/*|  <PARAM>   flags      sint           INPUT  Form type, etc.       |*/
/*|  <PARAM>   field_id   string         INPUT  Field in that form.   |*/
/*|                                                                   |*/
/*|  <PURPOSE> Reset a field on a form to its original contents - that|*/
/*|            is - the contents of that field derived at parsing time|*/
/*|                                                                   |*/
/*|  <OUTPUT>  TRUE if the field is found and reset.                  |*/
/*|            FALSE if the field is not on the form or error.        |*/
/*|                                                                   |*/
/*|  <CALLS>   stk_get(), gs_high(), gs_scolor(), os_memd(), gcls_seg)|*/
/*|            s_copy(), f_render(), gcreat_seg()                     |*/
/*|                                                                   |*/
/*|  <MODS>    19_May_88   J.L.Foster     Added return value of true  |*/
/*|                                        if the field is reset.     |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "debug.h"
#include "form_i.h"




sint f_rstfld( form_id, flags, field_id )
sint                    flags ;
string         form_id, field_id;

{

   sint         rc            =   0 ,
                field_counter =   0 ;
   form_dat    *fcb           =  NIL;
   INF         *current_field =  NIL;
   string       last_field    =S_NUL;
   boolean      found         =FALSE;



   s_trim(     &form_id, 'B'           );
   s_upper(     form_id,  0,  0        );
   s_trim(     &field_id, 'B'          );
   s_upper(     field_id,  0,  0       );


   fcb = frm_inta( form_id, flags );
   if( fcb != NIL && ( fcb != FORM_NOT_ACTIVE ) )
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if( s_comp( current_field->ocd->name, field_id, 0, 0,'M',&rc ) )
         {

            current_field = current_field->next;

         }
         else
         {


            s_copy( &(current_field->contents), current_field->init_val,
                                                0, 0 );
            s_copy( &(current_field->prv_cont), current_field->init_val,
                                                0, 0 );


            found = TRUE;
            break;

         }
      }
   }


   return(found);
}
