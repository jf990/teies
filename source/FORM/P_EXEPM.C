/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    p_exepm                                                |
  |  <AUTHOR>  JF                                                     |
  |  <DATE>    August 25, 1989                                        |
  |  <LEVEL>   FORMS.PM                                               |
  |  <GLOBAL>  TM_master->f_npmfun sint   INPUT  Number of elements   |
  |                                              in PF CMD structure  |
  |                                                                   |
  |  :GLOBAL.  TM_master->f_pmfun  byte * INPUT  Pointer to start of  |
  |                                              PF CMD structure     |
  |                                                                   |
  |  <PARAM>  *p_prof            form_dat IN/OUT Pointer to PM's pro- |
  |                                              file & INF's.        |
  |                                                                   |
  |  <PARAM>   function          sint     IN     Bit mask with flags  |
  |                                              for forms control.   |
  |                                                                   |
  |  <PURPOSE> Determine if a function selected is intended for the PM|
  |  <OUTPUT>  Boolean, true if PM function.                          |
  |  <ERRORS>  None.                                                  |
  |  <LIMITS>  None known.                                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "ti.h"
#include "form_i.h"




boolean     p_exepm ( function_req, p_prof )

hword                 function_req;
struct p_prof_s                   * p_prof ;


{

   sint                 rc          =     0,
                        pm_function =     0;
   boolean              handled     = FALSE;
   struct form_global * f_global    =   NIL;
   struct exec_pfk    * pm          =   NIL;




   switch( function_req )
   {

      case FORM_PAGE_DOWN:

              pm_function = PM_NEXT ;
              break;

      case FORM_PAGE_UP  :

              pm_function = PM_PREV ;
              break;

      case FORM_HOME     :

              pm_function = PM_HOME ;
              break;

      case FORM_TOP      :

              pm_function = PM_TOP  ;
              break;

      case FORM_BOTTOM   :

              pm_function = PM_BOT  ;
              break;

      case FORM_HNEXT    :

              pm_function = PM_HNEXT;
              break;

      case FORM_HPREV    :

              pm_function = PM_HPREV;
              break;

      case FORM_LEFT     :

              pm_function = PM_LEFT ;
              break;

      case FORM_RIGHT    :

              pm_function = PM_RIGHT;
              break;

      default            :

              pm_function = 0 ;
              handled     = FALSE;
              break;
   }


   if( pm_function != 0 )
   {

      f_global = (struct form_global *) TM_master->forms ;
      pm       = (struct exec_pfk    *) f_global->pm_fun ;
      rc       = ( * pm[ pm_function ].exe_fun ) ( p_prof );
      handled  = TRUE ;

   }

   return(handled);
}
