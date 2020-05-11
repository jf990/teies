/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_fend                                                 |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    October 12, 1987                                       |
  |  <LEVEL>   FORMS                                                  |
  |  <GLOBAL>  name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PURPOSE> After a form parse is complete, deallocate everything  |
  |            no longer necessary to have lying around.              |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   obj_cls()                                              |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    01-12-89    JF       Added obj_cls() of the form file. |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "stack.h"
#include "ti.h"
#include "form_i.h"

#define s_clear_str( str )  if(str != S_NULL) { s_drop( &(str) ); }



sint f_fend (form_id, formdata, ocb)

string      form_id  ;            /* Form's ID string                  */
form_dat   *formdata ;            /* Pointer to form's data structure  */
OCB        *ocb      ;


{


   struct   form_process * TM_curr  = NULL;
   struct   f_fmat_s     * f_fmat   = NULL;
   struct   p_prof_s     * p_prof   = NULL;
   struct   f_prof_s     * f_prof   = NULL;
   struct   f_prof_s     * f_profdf = NULL;
   struct   f_gks_s      * f_world  = NULL;
   wind_dat              * body     = NULL;
   tint                    pages    =  0  ;
   sint                    rc       =  0  ;





                                  /* Pointer to global GKS values      */

   f_world = ( struct f_gks_s * )
                  ( (struct form_global *) (TM_master->forms))->f_world ;


   TM_curr = (struct form_process *) TM_master->current->forms ;


   f_fmat   = (struct f_fmat_s *) TM_curr->f_fmat ;
   f_prof   = (struct f_prof_s *) TM_curr->f_prof ;
   p_prof   = (struct p_prof_s *) TM_curr->p_prof ;
   f_profdf = (struct f_prof_s *) TM_curr->f_profdf;
   body     =  p_prof->body   ;
   rc       =  obj_cls( ocb ) ;


                                  /* Save the Form Id to the structure */

   s_copy (&(formdata->form_id), form_id, 0, 0) ;

                                  /* Save the p_prof pointer           */

   formdata->p_prof  = (byte *)p_prof ;

                                  /* Save the INF pointers             */

   formdata->fst_inf = f_fmat->fst_inf ;

                                  /* Close last segment opened         */

   gcls_seg () ;

/* Copy proper info from f_fmat to p_prof
     and drop all strings in the following
 */

   pages = (tint) (body->w_max_y - f_fmat->cur_y) /  \
           (tint) (body->win_len + 1.0) ;
   f_fmat->cur_y = body->w_max_y - body->win_len -   \
                  (body->win_len + 1.0) * (real) pages ;

   body->w_min_y  = p_prof->w_min_y = f_fmat->cur_y ;
   body->w_max_x  = p_prof->w_max_x = f_fmat->org_x + f_fmat->max_rmar ;

   body->last_seg = f_fmat->last_seg ;
   p_prof->w_cur_x= body->w_min_x ;
   p_prof->w_cur_y= body->w_max_y - body->win_len ;

   f_world->last_seg = f_fmat->last_seg ;
   f_world->bot_y    = f_fmat->cur_y    ;


   stk_end ((STACK *)f_fmat->hi_lite ) ;
   stk_end ((STACK *)f_fmat->color_f ) ;
   stk_end ((STACK *)f_fmat->color_b ) ;
   stk_end ((STACK *)f_fmat->font    ) ;
   stk_end ((STACK *)f_fmat->list_stk) ;
   stk_end ((STACK *)f_fmat->if_stack) ;


   os_memd ( f_fmat   ) ;
   TM_curr->f_fmat = NIL;
                                        /* Make sure all strings off the
                                           format profile are dropped  */
   s_clear_str( f_prof->mark_up   );
   s_clear_str( f_prof->open      );
   s_clear_str( f_prof->cls       );
   s_clear_str( f_prof->end       );
   s_clear_str( f_prof->bvar      );
   s_clear_str( f_prof->evar      );
   s_clear_str( f_prof->io_otype  );
   s_clear_str( f_prof->io_id     );
   s_clear_str( f_prof->io_name   );
   s_clear_str( f_prof->io_index  );
   s_clear_str( f_prof->io_prompt );
   s_clear_str( f_prof->io_help   );
   s_clear_str( f_prof->io_def    );
   s_clear_str( f_prof->att_name  );
   s_clear_str( f_prof->att_ref   );
   s_clear_str( f_prof->ctr_brk   );
   s_clear_str( f_prof->h_id      );
   s_clear_str( f_prof->h_title   );
   s_clear_str( f_prof->dl_tsize  );
   s_clear_str( f_prof->dl_hdhi   );
   s_clear_str( f_prof->dl_trmhi  );
   s_clear_str( f_prof->fig_id    );
   s_clear_str( f_prof->fig_plc   );
   s_clear_str( f_prof->fig_frmc  );
   s_clear_str( f_prof->sp_units  );
   s_clear_str( f_prof->rl_list   );
   s_clear_str( f_prof->rl_ref    );
   s_clear_str( f_prof->rl_name   );

   os_memd ( f_prof   ) ;
   TM_curr->f_prof = NIL;
   os_memd ( f_profdf ) ;
   TM_curr->f_profdf = NIL;


   return(rc);
}
