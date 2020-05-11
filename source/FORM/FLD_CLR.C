/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    fld_clr.c                                              |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <LEVEL>   FORMS.FFUN                                             |*/
/*|  <DATE>    September 26, 1989                                     |*/
/*|  <PARAM>   current  INF        INPUT   Pointer to struct of a fld |*/
/*|                                        to clear out all memory al-|*/
/*|                                        located to a field.        |*/
/*|                                                                   |*/
/*|  <PURPOSE> Remove an individual field.                            |*/
/*|  <OUTPUT>  sint                                                   |*/
/*|  <CALLS>              os_memd(), s_copy(), s_drop()               |*/
/*|  <MODS>                                                           |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"
#include "vars.h"
#include "ti.h"
#include "form_i.h"




sint fld_clr(      current )

INF              * current;

{







   if( current == NIL  )
   {


   }
   else
   {


      s_drop( & current->id       );
      s_drop( & current->prompt   );
      s_drop( & current->contents );
      s_drop( & current->defalt   );
      s_drop( & current->prv_cont );
      s_drop( & current->max      );
      s_drop( & current->min      );
      s_drop( & current->other    );
      s_drop( & current->help_id  );
      s_drop( & current->init_val );

      if( current->ocd != NIL )
      {

         ocd_drp( & (current->ocd)   );

      }
   }


   return(0);
}
