/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_bot_m                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    March 23, 1988                                         |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  TM_master->current->p_prof  struct p_prof_s * IN/OUT   |
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
  |  :PURPOSE. To process <bottomm> markup.                           |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   None.                                                  |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |  <MODS>    07_Dec_88       JF          Modified to WC 3.13B level.|
  |                                        Corrected readability, in- |
  |                                        dentation, removed 2 inter-|
  |                                        mediate D_Leave()s for one |
  |                                        return at the end.         |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "ti.h"
#include "debug.h"
#include "form_i.h"

sint   f_bot_m(          f_prof, f_fmat         )

struct         f_prof_s *f_prof;
struct         f_fmat_s *f_fmat;

{


   struct p_prof_s *p_prof = NIL; /* Pointer to PM's profile           */
   wind_dat        *bot_mat= NIL; /* Pointer to data on bottom window  */
   sint             rc     =   0;



                                  /* Clear Heading Markup if active    */
   if (    CHK_MASK (f_fmat->status, HEADING)   )
   {

      f_eh (f_prof, f_fmat) ;

   }
                                  /* Check to see if location OK       */
   if ( ! CHK_MASK (f_fmat->status, START) )
   {

      rc = -1;

   }
   else
   {
                                  /* Get pointer to PM's profile       */
                                  /* Check to see if in top matter     */

      p_prof = (struct p_prof_s *)
             ((struct form_process *) TM_master->current->forms)->p_prof;

      if (CHK_MASK (f_fmat->status, BOT_MAT) || p_prof->bot_mat != NULL)
      {

         rc = -1;

      }
      else
      {

                               /* Set status to indicate in top mat */
         SET_MASK (f_fmat->status, BOT_MAT) ;
                               /* Allocate space for window data    */
         bot_mat = (wind_dat *)os_mema (sizeof(wind_dat));
         p_prof->bot_mat = bot_mat;
                               /* Position cursor at left margin    */
         if (f_fmat->cur_x != f_fmat->org_x + f_fmat->lef_mar)
         {
            f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar ;
            f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
         }
                                  /* Close current segment             */
         gcls_seg () ;
                                  /* Increment segment counter         */
         f_fmat->last_seg +=1 ;
                                  /* Create new segment                */
         gcreat_seg (f_fmat->last_seg) ;
                                  /* Fill in data for window           */
         bot_mat->trans_nm = 3                ;
         bot_mat->frst_seg = f_fmat->last_seg ;
         bot_mat->last_seg = f_fmat->last_seg ;
         bot_mat->win_wid  = p_prof->scrn_wid ;
         bot_mat->win_len  = 1.0              ;
         bot_mat->w_min_x  = f_fmat->org_x + f_fmat->lef_mar ;
         bot_mat->w_max_x  = f_fmat->org_x + f_fmat->rig_mar ;
         bot_mat->w_min_y  = f_fmat->cur_y ;
         bot_mat->w_max_y  = f_fmat->cur_y ;
         bot_mat->v_min_x  = 0.0 ;
         bot_mat->v_max_x  = 1.0 ;
         bot_mat->v_min_y  = 0.0 ;
         bot_mat->v_max_y  = 1.0 ;


       }
   }

   return(rc);
}
