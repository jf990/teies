/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_qhelp                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 15, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.PM                                               |
  |                                                                   |
  |  :GLOBAL.  name        type      OUTPUT  description              |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. This routine checks all the input fields to determine  |
  |            if help was requested on any particular field.         |
  |                                                                   |
  |  :OUTPUT.  Returns TRUE if help was asked for and returns FALSE   |
  |            otherwise.                                             |
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

#include "stdio.h"
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"



boolean p_qhelp ( formdata, p_prof, flags )

form_dat        * formdata ;
struct p_prof_s * p_prof   ;
hword             flags    ;


{

   INF      *cur_fld  =   NULL ;  /* Pointer to current field          */
   string    fld_val  = S_NULL ;  /* Value of current field            */
   sint      rc       =      0 ;  /* Return code   from s_comp         */
   sint      flag     =      0 ;  /* Flag returned from s_comp         */
   boolean   get_help =  FALSE ;  /* Was any help requested ?          */





   if (CHK_MASK (flags, HELP_FORM))
   return(FALSE);


                                  /* Get Ptr to first field structure  */
   cur_fld = formdata->fst_inf ;
                                  /* Go through all the fields         */
   while ( cur_fld != NIL )
   {
                                  /* If it's an input field then check */
                                  /*    if help was requested          */

      if (  cur_fld->ftype == FLD_INPUT  )
      {


         s_nulout (  & (p_prof->help_dat)  ) ;
         s_copy (    & fld_val,      cur_fld->contents, 0, 0 ) ;
         s_ctrim (   & fld_val, 'B', cur_fld->pad            ) ;

         rc =  s_comp ( fld_val, TM_master->ucb->help_rqc,
                      1,  s_len( TM_master->ucb->help_rqc ),
                                                          'M', & flag ) ;

         if (rc == 0)
         {                        /*************************************/
                                  /* Specify what to get help on.  If  */
                                  /* the field definition specified a  */
                                  /* HELP= use that, otherwise default */
                                  /* to the field name as the key for  */
                                  /* the location of the help info.    */
                                  /*************************************/

            if (  s_len(  cur_fld->help_id  ) > 0  )
            {

               s_copy ( & (p_prof->help_dat), cur_fld->help_id, 0, 0  );

            }
            else
            {

               s_copy ( & (p_prof->help_dat), cur_fld->ocd->name, 0, 0 );

            }

            p_help (  formdata, p_prof  ) ;

            s_copy ( & (cur_fld->contents), cur_fld->prv_cont, 0, 0 ) ;

            get_help = TRUE ;

         }
      }
      cur_fld = cur_fld->next ;

   }

   s_drop ( & fld_val  ) ;

   return(get_help);


}
