/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    fld_new.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :DATE.    November 30, 1987                                      |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   id       string     INPUT   String containing the field|*/
/*|                                        id of a new field to add to|*/
/*|                                        the list of fields.        |*/
/*|                                                                   |*/
/*|  :PARAM.   f_prof   struct *   INPUT   Forms profile structure of |*/
/*|                                        initial field values.      |*/
/*|                                                                   |*/
/*|  :PURPOSE. Add and initialize a new form field.                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  current  INF *                                         |*/
/*|                                                                   |*/
/*|  :CALLS.              os_mema(), s_copy()                         |*/
/*|                       s_upper()                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.                                                           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
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

INF *fld_new(      f_prof      )

struct f_prof_s   *f_prof;

{

   INF        *current     =  NIL   ;  /* Holder for current inf values*/
   INF        *last        =  NIL   ;  /* Holder for current inf values*/
   struct f_fmat_s *f_fmat =  NIL   ;



   f_fmat = (struct f_fmat_s *)
           ((struct form_process *)TM_master->current->forms)->f_fmat;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Create a new inf entry to hold the passed values.               |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   current = (INF *) os_mema( sizeof( INF ) );
   os_memn( current, sizeof( INF )  );

   if ( FORMAT(fst_inf) == NIL )       /* Is this the 1st one?         */
   {

      FORMAT(fst_inf) = current;
      current->next   = NIL;            /* 1st inf points nowhere      */
      current->prev   = NIL;

   }
   else
   {

      if( NIL == fld_fnd( PROFILE( io_id ), f_fmat->rl_item ))
      {
                                  /* Get ptr to last inf               */
         last = f_fmat->fst_inf ;
         while (last->next != NULL)
            last = last->next ;
                                  /* Set Ptrs to proper values         */
         last->next = current ;
         current->prev = last ;
         current->next = NULL ;
      }
      else
      {

         os_memd ( current );
   return(0);

      }
   }

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Initialize the new input field.                                 |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   current->ocd              = ocd_new( &(current->ocd), NIL );
   current->ocb              = f_fmat->ocb;

   fld_stem ( &(PROFILE (io_id   )), PROFILE( io_id     )
                                   , f_fmat->rl_item           );
   s_copy   ( &(current->ocd->name), PROFILE( io_name   ), 0, 0);
   s_copy   ( &(current->prompt)   , PROFILE( io_prompt ), 0, 0);
   s_copy   ( &(current->defalt)   , PROFILE( io_def    ), 0, 0);
   s_copy   ( &(current->id)       , PROFILE( io_id     ), 0, 0);
   s_copy   ( &(current->help_id)  , PROFILE( io_help   ), 0, 0);
   s_upper  (  current->id,                                0, 0);

   current->just      = PROFILE( io_just );
   current->pad       = PROFILE( io_pad  );
   current->len       = PROFILE( io_len  );
   current->ftype     = PROFILE( io_io   );
   current->echo      = PROFILE( io_echo );
   current->type      =  -1    ;
   current->pos_x     =    0.0 ;
   current->pos_y     =    0.0 ;
   current->seg_num   =  -1    ;
   current->stem      = f_fmat->rl_item;

   return(current);
}
