/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_curpos                                               |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    June 1, 1988                                           |
  |  <LEVEL>   FORMS                                                  |
  |  <PARAM>   form_id    string   INPUT   Form ID of form on which   |
  |                                        the cursor is to be        |
  |                                        repositioned               |
  |  <PARAM>   flags      flags    INPUT   Flags indicating types of  |
  |                                        form                       |
  |  <PARAM>   fld_name   string   INPUT   Name of field at which to  |
  |                                        position the cursor.       |
  |  <PARAM>   pos_x      real     INPUT   X offset of cursor in      |
  |                                        relation to above field.   |
  |  <PARAM>   pos_y      real     INPUT   Y offset of cursor in      |
  |                                        relation to above field.   |
  |  <PURPOSE> To position the cursor at a certain field in a form.   |
  |  <OUTPUT>  sint        Returns -1 if error occurs and 0 otherwise.|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "debug.h"
#include "form_i.h"

sint f_curpos (form_id, flags, fld_name, pos_x, pos_y)
string form_id  ;
sint   flags    ;
string fld_name ;
real   pos_x    ;
real   pos_y    ;
{
   form_dat *formdata = NULL  ;   /* Pointer to form data structure    */
   INF      *cur_inf  = NULL  ;   /* Pointer to current field inf      */
   boolean   found    = FALSE ;   /* Was the field entry found         */
   area      fld_pos          ;   /* GKS structure containing coord's  */
   sint      rc       = 0     ;   /* Return code from s_comp           */
   tint      page     = 0     ;   /* Page on which field is found      */
   real      ypos     = 0.0   ;   /* Y coordinate of field             */
struct p_prof_s *p_prof= NULL ;   /* Pointer to PM's profile           */
   wind_dat *body     = NULL  ;   /* Pointer to PM's window informatn  */


                                  /* Check to see if form is in table  */
formdata = frm_inta (form_id, flags) ;

if (formdata == NULL || formdata == (form_dat *)0xFFFFFFFF )
{

   rc = -1;

}
else
{
                                  /* Get pointer to first field entry  */
   cur_inf = formdata->fst_inf ;

   while (cur_inf != NULL && !found)
   {
      if ( s_comp (cur_inf->ocd->name, fld_name, 0, 0, 'M', &rc) != 0)
      {                           /* Go to next field entry            */
         cur_inf = cur_inf->next ;
      }
      else                        /* Get coordinates of field          */
      {
         fld_pos.xmin =        cur_inf->pos_x + pos_x ;
         fld_pos.ymax = ypos = cur_inf->pos_y + pos_y ;
         found        =        TRUE           ;
      }
   }
                                  /* Check to see if field was found   */
   if ( ! found )
   {

      rc = -1;

   }
   else
   {
                                     /* Get pointer to PM's info       */
      p_prof = (struct p_prof_s *) formdata->p_prof ;
      body   = (wind_dat *) p_prof->body ;
                                     /* If field is in body of form    */
      if (ypos >= body->w_min_y && ypos <= body->w_max_y)
      {                              /* Compute page of field          */

         page = (tint)(body->w_max_y - ypos) / (tint)(body->win_len+1.0);
                                     /* Set new current Y position     */
         p_prof->w_cur_y = body->w_max_y - body->win_len - \
                          (body->win_len + 1.0) * (real) page ;

      }
                                     /* Initialize string input device */
      g_str_ini (p_prof->wk_id, p_prof->wk_id, S_NULL, 1, & fld_pos, 0) ;

   }
}


   return(rc);
}
