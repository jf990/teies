/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    frm_drp                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 11, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  name        type      OUTPUT  description              |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. purpose                                                |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   None.                                                  |
  |                                                                   |
  |  :ERRORS.  None.                                                  |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"



sint frm_drp ( form_id, flags )

string         form_id ;
hword          flags ;


{

   sint                 rc       =   0;
   form_dat           * formdata = NIL;
   struct form_global * f_global = NIL;




   formdata = frm_inta (form_id, flags) ;
                                  /* Check if form exists              */
   if (formdata == NIL || formdata == (form_dat *) -1 )
   {

      rc = -1;

   }
   else
   {


      f_global = (struct form_global *) TM_master->forms ;

                                  /* Drop the string containing FormID */
      s_drop (&(formdata->form_id)) ;

      if (formdata->next == NULL && formdata->prev == NULL)
      {
         f_global->f_table = NULL ;
      }
      else
      {
         if (formdata->prev != NIL )
         {

            (formdata->prev)->next = formdata->next ;

         }
         else
         {

            f_global->f_table = (byte *)formdata->next ;

         }

         if (formdata->next != NIL )
         {

            (formdata->next)->prev = formdata->prev ;

         }
         else
         {

            (formdata->prev)->next = formdata->next ;

         }
      }

      if( formdata->p_prof != NIL )
      {


      }
      if( formdata->fst_inf != NIL )
      {


      }
      os_memd (  formdata  ) ;

   }

   return(rc);
}
