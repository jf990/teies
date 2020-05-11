/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    p_disp                                                 |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    September 29, 1987                                     |
  |  <LEVEL>   FORMS.PM                                               |
  |  <GLOBAL>  TM_master->f_npmfun sint   INPUT  Number of elements   |
  |                                              in PF CMD structure  |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_pmfun  byte * INPUT  Pointer to start of  |
  |                                              PF CMD structure     |
  |                                                                   |
  |  <PARAM>  *formdata          form_dat IN/OUT Pointer to PM's pro- |
  |                                              file & INF's.        |
  |                                                                   |
  |  <PARAM>   flags             sint     IN     Bit mask with flags  |
  |                                              for forms control.   |
  |                                                                   |
  |  <PURPOSE> External interface to the Presentation Manager         |
  |  <OUTPUT>  Returns a pointer to the value of the last PFkey hit.  |
  |  <CALLS>   p_beg, p_end, p_home, p_top, p_bot, p_left, p_right,   |
  |            p_prev, p_next, p_hprev, p_hnext, p_quit, grq_mstr,    |
  |            gclr_vi, g_redraw                                      |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "tm.h"
#include "ti.h"
#include "form_i.h"



hword  p_disp (formdata, flags)

form_dat  *formdata ;             /* Struct containing p_prof & inf's  */
sint       flags    ;             /* Bit Mask containing control flags */


{
   sint             count   =     -1 ;/* Counter for s_comp loop       */
   sint             funkey  =     -1 ;/* Number of key struck          */
   sint             pf_ncmd =      0 ;/* Number of elements in CMD     */
   sint             rc      =      0 ;/* Return code from s_comp       */
   sint             flag    =      0 ;/* Flag returned from s_comp     */
   hword            pf_val  =      0 ;/* Value  of key struck          */
   struct form_global * f_global =  NIL ;
   struct exec_pfk *pf_cmd  =   NULL ;/* Ptr to PF keys CMD structure  */
   struct p_prof_s *p_prof  =   NULL ;/* Ptr to PM's profile           */
   boolean          help_requested = FALSE ;
   wind_dat        *body    =   NULL ;/* Window info on the body       */
   wind_dat        *top_mat =   NULL ;/* Window info on top matter     */
   wind_dat        *bot_mat =   NULL ;/* Window info on bottom matter  */
   string           error_prefix = S_NULL,
                    error_id= S_NULL;




  #D_start (p_disp, PM, 'formdata=0x%X flags=0x%X','formdata, flags')

                                  /* Get neccesary info from TM_master */

   f_global = (struct form_global *) TM_master->forms ;
   pf_cmd   = (struct exec_pfk *) f_global->pm_fun    ;
   pf_ncmd  =                     f_global->num_pm_fun;
   p_prof   = (struct p_prof_s *) formdata->p_prof    ;
   body     =                     p_prof->body        ;
   top_mat  =                     p_prof->top_mat     ;
   bot_mat  =                     p_prof->bot_mat     ;


   #D_str  ('formdata->form_id','Form ID')
   #D_show ('p_prof=0x%X fst_inf=0x%X','p_prof,formdata->fst_inf') ;

   #D_pprofs ('p_prof', 'PMs profile');



   #D_wnddat ('top_mat','Top    Matter') ;
   #D_wnddat ('body   ','Body of Text ') ;
   #D_wnddat ('bot_mat','Bottom Matter') ;
                                  /* Set funkey to invalid value       */
   funkey = -1 ;
                                  /* Set up whatever is neccessary     */
   p_beg  (formdata->form_id, p_prof) ;
                                  /* Show first screen of the form     */
   p_show_p(p_prof) ;
                                  /* Sound alarm if requested to       */
   if (CHK_MASK(flags, SOUND_ALARM) || p_prof->alarm)
      g_alarm (p_prof->wk_id) ;

   do
   {
                                  /* Request input for fields and wait */
                                  /* for a command key PF/PA/ENTER     */

      grq_mstr( p_prof->wk_id, & funkey );

      #D_show ('key hit from GKS = %d','funkey') ;

                                  /*===================================*/
                                  /* Convert GKS key number to FORMS   */
                                  /* control function. First, see if a */
                                  /* form redefined a key in the p_prof*/
                                  /* table.  If it is 0 there then grab*/
                                  /* it from the global table.         */
                                  /*===================================*/

      if( ( pf_val = p_prof->pf_key(: funkey :) ) == 0 )
      {

         pf_val = ((struct form_global *) TM_master->forms)->pf_key(:funkey:);

      }
      #D_show( 'Function Value is %d','pf_val' );

      if( pf_val == (hword) FORM_HELP )
      {

         if( TM_master->current->except != 0 )
         {

            s_sitos( & error_id,     TM_master->current->except   );
            s_init(  & error_prefix, "E"                          );
            s_cat(   & error_id,     error_prefix, error_id, 0, 0 );
            s_copy(  & ( p_prof->help_dat ),       error_id, 0, 0 );
            s_drop(  & error_id     );
            s_drop(  & error_prefix );

         }
         else
         {

            s_copy( & ( p_prof->help_dat ), formdata->form_id, 0, 0 );

         }
         p_help( formdata, p_prof );
         help_requested = TRUE ;

      }
      else
      {
                                  /* Check to see if help was requestd */
                                  /*    on any of the input fields and */
                                  /*    process it if it was           */

         help_requested = p_qhelp (  formdata, p_prof, flags  ) ;

                                  /* If help was not requested         */
                                  /*    check to see if the function   */
                                  /*    requested is intended for the  */
                                  /*    Presentation Manager.  If so,  */
                                  /*    execute that function here and */
                                  /*    show the form again.           */
      }
      if ( ! help_requested  &&  ! p_exepm( pf_val, p_prof ) )
      {

                                  /* Else if key has a definition then */
                                  /*    end forms and return value     */
         p_prof->leave = TRUE ;

      }
      else
      {
                                  /* Need to redraw the viewport of the*/
                                  /* form after showing the help form  */
                                  /* or executing a PM function.       */
                                  /* Set viewport of body              */
         gs_view (body->trans_nm,                 \
                  body->v_min_x  , body->v_max_x, \
                  body->v_min_y  , body->v_max_y) ;

                                  /* Set viewport and window of topmat */
         if (top_mat != NULL)
         {
            gs_view (top_mat->trans_nm,                    \
                     top_mat->v_min_x  , top_mat->v_max_x, \
                     top_mat->v_min_y  , top_mat->v_max_y) ;
            gs_wind (top_mat->trans_nm,                    \
                     top_mat->w_min_x  , top_mat->w_max_x, \
                     top_mat->w_min_y  , top_mat->w_max_y) ;
         }
                                  /* Set viewport and window of botmat */
         if (bot_mat != NULL)
         {
            gs_view (bot_mat->trans_nm,                    \
                     bot_mat->v_min_x  , bot_mat->v_max_x, \
                     bot_mat->v_min_y  , bot_mat->v_max_y) ;
            gs_wind (bot_mat->trans_nm,                   \
                     bot_mat->w_min_x  , bot_mat->w_max_x, \
                     bot_mat->w_min_y  , bot_mat->w_max_y) ;
         }
                                  /* Redraw screen                     */
         p_show_p (p_prof) ;

      }
   }
   while (  ! p_prof->leave  ) ;
                                  /* Clean up whatever is neccessary   */
   p_end (    p_prof  ) ;
                                  /* Return value of last key hit      */
   #D_return( 'pf_val' ) ;
}
