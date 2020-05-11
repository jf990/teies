/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_sfield.c                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   f_prof  struct f_prof_s * INPUT  Parser format variable|*/
/*|  :PARAM.   f_fmat  struct f_fmat_s * INOUT  Function format vars  |*/
/*|  :PARAM.   wanted  string            INPUT  String indicating     |*/
/*|                                             field id to use at the|*/
/*|                                             current location in   |*/
/*|                                             the form to place a   |*/
/*|                                             field.                |*/
/*|                                                                   |*/
/*|  :PURPOSE. Processes imbedded variable markups (&var;) in the form|*/
/*|            by getting current field contents from the INF struct. |*/
/*|            If an input field then the format input flag is set so |*/
/*|            f_render() can worry about setting up the proper input |*/
/*|            field.  F_field still has to worry about length,       |*/
/*|            prompt chars, default, padding, etc.                   |*/
/*|                                                                   |*/
/*|            Prompt characters are placed outside the field, so if  |*/
/*|            the field is valid the begin prompt char is output b4  |*/
/*|            the GKS segment is updated.  When the field is complete|*/
/*|            the segment is closed and the ending prompt char is put|*/
/*|            in the next segment and f_rendered.                    |*/
/*|                                                                   |*/
/*|            All current attributes are checked to insure the field |*/
/*|            is generated with active attribute values.             |*/
/*|                                                                   |*/
/*|  :OUTPUT.  Always 0.                                              |*/
/*|                                                                   |*/
/*|  :CALLS.   stk_get(), gs_high(), gs_scolor(), os_memd(), gcls_seg)|*/
/*|            s_copy(), f_render(), gcreat_seg()                     |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    14_Dec_87   J.L.Foster                Written.         |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "stack.h"
#include "debug.h"
#include "bse.h"
#include "ti.h"
#include "form_i.h"


sint f_sfield( name , f_prof, f_fmat )

string             name  ;
struct   f_prof_s *f_prof;
struct   f_fmat_s *f_fmat;

{

   INF     *fld_ptr  =  NIL   ;        /* Pointer for checking INF's   */
   sint     rc       =  0     ;        /* RC from things called        */
   boolean  found    =  FALSE ;        /* True if we find the variable */
   boolean  prompt   =  FALSE ;        /* True if we place prompt chars*/
   string   promptch =  S_NULL;        /* String with prompt character */
   sint    *color_f  = NIL    ;        /* Foreground color             */
   sint    *hi_lite  = NIL    ;        /* Highlighting level           */

   s_upper(name,  0,  0        );

   fld_ptr = fld_fnd( name, f_fmat->rl_item );

   if ( ( NIL == fld_ptr || fld_ptr == (INF *) 0xFFFFFFFF ) ||
        ( !f_prof->var_prse) || ( !f_prof->all_prse) )
   {

       /*+------------------------------------------------------------+*/
       /*|                                                            |*/
       /*| Could not find--return the VARIABLE given.                 |*/
       /*|                                                            |*/
       /*+------------------------------------------------------------+*/

      rc = -1;

   } else
   {
      if( fld_ptr->prompt != S_NULL && s_len(fld_ptr->prompt) > 0 )
      {

         s_copy( &promptch, fld_ptr->prompt, 1, 1 );
         f_render( f_fmat, promptch, F_RENDER_NCR, NIL ) ;
         prompt = TRUE;

      }
      gcls_seg  (  );
      gcreat_seg( ++(f_fmat->last_seg));

      hi_lite = (sint *) stk_get( (STACK *) f_fmat->hi_lite );
      color_f = (sint *) stk_get( (STACK *) f_fmat->color_f );

      if( hi_lite != NIL )
         gs_shigh( f_fmat->last_seg, *hi_lite );
      if( color_f != NIL )
         gs_scolor( f_fmat->last_seg, *color_f );

      f_fmat->cur_fld  = fld_ptr ;
      fld_ptr->seg_num = f_fmat->last_seg;
      f_fmat->ftype    = fld_ptr->ftype ;
      f_fmat->echo     = fld_ptr->echo  ;
      f_render(f_fmat,fld_ptr->contents, F_RENDER_NCR,&(fld_ptr->flags));

      f_fmat->ftype = FLD_NONE ;

      gcls_seg  (  );
      gcreat_seg( ++(f_fmat->last_seg));

      if( hi_lite != NIL )
         gs_shigh( f_fmat->last_seg, *hi_lite );
      if( color_f != NIL )
         gs_scolor( f_fmat->last_seg, *color_f );

      os_memd (hi_lite) ;
      os_memd (color_f) ;

      if( prompt )
      {
         s_copy( &promptch, fld_ptr->prompt, 2, 1     ) ;
         f_render( f_fmat, promptch, F_RENDER_NCR, NIL ) ;
         s_drop (&promptch) ;
      }
   }

   return(rc);
}
