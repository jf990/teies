/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    frm_inta                                               |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    January 14, 1987                                       |
  |  <LEVEL>   FORMS                                                  |
  |  <GLOBAL>  TM_master->f_table form_dat INPUT  Pointer to form_dat |
  |                                               table.              |
  |                                                                   |
  |  <PARAM>   form_id            string   INPUT  Form ID of the form |
  |                                               you want or -1 for  |
  |                                               the last one.       |
  |                                                                   |
  |  <PURPOSE> This routine returns a pointer to the FORM_DAT         |
  |            given the desired form_id.                             |
  |                                                                   |
  |  <OUTPUT>  struct form_dat *                                      |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"

form_dat *frm_inta (form_id, flags)
string form_id ;                  /* Form Identifier                   */
hword  flags ;
{

   form_dat  *curform=  NIL ;     /* Current Form                      */
   sint       flag   =    0 ;     /* Return code from s_comp           */


if( TM_master->forms != NIL )
{

   curform = (form_dat *)
                      ((struct form_global *)TM_master->forms)->f_table ;

   if (curform != NIL)            /* If table doesn't exist            */
   {

      if (form_id == (string) -1 )
      {

         while (curform->next != NIL)
         {

            curform = curform->next ;

         }
      }
      else                        /* Find Form_dat with passed ID      */
      {

         while (  !  (
              (s_comp (form_id,curform->form_id,0,0,'M',&flag) == 0)
                                     &&
         (CHK_MASK(flags, HELP_FORM)==CHK_MASK(curform->flags,HELP_FORM))
               )     )
         {

            curform = curform->next ;
            if (curform == NIL)
            {

               curform = (form_dat *) -1;
               break;

            }
         }
      }
   }
   else
   {


   }
}
else
{

   if( form_id == (string) -1 )
   {

      except( FORMS_LEVEL_NOT_ACTIVE, CONTROL_ERROR, S_NULL,
                                        S_NULL, S_NULL, S_NULL, S_NULL );

   }
   else
   {

      except( FORMS_LEVEL_NOT_ACTIVE, CONTROL_ERROR, form_id,
                                        S_NULL, S_NULL, S_NULL, S_NULL );

   }
}

   return ( curform ) ;
}
