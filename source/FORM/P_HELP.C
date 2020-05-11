/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    p_help ()                                              |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    September 3, 1987                                      |
  |  <LEVEL>   FORMS.PM                                               |
  |  <GLOBAL>  name        type      OUTPUT  description              |
  |                                                                   |
  |  <PARAM>   formdata    type      OUTPUT  description              |
  |  <PARAM>   p_prof      type      OUTPUT  description              |
  |                                                                   |
  |  <PURPOSE> Help was requested on a screen or field.  Locate and   |
  |            render the proper help form.  All help forms are of    |
  |            file type HELP unless they are in a TEIES object.      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>    Apr_03_88   JF    Added editor interface.              |
  |            Jul_20_89   JF    Added t_edtscr interface.            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form_i.h"
#include "form_x.h"




sint p_help (    formdata, p_prof  )

form_dat        *formdata ;
struct p_prof_s *p_prof   ;


{

   string
             var1          = S_NULL ,
             form_id       = S_NULL ,
             help_id       = S_NULL ,
             holder        = S_NULL ,
             state_id      = S_NULL ,
             command_line  = S_NULL ;

   sint      rc            =      0 ,
             key_hit       =      0 ;

   boolean   edit_help_form=  FALSE ;






                                 /* If there is no reference defined   */
   if (s_len (p_prof->help_dat) == 0)
   {

      s_copy ( & form_id, formdata->form_id, 0, 0) ;

   }
   else
   {

      s_copy ( & form_id, p_prof->help_dat, 0, 0  ) ;

   }
   s_copy( & help_id, form_id, 0, 0 );
                                  /* Attempt to give help              */
   key_hit = form (  form_id, NULL, HELP_FORM  ) ;

                                  /* If help form does not exist       */
   if (  key_hit == FORM_NOT_FORMED  )
   {

      except( FORMS_HELP_NOT_AVAILABLE, USER_ERROR, form_id, state_id,
                                                S_NULL, S_NULL, S_NULL );
      s_init ( & var1, "HELP_ID") ;
      var_set (  var1, form_id  ) ;

      s_init ( & form_id, "NO_HELP") ;
      key_hit = form ( form_id, NULL, HELP_FORM ) ;
      s_drop ( & var1 ) ;

      if( key_hit == FORM_EDIT_HELP )
      {

         edit_help_form = TRUE ;

      }
   }
   else
   {

      if( key_hit == FORM_EDIT_HELP )
      {

         edit_help_form = TRUE ;

      }
      rc = f_updfld( form_id, HELP_FORM );
      s_init( & command_line, "%CMD" );
      if( f_rstfld( form_id, HELP_FORM, command_line ))
      {                                            /********************
                                                    * Only attempt to  *
                                                    * check the command*
                                                    * line if one was  *
                                                    * on the form.  The*
                                                    * field is cleared *
                                                    * so we dont execut*
                                                    * it again next tme*
                                                    ********************/

         s_copy(  & holder, (string) var_get( command_line ), 0, 0 );
         s_ctrim( & holder, 'B', ' ' );
         s_ctrim( & holder, 'B', '_' );
         var_clr(   command_line     );
         rc = t_help( holder  );

      }
   }
   if( edit_help_form )
   {

      rc = t_edtscr( help_id, HELP_FORM, 0 );
      t_clrerr( TM_master->current->tm_pid );

   }

   f_close (   form_id, HELP_FORM ) ;
   s_drop (  & form_id            ) ;
   s_drop (  & help_id            ) ;

   return(rc);
}
