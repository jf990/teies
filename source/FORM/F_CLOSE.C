/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_close.c                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 9, 1987                                       |
  |  <LEVEL>   FORMS                                                  |
  |  <GLOBAL>  None.                                                  |
  |  <PARAM>   form_id   string   INPUT  Form ID of the form to       |
  |                                      remove from the world and    |
  |                                      list of parsed forms.        |
  |                                                                   |
  |  <PARAM>   flags      hword    INPUT                              |
  |  <PURPOSE> Take a form out of the world and release associated    |
  |            memory (GKS segments, profiles, structures).           |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   frm_inta(), s_drop(), os_memd(), frm_drp()             |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"
#include "ti.h"



sint f_close (form_id, flags)

string form_id ;
hword  flags   ;


{
   struct p_prof_s *p_prof   = NULL ;
   form_dat        *formdata = NULL ;
   sint             count    =    0 ,
                    key_number =  0 ,
                    rc       =    0 ,
                    segment  =    0 ;
   INF             *current  = NULL ;



formdata = frm_inta (form_id, flags) ;

if (formdata == NIL || formdata == (form_dat *) -1 )
{


}
else
{

   p_prof =  (struct p_prof_s *) formdata->p_prof    ;



                                  /* Remove all GKS segments used by   */
                                  /* this form.                        */

   if( p_prof->top_mat != NIL )
   {

      for( segment =  p_prof->top_mat->frst_seg;
           segment <= p_prof->top_mat->last_seg;
           segment ++                              )
      {

         gdelet_seg( segment );

      }
   }
   if( p_prof->bot_mat != NIL )
   {

      for( segment =  p_prof->bot_mat->frst_seg;
           segment <= p_prof->bot_mat->last_seg;
           segment ++                              )
      {

         gdelet_seg( segment );

      }
   }
   if( p_prof->body != NIL )
   {

      for( segment =  p_prof->body->frst_seg;
           segment <= p_prof->body->last_seg;
           segment ++                              )
      {

         gdelet_seg( segment );

      }
   }

                                  /* Drop all the fields used by this  */
                                  /* form.                             */

   while ( (current = formdata->fst_inf) != NIL && rc == 0 )
   {

      rc = fld_clr( current );

      if( current->next == NIL && current->prev == NIL )
      {

         formdata->fst_inf = NIL;

      }
      else
      {

         if( current->prev == NIL )
         {

            current->next->prev = NIL;
            formdata->fst_inf   = current->next;

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

                                  /* Drop structures p_prof points to  */

   os_memd  ( p_prof->top_mat )   ;
   os_memd  ( p_prof->body    )   ;
   os_memd  ( p_prof->bot_mat )   ;

                                  /* Drop Str containing help data     */

   s_drop ( & (p_prof->help_dat))  ;

                                  /* Drop function key strings if the  */
                                  /* Form redefined any.               */

   for( key_number = 0; key_number < F_NUM_REDEFINABLE_FUNKEYS; key_number ++ )
   {

      if( p_prof->pf_val[ key_number ] != S_NULL )
      {

         s_drop( & (p_prof->pf_val[ key_number ]) );

      }
   }

                                  /* Drop p_prof structure now that    */
                                  /* everything it points to has been  */
                                  /* deallocated.                      */

   os_memd  (  p_prof          )   ;
   formdata->p_prof = NIL ;


                                  /* Remove formdata from parsed table */
   frm_drp (form_id, flags) ;

}


   return(0);
}
