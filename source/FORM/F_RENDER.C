/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_render()                                             |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.                                                           |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  TM_master->current->p_prof  struct p_prof_s *  IN/OUT  |
  |                                        Pointer to the Presenta-   |
  |                                        tion Manager's profile.    |
  |                                                                   |
  |  :PARAM.   f_prof  struct f_prof_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains default & parsed  |
  |                                        qualifier values.          |
  |                                                                   |
  |  :PARAM.   f_fmat  struct f_fmat_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains info required to  |
  |                                        render data properly in    |
  |                                        the GKS world.             |
  |                                                                   |
  |  :PURPOSE. To render the text in the GKS world.                   |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "tm.h"
#include "debug.h"
#include "stack.h"
#include "form_i.h"


sint f_render (f_fmat, buffin, type, flag_ptr )

struct f_fmat_s *f_fmat   ;
string buffin             ;
hword  type               ;
byte  *flag_ptr           ;


{

   string buffer      = S_NULL,
          bufout      = S_NULL,
          buff_out    = S_NULL,
          blank       = S_NULL;
   real   width       = 0.0   ,
          pos         = 0.0   ;
   sint   nextchar    = 1     ,
          frstchar    = 0     ,
          count       =  0    ,
          bufinlen    = 0     ,
          llineout    = 0     ,
          hl          = 0     ,
        * high_lite   = NIL   ,
          len_line    = 0     ,
          len_to_disp = 0     ,
          len_to_disp_s = 0   ,
          pos_first     = 0   ;
   byte   testchar      = 0   ;
   bool   to_eol      = FALSE ,
          is_field    = FALSE ,
        * if_cond     = NULL  ;
   struct f_prof_s * f_prof = NULL;
   struct p_prof_s * p_prof = NULL;




f_prof = (struct f_prof_s * )
        ((struct form_process * ) TM_master->current->forms)->f_prof ;

if_cond  = (boolean *) stk_get ((STACK *) f_fmat->if_stack );


if (  NOT ( ( !(*if_cond) )  ||  ( !(f_prof->all_prse) ) )  )
{

   if (CHK_MASK(f_fmat->status, START  ) && \
      !CHK_MASK(f_fmat->status, TOP_MAT) && \
      !CHK_MASK(f_fmat->status, BOT_MAT)    )
   {
      CLR_MASK (f_fmat->status, START)   ;
      SET_MASK (f_fmat->status, FORMING) ;
      p_prof = (struct p_prof_s * )
           ((struct form_process * ) TM_master->current->forms)->p_prof ;
      p_prof->body->frst_seg = f_fmat->last_seg ;
      p_prof->body->last_seg = f_fmat->last_seg ;
      if (p_prof->top_mat != NULL)
         p_prof->body->v_max_y  = p_prof->top_mat->v_min_y ;
      if (p_prof->bot_mat != NULL)
         p_prof->body->v_min_y  = p_prof->bot_mat->v_max_y ;
      p_prof->body->w_min_y = f_fmat->cur_y ;
      p_prof->body->w_max_y = f_fmat->cur_y ;
   }

   s_copy ( & buffer, buffin, 0, 0);

   if (CHK_MASK (f_fmat->status, UPPERCASE))
      s_upper (buffer, 0, 0) ;

   if( (high_lite = (sint *) stk_get( (STACK *) f_fmat->hi_lite )) != NIL )
   {

      hl = * high_lite;

   }
   width    = f_fmat->rig_mar - f_fmat->lef_mar + 1.0 ;
   bufinlen = s_len (buffer) ;

   if ( NOT ( bufinlen == 0 && f_fmat->just != N_JUST ) )
   {



      if      (f_fmat->just == N_JUST)
      {
         if( f_fmat->ftype == FLD_INPUT      ||
             f_fmat->ftype == FLD_OUTPUT       )
         {
            sint  field_type = FINPUT;


            f_fmat->cur_fld->pos_x   = f_fmat->cur_x    ;
            f_fmat->cur_fld->pos_y   = f_fmat->cur_y    ;
            f_fmat->cur_fld->seg_num = f_fmat->last_seg ;
            if( f_fmat->ftype == FLD_OUTPUT )
            {
               field_type = FOUTPUT;
            }
            g_field( f_fmat->cur_x,f_fmat->cur_y,field_type,s_len(buffin),
                     buffin, f_fmat->echo, hl, flag_ptr) ;
            is_field = TRUE;

         }
         else
         {
            g_ttext (f_fmat->cur_x, f_fmat->cur_y, buffer) ;
         }
         if (type != F_RENDER_NCR)
         {
            f_fmat->cur_x = f_fmat->lef_mar ;
            f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
         } else
         {
            f_fmat->cur_x = f_fmat->cur_x + (real) s_len (buffer) + 1.0 ;
         }
      }
      else if (f_fmat->just == C_JUST)
      {
         if (f_fmat->cur_x != f_fmat->lef_mar)
         {
            f_fmat->cur_x = f_fmat->lef_mar ;
            f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
         }

         if (width > (real) bufinlen)
            pos = f_fmat->lef_mar + (width - (real) bufinlen) / 2.0 ;
         else
            pos = f_fmat->lef_mar ;

         if( f_fmat->ftype == FLD_INPUT || f_fmat->ftype == FLD_OUTPUT )
         {

            sint  field_type = FINPUT;

            f_fmat->cur_fld->pos_x   = f_fmat->cur_x    ;
            f_fmat->cur_fld->pos_y   = f_fmat->cur_y    ;
            f_fmat->cur_fld->seg_num = f_fmat->last_seg ;

            if( f_fmat->ftype == FLD_OUTPUT )
            {
               field_type = FOUTPUT;
            }
            g_field( pos, f_fmat->cur_y, field_type, s_len(buffin),
                     buffin, f_fmat->echo, hl, flag_ptr) ;
            is_field = TRUE;

         }
         else
         {

            g_ttext (pos, f_fmat->cur_y, buffer) ;

         }
         f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
      }
      else
      {
         s_init (&blank, " ");
         while (s_len (buffer) != 0)
         {
            len_line      = f_fmat->rig_mar - f_fmat->cur_x + 1.0;
            len_to_disp   = 0;
            if (f_fmat->ftype == FLD_NONE)
            {
               s_trim (&buffer, 'L');
            }
            pos_first     = s_first (blank, buffer, len_to_disp, 0);
            if (s_len (buffer) <= len_line)
            {
               pos_first = 0;
            }
            if ( (pos_first == 0) || (f_fmat->ftype != FLD_NONE) )
            {
               len_to_disp = s_len (buffer);
               pos_first = 0;
            }
            len_to_disp_s  = len_to_disp;
            while ( (len_to_disp <= len_line ) &&
                    (pos_first   != 0        ) )
            {
               len_to_disp_s = len_to_disp;
               pos_first     = s_first (blank, buffer, len_to_disp+1, 0);
               len_to_disp   = pos_first;
            }
            len_to_disp = len_to_disp_s;
            s_clip (&buff_out, &buffer, 0, len_to_disp);
            if (f_fmat->ftype == FLD_INPUT || f_fmat->ftype == FLD_OUTPUT)
            {

               sint  field_type = FINPUT;

               f_fmat->cur_fld->pos_x   = f_fmat->cur_x;
               f_fmat->cur_fld->pos_y   = f_fmat->cur_y;
               f_fmat->cur_fld->seg_num = f_fmat->last_seg;
               if (f_fmat->ftype == FLD_OUTPUT)
               {
                  field_type = FOUTPUT;
               }
            g_field( f_fmat->cur_x,f_fmat->cur_y,field_type,s_len(buffin),
                     buffin, f_fmat->echo, hl, flag_ptr) ;
               is_field = TRUE;

            } else
            {
               g_ttext (f_fmat->cur_x, f_fmat->cur_y, buff_out);
            }
            f_fmat->cur_x = f_fmat->cur_x + (real) len_to_disp + 1.0;
            if ( ( (s_len (buffer) != 0) && (buffer != S_NULL) ) ||
                 (f_fmat->cur_x >= f_fmat->rig_mar) )
            {
               f_fmat->cur_x  = f_fmat->lef_mar;
               f_fmat->cur_y  = f_fmat->cur_y - (real) f_fmat->spacing;
            }
         }
         s_drop (&blank);
      }

      if( high_lite != NIL )
      {

         os_memd( high_lite );

      }

      if (CHK_MASK (f_fmat->status, F_CENTER ))
      {
         f_ectr (f_prof, f_fmat) ;
      }

      if ( ! is_field )
      {

         s_drop ( & buffer );

      }
   }

   if( if_cond != NIL )
   {

      os_memd( if_cond );

   }

}


   return(0);
}
