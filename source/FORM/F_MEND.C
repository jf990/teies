/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <name>    f_mend()                                               |
  |  <AUTHOR>  John L. Foster, Nestor Voronka                         |
  |  <DATE>    July 14, 1988                                          |
  |  <LEVEL>   Forms                                                  |
  |  <GLOBAL>  TM_master->f_cmds       Table of markups their argu-   |
  |                                    ment parsers and exe funcs     |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_ncmds      # of elements in above table   |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_pmfun      Table of PM's command keys and |
  |                                    their exe functions            |
  |                                                                   |
  |  <GLOBAL>  TM_master->f_npmfun     # of elements in above table   |
  |                                                                   |
  |  <PARAM>   None.                                                  |
  |                                                                   |
  |  <PURPOse> Un do what f_minit() has done, that is, deallocate     |
  |            all global forms structures and data areas.            |
  |                                                                   |
  |            Also - if there are any forms left in the world then   |
  |            call f_close() on each of them.                        |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   os_memd(), s_drop ()                                   |
  |  <ERRORS>                                                         |
  |  <LIMITS>  Was derived from f_minit(), so not only can this be    |
  |            smarter, but if an allocation is added to f_minit()    |
  |            then it better be deallocated here!                    |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"





sint f_mend ()


{


   sint   j                      =   -1 ;
   sint   k                      =   -1 ;
   struct form_global * f_global =  NIL ;
   struct _f_exe_cmd  * f_mcmds  =  NIL ;
   struct exec_pfk    * f_pmfun  =  NIL ;
   struct f_qual_s    * f_units  =  NIL ;
   struct f_qual_s    * f_colors =  NIL ;
   struct f_qual_s    * f_just   =  NIL ;
   struct f_qual_s    * m_quals  =  NIL ;
   struct f_gks_s     * f_world  =  NIL ;
   form_dat           * f_table  =  NIL ;




   f_global = (struct form_global *) TM_master->forms;

   if( f_global != NIL )
   {

      f_mcmds  =  ( struct _f_exe_cmd * ) f_global->f_cmds  ;
      f_pmfun  =  ( struct  exec_pfk  * ) f_global->pm_fun  ;
      f_units  =  ( struct  f_qual_s  * ) f_global->f_units ;
      f_colors =  ( struct  f_qual_s  * ) f_global->f_colors;
      f_just   =  ( struct  f_qual_s  * ) f_global->f_just  ;
      f_world  =  ( struct  f_gks_s   * ) f_global->f_world ;
      f_table  =  (         form_dat  * ) f_global->f_table ;

      if( f_table != NIL )
      {


         while( (f_table = frm_inta( (string) -1, NO_FLAGS )) != NIL )
         {

            f_close( f_table->form_id, f_table->flags );

         }
      }

      for (j = 0; j < f_global->f_ncmds; j++)
      {
         if (f_mcmds[j].quals.qual != S_NULL)
         {
            s_vdrop ( &(f_mcmds[j].quals.qual) );
         }
         os_memd (   f_mcmds[j].quals.id    );
         s_drop  ( &(f_mcmds[j].name      ) );
      }


      os_memd( f_mcmds  );
      os_memd( f_pmfun  );
      os_memd( f_units  );
      os_memd( f_colors );
      os_memd( f_just   );
      os_memd( f_world  );

      f_global->f_cmds   = NIL ;
      f_global->pm_fun   = NIL ;
      f_global->f_units  = NIL ;
      f_global->f_colors = NIL ;
      f_global->f_just   = NIL ;
      f_global->f_world  = NIL ;

      os_memd( TM_master->forms );
      TM_master->forms = NIL ;

   }


   return(0);
}
