/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_profil.c                                             |
  |  <AUTHOR>  John L. Foster, Nestor Voronka                         |
  |  <DATE>    October 23, 1987                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  TM_master->current, TM_master->forms.                  |
  |  <PARAM>   string      form_id   Form ID of current form          |
  |  <PARAM>   OCB        *form_ocb  OCB of object containing the form|
  |  <PARAM>   OCD        *form_ocd  OCD of records to parse          |
  |  <PARAM>   OCB        *obj_ocb   OCB of object referenced by form |
  |  <PARAM>   hword       flags     Control flags from form          |
  |  <PURPOSE> Initialize the forms processor.                        |
  |  <OUTPUT>  Always return 0.                                       |
  |  <CALLS>   os_mema(), f_getorg(), s_init ()                       |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "stack.h"
#include "ti.h"
#include "form_i.h"


form_dat * f_profil ( form_id, form_ocb, form_ocd, obj_ocb, flags )

string                form_id  ;
OCB                 * form_ocb ;
OCD                 * form_ocd ;
OCB                 * obj_ocb  ;
hword                 flags    ;


{

   struct form_process * f_global = NIL;
   struct f_fmat_s *f_fmat    = NULL ;
   struct f_prof_s *f_prof    = NULL ;
   struct f_prof_s *f_default = NULL ;
   struct p_prof_s *p_prof    = NULL ;
   wind_dat        *winddata  = NULL ;
   form_dat        *formdata  = NULL ;

   sint             rc        =  0   ;
   sint             wk_id     =  1   ;
   sint             du        =  0   ;
   real             xras      =  0.0 ;
   real             yras      =  0.0 ;
   real             wid       =  0.0 ;
   real             len       =  0.0 ;
   tint             count     =  0   ;
   boolean          tmp_bool  = TRUE ;




                                 /* = = = = = = = = = = = = = = = = = */
                                 /* Get the TM Process forms pointer  */
                                 /* to store all global data for a    */
                                 /* given form.                       */
                                 /* = = = = = = = = = = = = = = = = = */

f_global = (struct form_process *) TM_master->current->forms ;

if( f_global == NIL  ||  f_global == (struct form_process *) -1 )
{

   f_global = (struct form_process *)
               os_mema( sizeof( struct form_process ) ) ;
   os_memn(   f_global, sizeof( struct form_process ) ) ;
   TM_master->current->forms = (byte *) f_global ;

}
                                 /* = = = = = = = = = = = = = = = = = */
                                 /* Allocate Forms Format structure.  */
                                 /* = = = = = = = = = = = = = = = = = */

   f_global->f_fmat = (byte *) os_mema( sizeof(struct f_fmat_s) );
   f_fmat           = ( struct f_fmat_s * ) f_global->f_fmat;
   os_memn    ( f_fmat,                sizeof (struct f_fmat_s) );

                                 /* = = = = = = = = = = = = = = = = = */
                                 /* Allocate Forms Profile structure. */
                                 /* = = = = = = = = = = = = = = = = = */

   f_global->f_prof = (byte *) os_mema( sizeof(struct f_prof_s));
   f_prof           = (struct f_prof_s *)   f_global->f_prof;
   os_memn ( f_prof,                   sizeof (struct f_prof_s));

                                 /* = = = = = = = = = = = = = = = = = */
                                 /* Allocate Forms Profile Default    */
                                 /* structure, containing default data*/
                                 /* for all markups and the forms.    */
                                 /* = = = = = = = = = = = = = = = = = */

   f_global->f_profdf = (byte *) os_mema(sizeof(struct f_prof_s));
   f_default          = (struct f_prof_s *)  f_global->f_profdf;
   os_memn ( f_default,                 sizeof (struct f_prof_s));

                                 /* = = = = = = = = = = = = = = = = = */
                                 /* Allocate the Presentation Mgr.    */
                                 /* profile structure to maintain data*/
                                 /* for displaying a form.            */
                                 /* = = = = = = = = = = = = = = = = = */

   f_global->p_prof   = (byte *) os_mema( sizeof(struct p_prof_s));
   p_prof             = (struct p_prof_s *)   f_global->p_prof;
   os_memn ( p_prof,                     sizeof (struct p_prof_s));

                                 /* = = = = = = = = = = = = = = = = = */
                                 /* Allocate the GKS Window data inter*/
                                 /* face structure that is placed in  */
                                 /* the Forms Profile (f_prof).       */
                                 /* = = = = = = = = = = = = = = = = = */

   winddata = (wind_dat *) os_mema(sizeof( wind_dat ));
   os_memn(     winddata,          sizeof( wind_dat ));


                                  /* Add form to list of parsed forms  */
   formdata = frm_add (form_id, flags) ;
                                  /* Fields<-->OBJ level link          */
   f_fmat->ocb      =         obj_ocb  ;
                                  /* Get GKS form location values      */
   f_getorg (&(f_fmat->org_x), &(f_fmat->org_y), &(f_fmat->last_seg)) ;

   f_fmat->cur_x    = f_fmat->org_x    ;
   f_fmat->cur_y    = f_fmat->org_y    ;
   f_fmat->ftype    =        FLD_NONE  ;
   f_fmat->echo     =            ECHO  ;

   rc = gqmax_disp (wk_id, &du, &wid, &len, &xras, &yras) ;

   if( rc != 0 && rc != 15000 )
   {

      except( rc, FATAL_ERROR, form_id,S_NULL, S_NULL, S_NULL, S_NULL );

   }
   else
   {

      f_fmat->lef_mar  =             0.0  ;
      f_fmat->rig_mar  =         wid-1.0  ;
      f_fmat->max_rmar = f_fmat->rig_mar  ;

      f_fmat->spacing  =             1    ;
      f_fmat->just     =          L_JUST  ;
      f_fmat->hi_lite  = (sint *)stk_ini (MAX_ATT_STACK, sizeof (sint)) ;
      f_fmat->color_f  = (sint *)stk_ini (MAX_ATT_STACK, sizeof (sint)) ;
      f_fmat->color_b  = (sint *)stk_ini (MAX_ATT_STACK, sizeof (sint)) ;
      f_fmat->font     = (sint *)stk_ini (MAX_ATT_STACK, sizeof (sint)) ;
      f_fmat->if_stack = (sint *)stk_ini (MAX_ATT_STACK,sizeof(boolean));
        stk_psh ((STACK *) f_fmat->if_stack , (byte *) &tmp_bool);


      f_fmat->list_stk = (byte *)
                        stk_ini(MAX_LIST_LEVELS, sizeof(struct f_lst_s));

      f_fmat->status   =           START  ;


                                  /* Set up Presentation Manager's     */
                                  /* with proper default values        */
      p_prof->wk_id    =   wk_id                         ;

      p_prof->scrn_wid = wid-1.0                         ;
      p_prof->scrn_len = len-1.0                         ;

      p_prof->w_min_x  = f_fmat->org_x                   ;
      p_prof->w_max_x  = f_fmat->org_x + f_fmat->rig_mar ;
      p_prof->w_min_y  = f_fmat->org_y                   ;
      p_prof->w_max_y  = f_fmat->org_y                   ;

      p_prof->w_cur_x  =       0.0                       ;
      p_prof->w_cur_y  =       0.0                       ;

      p_prof->body     = winddata                        ;

      winddata->trans_nm =  1   ;
      winddata->frst_seg =  f_fmat->last_seg ;
      winddata->last_seg =  f_fmat->last_seg ;
      winddata->win_wid  =  wid - 1.0 ;
      winddata->win_len  =  len - 1.0 ;
      winddata->w_min_x  =  p_prof->w_min_x ;
      winddata->w_max_x  =  p_prof->w_max_x ;
      winddata->w_min_y  =  p_prof->w_min_y ;
      winddata->w_max_y  =  p_prof->w_max_y ;
      winddata->v_min_x  =  0.0 ;
      winddata->v_max_x  =  1.0 ;
      winddata->v_min_y  =  0.0 ;
      winddata->v_max_y  =  1.0 ;

      p_prof->leave    =    FALSE ;
      p_prof->alarm    =    FALSE ;

      s_copy ( & ( p_prof->help_dat ), form_id, 0, 0) ;

                                 /**************************************/
                                 /* Define the hard-coded defaults for */
                                 /* all the qualifiers.  Later the pro-*/
                                 /* file parser will find what values  */
                                 /* the user changed any of these to,  */
                                 /* and we overwrite these values with */
                                 /* those specified from the profile.  */
                                 /* = = = = = = = = = = = = = = = = =  */

      f_default->io_var   =   FALSE ;/* field - variable               */
      f_default->io_io    =  FLD_OUTPUT;/* field-IN, REP or OUT(deflt) */
      f_default->io_echo  =  ECHO   ;/* field-  NOECHO / ECHO  (deflt) */
      f_default->io_pad   =    ' '  ;/* field - pad                    */
      f_default->io_just  =  L_JUST ;/* field - justify                */

      f_default->h_level  =      2  ;/* h - (a number)                 */

      f_default->hp_level =      1  ;/* hp - (a number)                */
      f_default->hp_color =      1  ;/* hp - color   (Blue)            */

      f_default->fig_wdth =     65  ;/* fig - width                    */

      f_default->sp_amt   =      1  ;/* space - amount                 */

      f_default->all_prse =  TRUE   ;/* enable parsing (non-remark)    */
      f_default->mkp_prse =  TRUE   ;/* must start parsing markups     */
      f_default->var_prse =  TRUE   ;/* must start parsing variables   */

      f_default->fn_on    =  FALSE  ;/* fn - footnote on / off         */

      f_default->note_on  =  FALSE  ;/* note - note on/off             */

                                 /*Now copy the default into the work  */
                                 /*struct for use by the routines.     */

      os_memc( f_default, f_prof, sizeof( struct  f_prof_s  ) );

                           /* Initialization of Tags */
      s_init(&(f_prof->open),"<");           /* opening markup tag     */
      s_init(&(f_prof->cls ),">");           /* closing markup tag     */
      s_init(&(f_prof->end ),"/");           /* end markup prefix      */
      s_init(&(f_prof->bvar),"&");           /* start tag for variable */
      s_init(&(f_prof->evar),";");           /* end   tag for variable */


      gst_color (4) ;

      gcreat_seg (f_fmat->last_seg) ;


   }


   return(formdata);
}
