/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    frm_dump                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 15, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  TM_master->f_table     INPUT  PTR to base of linked    |
  |                                          list containing data     |
  |                                          of parsed forms          |
  |                                                                   |
  |  :PURPOSE. To dump the table of forms already parsed              |
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

sint frm_dump ()
{
   form_dat *formdata = NULL ;

                                  /* Get ptr to base of the table      */
   formdata = (form_dat *)
                       ((struct form_global *)TM_master->forms)->f_table;
                                  /* Check to see if table exists      */
   if (formdata == NULL)
   return(0);

   while (formdata != NULL)
   {

      formdata = formdata->next ;
   }

   return(0);
}
